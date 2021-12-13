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

			StartOfThreadMaster2(thDataMaster);

			BeginInvoke(gcnew VTmDelegate(this, &FormMain::ThreadFinished), thDataMaster);
		}
		void FormMain::StartOfThreadMaster2(ThreadDataMaster^ thDataMaster)
		{
			String^ initialError;
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
					if (Directory::Exists(path))
						isSrcDir = true;
					if (!File::Exists(path) && !Directory::Exists(path))
					{
						initialError = 
							String::Format(I18N(L"The source path '{0}' does not exist."),
								path);
						break;
					}
				}
				if (!String::IsNullOrEmpty(initialError))
					break;

				if (String::IsNullOrEmpty(thDataMaster->Dst))
				{
					initialError = I18N(L"The destination path is empty.");
					break;
				}

				bool bDstIsDir = thDataMaster->SrcPaths->Length > 1 ||
					isSrcDir ||
					thDataMaster->Dst->EndsWith("\\") || thDataMaster->Dst->EndsWith("/");

				List<String^> processedSrcPath;
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

					if (stdIsSamePath(TO_LPCWSTR(srcpath), TO_LPCWSTR(thDataMaster->Dst)))
					{
						initialError = String::Format(
							I18N(L"The source path '{0}' and destination directory '{1}' is same."),
							srcpath, thDataMaster->Dst);
						break;
					}


					if (Directory::Exists(srcpath))
					{
						if (File::Exists(thDataMaster->Dst))
						{
							initialError = I18N(L"Source is directory but destination is a file.");
							break;
						}
						if (stdIsSubDirectory(TO_LPCWSTR(srcpath), TO_LPCWSTR(thDataMaster->Dst)))
						{
							initialError = String::Format(
								I18N(L"The destination directory is a subdirectory of the source path '{0}'."),
								srcpath);
							break;
						}
					} // if (Directory::Exists(srcpath))
					else
					{
						DASSERT(File::Exists(srcpath));
						DASSERT(!stdIsSamePath(TO_LPCWSTR(srcpath), TO_LPCWSTR(thDataMaster->Dst)));
						if (bDstIsDir)
						{
							if (File::Exists(thDataMaster->Dst->TrimEnd((gcnew String(L"/\\"))->ToCharArray())))
							{
								initialError = I18N(L"The destination name ends with path separator but it exists as a file.");
								break;
							}
						}
					}
				}

				if (String::IsNullOrEmpty(initialError) &&
					bDstIsDir && !Directory::Exists(thDataMaster->Dst))
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
						return;
					}

					stdCreateCompleteDirectory(TO_LPCWSTR(thDataMaster->Dst));
					if (!Directory::Exists(thDataMaster->Dst))
					{
						initialError = I18N(L"Failed to create a directory.");
						break;
					}
				}

			} while (false);
			if (!String::IsNullOrEmpty(initialError))
			{
				EndInvokeWithTN(
					thDataMaster->ThreadNumber,
					BeginInvoke(gcnew VISDelegate(this, &FormMain::OnThreadError),
						thDataMaster->ThreadNumber,
						initialError));
				return;
			}

			for each (String ^ path in thDataMaster->SrcPaths)
			{
				List<String^>^ dstDirs;
				KVS^ sds;
				try
				{
					String^ message = I18N(L"Obtaining source files and directories...");
					BeginInvoke(gcnew VSDelegate(this, &FormMain::ThreadLog), message);
					ThreadTransitory::SetProgress(message);
					sds = AmbLib::GetSourceAndDestFiles(path, thDataMaster->Dst, dstDirs);
				}
				catch (Exception^ ex)
				{
					EndInvokeWithTN(
						thDataMaster->ThreadNumber,
						BeginInvoke(gcnew VISDelegate(this, &FormMain::OnThreadError),
							thDataMaster->ThreadNumber,
							ex->Message));
					return;
				}
				thDataMaster->AddTask(gcnew ThreadDataPath(path, sds, dstDirs));
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
				for each (ThreadDataPath ^ thDataPath in thDataMaster->Tasks)
				{
					for each (KV kv in thDataPath->SDS)
					{
						System::IO::FileInfo fi(kv.Key);
						totalInputSize += fi.Length;
						++totalFileCount;
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
							return;
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
				EndInvokeWithTN(
					thDataMaster->ThreadNumber,
					BeginInvoke(gcnew VISDelegate(this, &FormMain::OnThreadError),
						thDataMaster->ThreadNumber,
						ex->Message));
			}

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
					Thread::Sleep(Math::Min(consecutiveErrorCount, 100u));
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