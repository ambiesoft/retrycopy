#pragma once

#include "stdafx.h"

#include "FormMain.h"
#include "helper.h"
#include "ThreadDataFile.h"


using namespace System::IO;
namespace Ambiesoft {
	namespace retrycopy {

		bool ThreadDataFile::FirstCheck()
		{
			DASSERT(!readingFile_->isValid());
			if (!readingFile_->Open() || readingFile_->FileSize==-1)
			{
				setReadError(readingFile_->LastError);
				return false;
			}

			readingFile_->Close();

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

	}
}