#include "stdafx.h"

#include "helper.h"
#include "FormMain.h"

#pragma comment(lib, "User32.lib")

using namespace System::Text;

using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
namespace retrycopy {

	void FormMain::StartOfThreadMaster(Object^ obj)
	{
		ThreadDataMaster^ thDataMaster = (ThreadDataMaster^)obj;
		EndInvoke(BeginInvoke(gcnew VVDelegate(this, &FormMain::ThreadStarted)));

		try
		{
			// calc total input size
			LONGLONG totalInputSize = 0;
			for each (KV kv in thDataMaster->SDS)
			{
				FileInfo fi(kv.Key);
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
		catch(Exception^){}
		BeginInvoke(gcnew VTmDelegate(this, &FormMain::ThreadFinished), thDataMaster);
	}
	void FormMain::StartOfThreadFile(ThreadDataFile^ thFileData)
	{
		EndInvoke(BeginInvoke(gcnew VTfDelegate(this, &FormMain::ThreadFileStarted),
			thFileData));
		thFileData->ProcessedSize = 0;
		
		// first check
		if (!thFileData->FirstCheck())
			return;
		bool initSrc = true;
		bool ignoreAllFail = false;
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
			bool sfpFailed = (0 == SetFilePointerEx(thFileData->HSrc, li, NULL, FILE_BEGIN));
			
			DWORD dwRead;
			if (sfpFailed || !ReadFile(thFileData->HSrc,
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
				if (!ignoreAllFail)
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
					ReadFailData^ rfd = (ReadFailData^)
						EndInvoke(BeginInvoke(gcnew RDLLLLDwIDelegate(this, &FormMain::ReadFileFailedGetUserAction), 
							thFileData->ProcessedSize,
							thFileData->SrcSize,
							le,
							retried));
					if (rfd->IsCancel)
						return;
					if (rfd->IsRetry)
					{
						retried = 0;
						if (le == ERROR_NOT_READY || le == ERROR_NO_SUCH_DEVICE)
						{
							initSrc = true;
						}
						continue;
					}
					if (rfd->IsIgnoreAll)
						ignoreAllFail = true;
				}
				EndInvoke(BeginInvoke(gcnew VLLIDelegate(this, &FormMain::ProgressWriteWithZero),
					thFileData->ProcessedSize, bufferSize));
				ZeroMemory(bb.get(), bufferSize);
				dwRead = bufferSize;
			}

			consecutiveErrorCount = 0;
			lastError = 0;
			ThreadTransitory::ClearFileLastError();

			if (dwRead == 0)
			{
				thFileData->SetDone();
				return;
			}

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
			// if ((thData->allProcessed_ % 10000) == 0)
			//{
			//	BeginInvoke(gcnew VLLDelegate(this, &FormMain::ProcessProgressed), 
			//		thFileData->ProcessedSize);
			//}
		} while (true);
	}
	//void FormMain::OnUpdateSize()
	//{
	//}


}