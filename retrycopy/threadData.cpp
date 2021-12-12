#pragma once

#include "stdafx.h"

#include "FormMain.h"
#include "helper.h"
#include "threadData.h"

using namespace System::IO;
namespace Ambiesoft {
	namespace retrycopy {

		ThreadDataMaster::ThreadDataMaster(int threadNumber, String^ srces, String^ dst) :
			threadNumber_(threadNumber), dst_(dst)
		{
			srcPaths_ = S2A(srces);
		}


		int ThreadDataMaster::TotalOKCount::get()
		{
			int ret = 0;
			for each (ThreadDataPath ^ thPath in Tasks) {
				ret += thPath->ToTatalOKCount;
			}
			return ret;
		}

		int ThreadDataMaster::TotalSkipCount::get()
		{
			int ret = 0;
			for each (ThreadDataPath ^ thPath in Tasks) {
				ret += thPath->ToTatalSkipCount;
			}
			return ret;
		}

		void ThreadDataPath::PrepareDstDirs()
		{
			for each (String ^ dir in dstdirs_)
			{
				try
				{
					Directory::CreateDirectory(dir);
				}
				catch (Exception^) {}
			}
		}
		bool ThreadDataFile::FirstCheck()
		{
			DASSERT(hSrc_ == nullptr);
			HANDLE hTmp = HSrc;
			if (hTmp == nullptr || hTmp == INVALID_HANDLE_VALUE ||
				SrcSize == -1)
			{
				setReadError(srcLE_);
				return false;
			}
			CloseHandle(hTmp);
			srcLE_ = 0;
			hSrc_ = nullptr;
			srcSize_ = -1;

			if (!File::Exists(dstFile_))
			{
				dwCDForWrite_ = CREATE_ALWAYS;
			}
			else
			{
				// Exist
				switch (ThreadTransitory::UserOverWrite)
				{
				case OVERWRITE_TYPE::OVERWRITE_YES:
					dwCDForWrite_ = OPEN_ALWAYS;
					break;
				case OVERWRITE_TYPE::OVERWRITE_NO:
					setWriteError(ERROR_FILE_EXISTS);
					return false;
				case OVERWRITE_TYPE::OVERWRITE_ASK:
					if ((bool)FormMain::theForm_->EndInvokeWithTN(
						ThreadNumber,
						FormMain::theForm_->BeginInvoke(
							gcnew BISDelegate(FormMain::theForm_, &FormMain::AskOverwrite),
							ThreadNumber,
							dstFile_)))
					{
						dwCDForWrite_ = OPEN_ALWAYS;
					}
					else
					{
						setWriteError(ERROR_FILE_EXISTS);
						return false;
					}
					break;
				default:
					DASSERT(false);
					return false;
				}
			}
			return true;
		}

		HANDLE ThreadDataFile::HSrc::get()
		{
			if (hSrc_ != nullptr && hSrc_ != INVALID_HANDLE_VALUE)
				return hSrc_;
			hSrc_ = CreateFile(
				TO_LPCWSTR(srcFile_),
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL |
				// FILE_FLAG_NO_BUFFERING|
				FILE_FLAG_RANDOM_ACCESS |
				0,
				NULL);
			if (!(hSrc_ != nullptr && hSrc_ != INVALID_HANDLE_VALUE))
				srcLE_ = GetLastError();

			LARGE_INTEGER li;
			if (GetFileSizeEx(hSrc_, &li))
			{
				SrcSize = li.QuadPart;
			}

			return hSrc_;
		}

		HANDLE ThreadDataFile::HDst::get()
		{
			if (hDst_ != nullptr && hDst_ != INVALID_HANDLE_VALUE)
				return hDst_;

			DASSERT(dwCDForWrite_ != 0);
			for (int i = 0; i < 2; ++i)
			{
				hDst_ = CreateFile(
					TO_LPCWSTR(dstFile_),
					GENERIC_WRITE,
					0,
					NULL,
					dwCDForWrite_,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
				if (hDst_ == INVALID_HANDLE_VALUE && GetLastError() == ERROR_PATH_NOT_FOUND)
				{
					EnsureDirectory(dstFile_);
					continue;
				}
				else
					break;
			}
			return hDst_;
		}
		String^ ThreadDataFile::GetNGReason() {
			if (IsOK)
				return String::Empty;
			System::Collections::Generic::List<String^> lst;
			if (!done_)
				lst.Add(I18N(L"Not Done"));
			if (zeroWritten_ != 0)
				lst.Add(String::Format(I18N(L"{0} bytes of zero was written"),
					zeroWritten_));
			if (leRead_ != 0)
				lst.Add(gcnew String(GetLastErrorString(leRead_).c_str()));
			if (leWrite_ != 0)
				lst.Add(gcnew String(GetLastErrorString(leWrite_).c_str()));
			if (SrcSize == -1)
				lst.Add(I18N(L"Source size is not obtained"));
			if (SrcSize != allProcessed_)
			{
				lst.Add(String::Format(I18N(L"Source size not equal to written size {0} != {1}"),
					SrcSize, allProcessed_));
			}
			return String::Join(L",", % lst);
		}

		// static
		void ThreadTransitory::SetLastErrorDisp(LONGLONG pos, LONGLONG allSize, DWORD le, int retried)
		{
			lastErrorDisp_ = (
				String::Format(I18N(L"Failed to ReadFile at {1} ({2}) {0} times"),
					retried,
					pos,
					gcnew String(GetLastErrorString(le).c_str())));
		}
		void ThreadTransitory::SetReadingProgress(LONGLONG pos, LONGLONG allSize, int retried)
		{
			progressDisp_ =
				String::Format(I18N(L"Reading at {0} with size {1} {2} times..."),
					pos, allSize, retried + 1);
		}
		void ThreadTransitory::SetWrittingProgress(LONGLONG pos, LONGLONG allSize)
		{
			progressDisp_ =
				String::Format(I18N(L"Writting at {0} with size {1}..."),
					pos, allSize);
		}

	}
}