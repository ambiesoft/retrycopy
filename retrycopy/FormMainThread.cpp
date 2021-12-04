#include "stdafx.h"

#include "helper.h"
#include "threadData.h"
#include "FormMain.h"
using namespace System::Collections::Generic;

#pragma comment(lib, "User32.lib")

using namespace System::Text;

using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

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

		EndInvokeWithTN(
			thDataMaster->ThreadNumber,
			BeginInvoke(gcnew VIDelegate(this, &FormMain::ThreadFinished),
			thDataMaster->ThreadNumber));
	}
	void FormMain::StartOfThreadMaster2(ThreadDataMaster^ thDataMaster)
	{
		String^ initialError;
		do
		{
			if (String::IsNullOrEmpty(thDataMaster->Src))
			{
				initialError = I18N(L"The source is empty.");
				break;
			}
			if (!File::Exists(thDataMaster->Src) && !Directory::Exists(thDataMaster->Src))
			{
				initialError = I18N(L"The source path does not exist.");
				break;
			}

			if (String::IsNullOrEmpty(thDataMaster->Dst))
			{
				initialError = I18N(L"The destination path is empty.");
				break;
			}

			if (Directory::Exists(thDataMaster->Src))
			{
				if (File::Exists(thDataMaster->Dst))
				{
					initialError = I18N(L"Source is directory but destination is a file.");
					break;
				}
				if (stdIsSamePath(TO_LPCWSTR(thDataMaster->Src), TO_LPCWSTR(thDataMaster->Dst)))
				{
					initialError = I18N(L"The destination directory is a subdirectory of the source directory.");
					break;
				}
				if (stdIsSamePath(TO_LPCWSTR(thDataMaster->Src), TO_LPCWSTR(thDataMaster->Dst)))
				{
					initialError = I18N(L"The destination directory is the same as the source directory.");
					break;
				}
				if (!Directory::Exists(thDataMaster->Dst))
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

					Directory::CreateDirectory(thDataMaster->Dst);
					if (!Directory::Exists(thDataMaster->Dst))
					{
						initialError = I18N(L"Failed to create a directory.");
						break;
					}
				}
			}
			else
			{
				DASSERT(File::Exists(thDataMaster->Src));
				if (stdIsSamePath(TO_LPCWSTR(thDataMaster->Src), TO_LPCWSTR(thDataMaster->Dst)))
				{
					initialError = I18N(L"The destination file is the same as the source file.");
					break;
				}
				if (thDataMaster->Dst->EndsWith("\\") || thDataMaster->Dst->EndsWith("/"))
				{
					if (File::Exists(thDataMaster->Dst->TrimEnd((gcnew String( L"/\\"))->ToCharArray())))
					{
						initialError = I18N(L"The destination name ends with path separator but it exists as a file.");
						break;
					}
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

		{
			List<String^>^ dstDirs;
			KVS^ sds;
			try
			{
				String^ message = I18N(L"Obtaining source files and directories...");
				BeginInvoke(gcnew VSDelegate(this, &FormMain::ThreadLog), message);
				ThreadTransitory::SetProgress(message);
				sds = AmbLib::GetSourceAndDestFiles(txtSource->Text, txtDestination->Text, dstDirs);
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
			thDataMaster->SetSDS(sds, dstDirs);
		}

		try
		{
			// prepare target dirs
			String^ message = I18N(L"Preparing target directories...");
			BeginInvoke(gcnew VSDelegate(this, &FormMain::ThreadLog), message);
			ThreadTransitory::SetProgress(message);
			thDataMaster->PrepareDstDirs();
			
			// calc total input size
			message = I18N(L"Calculating total size...");
			BeginInvoke(gcnew VSDelegate(this, &FormMain::ThreadLog), message);
			ThreadTransitory::SetProgress(message);
			LONGLONG totalInputSize = 0;
			for each (KV kv in thDataMaster->SDS)
			{
				System::IO::FileInfo fi(kv.Key);
				totalInputSize += fi.Length;
			}
			
			thDataMaster->SetTotalInputSize(totalInputSize);
			ThreadTransitory::TotalCount = thDataMaster->SDS->Count;
			for (int i = 0; i < thDataMaster->SDS->Count; ++i)
			{
				ThreadTransitory::ProcessedTotalCount = i + 1;
				ThreadDataFile^ tdf = gcnew ThreadDataFile(
					thDataMaster->ThreadNumber, i + 1,
					thDataMaster->SDS[i].Key, thDataMaster->SDS[i].Value);

				StartOfThreadFile(tdf);
				if (thDataMaster->ThreadNumber != ThreadTransitory::ThreadNumber)
					return;
				thDataMaster->TotalProcessedSize += tdf->ProcessedSize;
				tdf->CloseFiles();
				if (tdf->IsSkipped)
				{
					DASSERT(!tdf->IsOK);
					thDataMaster->IncrementSkipCount();
				}
				if (tdf->IsOK)
				{
					thDataMaster->IncrementOK();
					AmbLib::CopyFileTime(tdf->SrcFile, tdf->DstFile,
						AmbLib::CFT::Creation);
				}
				EndInvokeWithTN(
					tdf->ThreadNumber,
					BeginInvoke(gcnew VTfDelegate(this, &FormMain::ThreadFileEnded),
					tdf));
				thDataMaster->AppendEnded(tdf);
			}
		}
		catch(ThreadAbortException^){}
		catch(Exception^ ex)
		{
			EndInvokeWithTN(
				thDataMaster->ThreadNumber,
				BeginInvoke(gcnew VISDelegate(this, &FormMain::OnThreadError),
				thDataMaster->ThreadNumber,
				ex->Message));
		}
		EndInvokeWithTN(
			thDataMaster->ThreadNumber,
			BeginInvoke(gcnew VTmDelegate(this, &FormMain::ThreadTaskFinished), thDataMaster));
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
			LARGE_INTEGER li;
			li.QuadPart = thFileData->ProcessedSize;
			if (!SetFilePointerEx(thFileData->HSrc, li, NULL, FILE_BEGIN))
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
			if (!ReadFile(thFileData->HSrc,
				bb.get(),
				bufferSize,
				&dwRead,
				NULL))
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