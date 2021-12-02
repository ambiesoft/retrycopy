#include "stdafx.h"

#include "helper.h"
#include "FormMain.h"
using namespace System::Collections::Generic;

#pragma comment(lib, "User32.lib")

using namespace System::Text;

using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
namespace retrycopy {

	void FormMain::StartOfThreadMaster(Object^ obj)
	{
		ThreadDataMaster^ thDataMaster = (ThreadDataMaster^)obj;
		EndInvoke(BeginInvoke(gcnew VVDelegate(this, &FormMain::ThreadStarted)));

		StartOfThreadMaster2(thDataMaster);

		EndInvoke(BeginInvoke(gcnew VVDelegate(this, &FormMain::ThreadFinished)));
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
					if (!(bool)EndInvoke(BeginInvoke(
						gcnew BSDelegate(this, &FormMain::OnThreadYesNo),
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
			}
		} while (false);
		if (!String::IsNullOrEmpty(initialError))
		{
			EndInvoke(BeginInvoke(gcnew VSDelegate(this, &FormMain::OnThreadError),
				initialError));
			return;
		}

		{
			List<String^>^ dstDirs;
			KVS^ sds;
			try
			{
				sds = AmbLib::GetSourceAndDestFiles(txtSource->Text, txtDestination->Text, dstDirs);
			}
			catch (Exception^ ex)
			{
				EndInvoke(BeginInvoke(gcnew VSDelegate(this, &FormMain::OnThreadError),
					ex->Message));
				return;
			}
			thDataMaster->SetSDS(sds, dstDirs);
		}

		try
		{
			// prepare target dirs
			thDataMaster->PrepareDstDirs();

			// calc total input size
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
				ThreadDataFile^ tdf = gcnew ThreadDataFile(i + 1,
					thDataMaster->SDS[i].Key, thDataMaster->SDS[i].Value);

				StartOfThreadFile(tdf);

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
				EndInvoke(BeginInvoke(gcnew VTfDelegate(this, &FormMain::ThreadFileEnded),
					tdf));
				thDataMaster->AppendEnded(tdf);
			}
		}
		catch(ThreadAbortException^)
		{ }
		catch(Exception^ ex)
		{
			EndInvoke(BeginInvoke(gcnew VSDelegate(this, &FormMain::OnThreadError),
				ex->Message));
		}
		EndInvoke(BeginInvoke(gcnew VTmDelegate(this, &FormMain::ThreadTaskFinished), thDataMaster));
	}
	void FormMain::StartOfThreadFile(ThreadDataFile^ thFileData)
	{
		EndInvoke(BeginInvoke(gcnew VTfDelegate(this, &FormMain::ThreadFileStarted),
			thFileData));
		thFileData->ProcessedSize = 0;
		
		// first check
		if (!thFileData->FirstCheck())
		{
			thFileData->SetSkipped();
			return;
		}
		bool initSrc = true;
		// bool ignoreAllFail = false;
		int retried = 0;
		int bufferSize = 0;
		std::unique_ptr<BYTE[]> bb;
		unsigned consecutiveErrorCount = 0;
		unsigned sameErrorCount = 0;
		DWORD lastError = 0;
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
					if ((bool)EndInvoke(BeginInvoke(
						gcnew BDwDelegate(this, &FormMain::OpenFileFailedGetUserAction),
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
				const DWORD le = GetLastError();
				
				if (ThreadTransitory::UserRetry < 0 ||
					retried++ < ThreadTransitory::UserRetry)
				{
					// Do retry
					ThreadTransitory::SetFileLastError(
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
					EndInvoke(BeginInvoke(gcnew RSDLLLLDwIDelegate(this, &FormMain::SFPFailedGetUserAction),
						thFileData->SrcFile,
						thFileData->ProcessedSize,
						thFileData->SrcSize,
						le,
						retried));
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
			
			DWORD dwRead;
			if (!ReadFile(thFileData->HSrc,
				bb.get(),
				bufferSize,
				&dwRead,
				NULL))
			{
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
				//if (!ignoreAllFail)
				{
					if (ThreadTransitory::UserRetry < 0 ||
						retried++ < ThreadTransitory::UserRetry)
					{
						ThreadTransitory::SetFileLastError(
							thFileData->ProcessedSize,
							thFileData->SrcSize,
							le,
							retried);
						if (le == ERROR_NOT_READY || le == ERROR_NO_SUCH_DEVICE)
						{
							initSrc = true;
						}
						continue;
					}
					// fail
					UserResponceOfFail^ rfd = (UserResponceOfFail^)
						EndInvoke(BeginInvoke(gcnew RSDLLLLDwIIDelegate(this, &FormMain::ReadFileFailedGetUserAction),
							thFileData->SrcFile,
							thFileData->ProcessedSize,
							thFileData->SrcSize,
							le,
							retried,
							bufferSize));
					if (rfd->IsCancel)
						return;

					// Change bufferSize to user's specified size
					DASSERT(ThreadTransitory::UserBuffer == rfd->BufferSize);
					ThreadTransitory::UserBuffer = rfd->BufferSize;

					if (rfd->IsRetry)
					{
						retried = 0;
						if (le == ERROR_NOT_READY || le == ERROR_NO_SUCH_DEVICE)
						{
							initSrc = true;
						}
						continue;
					}

					//if (rfd->IsIgnoreAll)
					//	ignoreAllFail = true;

					if (bufferSize != ThreadTransitory::UserBuffer)
					{
						bufferSize = ThreadTransitory::UserBuffer;
						DASSERT(bufferSize > 0);
						bb.reset(new BYTE[bufferSize]);
					}
				}
				EndInvoke(BeginInvoke(gcnew VLLIDelegate(this, &FormMain::ProgressWriteWithZero),
					thFileData->ProcessedSize, bufferSize));
				ZeroMemory(bb.get(), bufferSize);
				dwRead = bufferSize;
			}

			consecutiveErrorCount = 0;
			lastError = 0;
			ThreadTransitory::ClearFileLastError();

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
		} while (true);
	}
}