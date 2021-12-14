#include "stdafx.h"

#include "helper.h"
#include "ThreadDataPath.h"
#include "FormMain.h"
using namespace System::Collections::Generic;

#pragma comment(lib, "User32.lib")

using namespace System::Text;

using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

namespace Ambiesoft {
	namespace retrycopy {
		Object^ FormMain::EndInvokeWithTN(int tn, IAsyncResult^ ir)
		{
			if (tn != ThreadTransitory::ThreadNumber)
				return nullptr;

			return Form::EndInvoke(ir);
		}

		//bool ddd(LPCWSTR pThreadName)
		//{
		//	typedef HRESULT(WINAPI* TSetThreadDescription)(HANDLE, PCWSTR);
		//	HMODULE hKernel32 = GetModuleHandleA("Kernel32.dll");
		//	if (hKernel32 == nullptr) {
		//		return false;
		//	}
		//	TSetThreadDescription f = reinterpret_cast<TSetThreadDescription>(
		//		GetProcAddress(hKernel32, "SetThreadDescription"));
		//	if (f == nullptr) {
		//		return false;
		//	}
		//	return SUCCEEDED(f(GetCurrentThread(), pThreadName));
		//}
		void FormMain::StartOfThreadMaster(Object^ obj)
		{
			ThreadDataMaster^ thDataMaster = (ThreadDataMaster^)obj;

			Thread::CurrentThread->Name = "Copy Thread";

			EndInvokeWithTN(thDataMaster->ThreadNumber,
				BeginInvoke(gcnew VIDelegate(this, &FormMain::ThreadStarted),
					thDataMaster->ThreadNumber));

			if (!StartOfThreadMaster2(thDataMaster))
				return;

			BeginInvoke(gcnew VTmDelegate(this, &FormMain::ThreadFinished), thDataMaster);
		}

		enum class PathType
		{
			NonExsitent, File, Directory
		};
		PathType GetPathType(String^ path)
		{
			if (File::Exists(path))
				return PathType::File;
			if (Directory::Exists(path))
				return PathType::Directory;
			return PathType::NonExsitent;
		}
		bool GetCultureStringFor_setlocale(std::string& ret)
		{
			try
			{
				System::Globalization::CultureInfo^ ci = System::Threading::Thread::CurrentThread->CurrentCulture;
				if (!ci)
					return false;

				String^ t3 = ci->ThreeLetterISOLanguageName;
				if (!t3)
					return false;

				ret = toStdAcpString(TO_LPCWSTR(t3));
				return true;
			}
			catch (Exception^)
			{

			}

			return false;
		}
		String^ GetCErrorString(int err)
		{
			//std::string loc;
			//if (GetCultureStringFor_setlocale(loc))
			//	setlocale(LC_ALL, "");// loc.c_str());
			//char b[1024];
			//char* pp = strerror(err);
			wchar_t buff[1024];
			if (0 != _wcserror_s(buff, err))
				return I18N(L"Unknown error");
			return gcnew String(buff);
		}
		String^ FormMain::CheckFiles(ThreadDataMaster^ thDataMaster, bool% bCancel)
		{
			String^ initialError;
			PathType ptDst = GetPathType(thDataMaster->Dst);
			do
			{
				bool isSrcDir = false;
				if (thDataMaster->IsSrcPathsEmpty)
				{
					initialError = I18N(L"The source is empty.");
					break;
				}

				for each (String ^ path in thDataMaster->SrcPaths)
				{
					PathType ptSrc = GetPathType(path);
					if (ptSrc == PathType::Directory)
						isSrcDir = true;
					if (ptSrc == PathType::NonExsitent)
					{
						initialError =
							String::Format(I18N(L"The source path '{0}' does not exist."),
								path);
						break;
					}
				}
				if (!String::IsNullOrEmpty(initialError))
					break;

				if (String::IsNullOrWhiteSpace(thDataMaster->Dst))
				{
					initialError = I18N(L"The destination path is empty.");
					break;
				}

				//bool bDstIsDir = thDataMaster->SrcPaths->Length > 1 ||
				//	isSrcDir ||
				//	thDataMaster->Dst->EndsWith("\\") || thDataMaster->Dst->EndsWith("/");

				List<String^> processedSrcPath;
				List<KeyValuePair<String^,String^>> processedSrcName;
				for each (String ^ srcpath in thDataMaster->SrcPaths)
				{
					// duplicate src check
					for each (String ^ s in processedSrcPath)
					{
						if (stdIsSamePath(TO_LPCWSTR(s), TO_LPCWSTR(srcpath)))
						{
							initialError = String::Format(I18N(L"The source paths '{0}' and '{1}' are same path."),
								s, srcpath);
							DASSERT(!String::IsNullOrEmpty(initialError));
							break;
						}
					}
					if (!String::IsNullOrEmpty(initialError))
						break;
					processedSrcPath.Add(srcpath);

					// duplicate src name
					String^ srcName = Path::GetFileName(srcpath);
					for each (KeyValuePair<String^,String^> kv in processedSrcName)
					{
						if (String::Compare(kv.Value, srcName, true) == 0)
						{
							initialError = String::Format(I18N(L"The source path '{0}' and '{1}' have a same name."),
								kv.Key, srcpath);
							DASSERT(!String::IsNullOrEmpty(initialError));
							break;
						}
					}
					if (!String::IsNullOrEmpty(initialError))
						break;
					processedSrcName.Add(KeyValuePair<String^, String^>(srcpath, srcName));

					if (stdIsSamePath(TO_LPCWSTR(srcpath), TO_LPCWSTR(thDataMaster->Dst)))
					{
						initialError = String::Format(
							I18N(L"The source path '{0}' and destination directory '{1}' is same."),
							srcpath, thDataMaster->Dst);
						break;
					}

					if (File::Exists(thDataMaster->Dst))
					{
						initialError = I18N(L"The destination path must be a directory, but it exists as a file.");
						break;
					}

					if (Directory::Exists(srcpath))
					{
						if (stdIsSubDirectory(TO_LPCWSTR(srcpath), TO_LPCWSTR(thDataMaster->Dst)))
						{
							initialError = String::Format(
								I18N(L"The destination directory is a subdirectory of the source path '{0}'."),
								srcpath);
							break;
						}
					}
					else
					{
						DASSERT(File::Exists(srcpath));
						DASSERT(!stdIsSamePath(TO_LPCWSTR(srcpath), TO_LPCWSTR(thDataMaster->Dst)));
						//if (bDstIsDir)
						//{
						//	if (File::Exists(thDataMaster->Dst->TrimEnd((gcnew String(L"/\\"))->ToCharArray())))
						//	{
						//		initialError = I18N(L"The destination name ends with path separator but it exists as a file.");
						//		break;
						//	}
						//}
					}
				}

				if (String::IsNullOrEmpty(initialError) &&
					!Directory::Exists(thDataMaster->Dst))
				{
					if (!(bool)EndInvokeWithTN(
						thDataMaster->ThreadNumber,
						BeginInvoke(
							gcnew BISDelegate(this, &FormMain::OnThreadYesNo),
							thDataMaster->ThreadNumber,
							String::Format(
								I18N(L"'{0}' does not exist. Do you want to create a new directory?"),
								thDataMaster->Dst
							) // String::Format
						) // BeginInvoke
					) // EndInvoke
						) // if
					{
						bCancel = true;
						return nullptr;
					}

					stdCreateCompleteDirectory(TO_LPCWSTR(thDataMaster->Dst));
					int errorn = errno;
					if (!Directory::Exists(thDataMaster->Dst))
					{
						initialError = String::Format(I18N(L"Failed to create a directory '{0}' ({1})."),
							thDataMaster->Dst, GetCErrorString(errorn));
						break;
					}
				}
			} while (false);
			return initialError;
		}
		String^ AppendBackSlash(String^ s)
		{
			DASSERT(!String::IsNullOrEmpty(s));
			if (s->EndsWith(L"\\") || s->EndsWith(L"/"))
				return s;
			return s + L"\\";
		}
		bool FormMain::StartOfThreadMaster2(ThreadDataMaster^ thDataMaster)
		{
			String^ initialError;
			if (!CDebug::IsMockReadFile)
			{
				bool bCancel = false;
				initialError = CheckFiles(thDataMaster, bCancel);
				if (bCancel)
					return true;
			}
			if (!String::IsNullOrEmpty(initialError))
			{
				EndInvokeWithTN(
					thDataMaster->ThreadNumber,
					BeginInvoke(gcnew VISDelegate(this, &FormMain::OnThreadAlert),
						thDataMaster->ThreadNumber,
						initialError));
				return true;
			}

			if (CDebug::IsMockReadFile)
			{
				KVS^ sds = gcnew KVS();
				sds->Add(KV(nullptr, Path::Combine(thDataMaster->Dst, L"mofile")));
				List<String^> dstDirs;
				thDataMaster->AddTask(gcnew ThreadDataPath(L"dummy", sds, %dstDirs));
			}
			else
			{
				for each (String ^ path in thDataMaster->SrcPaths)
				{
					List<String^>^ dstDirs;
					KVS^ sds;
					try
					{
						String^ message = I18N(L"Obtaining source files and directories...");
						BeginInvoke(gcnew VSDelegate(this, &FormMain::ThreadLog), message);
						ThreadTransitory::SetProgress(message);
						sds = AmbLib::GetSourceAndDestFiles(path, 
							AppendBackSlash(thDataMaster->Dst),
							dstDirs);
#ifdef _DEBUG
						// check sds is all same filename
						for each(KV kv in sds)
						{
							DASSERT(Path::GetFileName(kv.Key) == Path::GetFileName(kv.Value));
						}
#endif
					}
					catch (Exception^ ex)
					{
						BeginInvoke(gcnew VISDelegate(this, &FormMain::OnThreadError),
							thDataMaster->ThreadNumber,
							ex->Message);
						return false;
					}
					thDataMaster->AddTask(gcnew ThreadDataPath(path, sds, dstDirs));
				}
			}

			try
			{
				thDataMaster->SetTaskStarted();

				String^ message;

				// calc total input size
				message = I18N(L"Calculating total size...");
				BeginInvoke(gcnew VSDelegate(this, &FormMain::ThreadLog), message);
				ThreadTransitory::SetProgress(message);

				LONGLONG totalInputSize = 0;
				int totalFileCount = 0;
				if (CDebug::IsMockReadFile)
				{
					totalInputSize = CReadingFileMock::FILESIZE;
					totalFileCount = 1;
				}
				else
				{
					for each (ThreadDataPath ^ thDataPath in thDataMaster->Tasks)
					{
						for each (KV kv in thDataPath->SDS)
						{
							System::IO::FileInfo fi(kv.Key);
							totalInputSize += fi.Length;
							++totalFileCount;
						}
					}
				}
				thDataMaster->SetTotalSize(totalInputSize, totalFileCount);
				ThreadTransitory::TotalCount = totalFileCount;

				for each (ThreadDataPath ^ thDataPath in thDataMaster->Tasks)
				{
					// prepare target dirs
					message = I18N(L"Preparing target directories...");
					BeginInvoke(gcnew VSDelegate(this, &FormMain::ThreadLog), message);
					ThreadTransitory::SetProgress(message);
					thDataPath->PrepareDstDirs();


					for (int i = 0; i < thDataPath->SDS->Count; ++i)
					{
						ThreadTransitory::ProcessedTotalCount = i + 1;
						ThreadDataFile^ tdf = gcnew ThreadDataFile(
							thDataMaster->ThreadNumber, i + 1,
							thDataPath->SDS[i].Key, thDataPath->SDS[i].Value);

						StartOfThreadFile(tdf);
						ThreadTransitory::SetProgress(String::Empty);

						if (thDataMaster->ThreadNumber != ThreadTransitory::ThreadNumber)
							return false;
						thDataMaster->TotalProcessedSize += tdf->ProcessedSize;
						tdf->CloseFiles();

						if (tdf->IsSkipped)
						{
							DASSERT(!tdf->IsOK);
							thDataPath->IncrementSkipCount();
						}
						if (tdf->IsOK)
						{
							thDataPath->IncrementOK();
							AmbLib::CopyFileTime(tdf->SrcFile, tdf->DstFile,
								AmbLib::CFT::Creation);
						}
						EndInvokeWithTN(
							tdf->ThreadNumber,
							BeginInvoke(gcnew VTfDelegate(this, &FormMain::ThreadFileEnded),
								tdf));
						thDataMaster->AppendEnded(tdf);
					}

					EndInvokeWithTN(
						thDataMaster->ThreadNumber,
						BeginInvoke(gcnew VTpDelegate(this, &FormMain::ThreadPathFinished),
							thDataPath));
				}
			}
			catch (ThreadAbortException^) {}
			catch (Exception^ ex)
			{
				BeginInvoke(gcnew VISDelegate(this, &FormMain::OnThreadError),
					thDataMaster->ThreadNumber,
					ex->Message);
				return false;
			}
			return true;
		}

#define RETURNIFTHREADNUMBER do { if(thFileData->ThreadNumber != ThreadTransitory::ThreadNumber) { return; } }while(false)
		void FormMain::StartOfThreadFile(ThreadDataFile^ thFileData)
		{
			RETURNIFTHREADNUMBER;

			EndInvokeWithTN(
				thFileData->ThreadNumber,
				BeginInvoke(gcnew VTfDelegate(this, &FormMain::ThreadFileStarted),
					thFileData));
			thFileData->ProcessedSize = 0;

			// first check
			if (!thFileData->FirstCheck())
			{
				thFileData->SetSkipped();
				return;
			}
			bool initSrc = true;
			bool bWZOMode = false;
			int retried = 0;
			int bufferSize = 0;
			std::unique_ptr<BYTE[]> bb;
			unsigned consecutiveErrorCount = 0;
			unsigned sameErrorCount = 0;
			DWORD lastError = 0;

			RETURNIFTHREADNUMBER;

			thFileData->SetTransferStarted();
			do
			{
				if (bufferSize != ThreadTransitory::UserBuffer)
				{
					bufferSize = ThreadTransitory::UserBuffer;
					DASSERT(bufferSize > 0);
					bb.reset(new BYTE[bufferSize]);
				}
				if (initSrc)
				{
					initSrc = false;
					switch (thFileData->InitSrc())
					{
					case ThreadDataFile::INITSRCRET::INITSRC_CANNOTOPEN:
						// ask user
						if ((bool)EndInvokeWithTN(
							thFileData->ThreadNumber,
							BeginInvoke(
								gcnew BIDwDelegate(this, &FormMain::OpenFileFailedGetUserAction),
								thFileData->ThreadNumber,
								thFileData->SrcLE)))
						{
							// retry
							initSrc = true;
							continue;
						}
						return;
					case ThreadDataFile::INITSRCRET::INITSRC_SIZECHANGED:
						// first time or size changed
						thFileData->ProcessedSize = 0;
						thFileData->InitDst();
						break;
					case ThreadDataFile::INITSRCRET::INITSRC_OK:
						// size not zero and not changed
						break;
					}
				}
				if (!thFileData->SetReadingPos(thFileData->ProcessedSize))
				{
					RETURNIFTHREADNUMBER;
					const DWORD le = GetLastError();

					if (retried++ < ThreadTransitory::UserRetry ||
						ThreadTransitory::UserRetry < 0)
					{
						// Do retry
						ThreadTransitory::SetLastErrorDisp(
							thFileData->ProcessedSize,
							thFileData->SrcSize,
							le,
							retried);
						initSrc = true;
						System::Threading::Thread::Sleep(100);
						continue;
					}

					// Ask user
					UserResponceOfFail^ sfpFail = (UserResponceOfFail^)
						EndInvokeWithTN(
							thFileData->ThreadNumber,
							BeginInvoke(gcnew RISDLLLLDwIDelegate(this, &FormMain::SFPFailedGetUserAction),
								thFileData->ThreadNumber,
								thFileData->SrcFile,
								thFileData->ProcessedSize,
								thFileData->SrcSize,
								le,
								retried));
					if (!sfpFail)
					{
						DASSERT(thFileData->ThreadNumber != ThreadTransitory::ThreadNumber);
						return;
					}
					if (sfpFail->IsCancel)
						return;
					if (sfpFail->IsRetry)
					{
						retried = 0;
						initSrc = true;
						continue;
					}
					DASSERT(false);
					return;
				}

				ThreadTransitory::SetReadingProgress(
					thFileData->ProcessedSize,
					bufferSize,
					retried);
				DWORD dwRead;
				if (!thFileData->ReadFromFile(bufferSize, bb.get(), &dwRead))
				{
					RETURNIFTHREADNUMBER;
					const DWORD le = GetLastError();
					if (le == lastError)
					{
						if (++sameErrorCount > 100)
						{
							initSrc = true;
						}
					}
					lastError = le;
					if (++consecutiveErrorCount > 100)
					{
						thFileData->setReadError(le);
						return;
					}
					//Thread::Sleep(Math::Min(consecutiveErrorCount, 100u));
					if (!bWZOMode)
					{
						if (retried++ < ThreadTransitory::UserRetry ||
							ThreadTransitory::UserRetry < 0)
						{
							ThreadTransitory::SetLastErrorDisp(
								thFileData->ProcessedSize,
								thFileData->SrcSize,
								le,
								retried);
							if (ShouldReopenError(le))
							{
								initSrc = true;
							}
							continue;
						}
						// fail
						UserResponceOfFail^ rfd = (UserResponceOfFail^)
							EndInvokeWithTN(
								thFileData->ThreadNumber,
								BeginInvoke(gcnew RISDLLLLDwIIIDelegate(this, &FormMain::ReadFileFailedGetUserAction),
									thFileData->ThreadNumber,
									thFileData->SrcFile,
									thFileData->ProcessedSize,
									thFileData->SrcSize,
									le,
									retried,
									ThreadTransitory::UserRetry,
									bufferSize));
						if (!rfd)
						{
							DASSERT(thFileData->ThreadNumber != ThreadTransitory::ThreadNumber);
							return;
						}
						if (rfd->IsCancel)
							return;

						// Change bufferSize to user's specified size
						DASSERT(ThreadTransitory::UserBuffer == rfd->BufferSize);
						ThreadTransitory::UserBuffer = rfd->BufferSize;

						if (rfd->IsRetry)
						{
							retried = 0;
							if (ShouldReopenError(le))
							{
								initSrc = true;
							}
							continue;
						}

						if (rfd->IsWZOMode)
						{
							bWZOMode = true;
							DASSERT(ThreadTransitory::UserBuffer == 1);
							ThreadTransitory::UserBuffer = 1;
							retried = 0;
							continue;
						}
						if (bufferSize != ThreadTransitory::UserBuffer)
						{
							bufferSize = ThreadTransitory::UserBuffer;
							DASSERT(bufferSize > 0);
							bb.reset(new BYTE[bufferSize]);
						}
					}
					else
					{
						// WZOMode
						if (retried++ < ThreadTransitory::UserRetry ||
							ThreadTransitory::UserRetry < 0)
						{
							ThreadTransitory::SetLastErrorDisp(
								thFileData->ProcessedSize,
								thFileData->SrcSize,
								le,
								retried);
							if (ShouldReopenError(le))
							{
								initSrc = true;
							}
							continue;
						}
						sameErrorCount = 0;
						consecutiveErrorCount = 0;
						retried = 0;
					}
					EndInvokeWithTN(
						thFileData->ThreadNumber,
						BeginInvoke(gcnew VILLIDelegate(this, &FormMain::ProgressWriteWithZero),
							thFileData->ThreadNumber,
							thFileData->ProcessedSize,
							bufferSize));
					ZeroMemory(bb.get(), bufferSize);
					dwRead = bufferSize;
					thFileData->AddZeroWritten(bufferSize);
				}
				else
				{
					// ReadFile OK
					bWZOMode = false;
					ThreadTransitory::ClearLastErrorDisp();
				}

				consecutiveErrorCount = 0;
				lastError = 0;
				ThreadTransitory::SetWrittingProgress(
					thFileData->ProcessedSize,
					bufferSize);

				RETURNIFTHREADNUMBER;

				DWORD dwWritten;
				if (!(WriteFile(thFileData->HDst,
					bb.get(),
					dwRead,
					&dwWritten,
					NULL) && dwWritten == dwRead))
				{
					thFileData->setWriteError(GetLastError());
					return;
				}
				DASSERT(dwRead == dwWritten);
				thFileData->ProcessedSize += dwWritten;
				retried = 0;

				if (dwRead == 0)
				{
					thFileData->SetDone();
					return;
				}
			} while (thFileData->ThreadNumber == ThreadTransitory::ThreadNumber);
		}
	}
}