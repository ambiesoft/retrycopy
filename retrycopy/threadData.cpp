#pragma once

#include "stdafx.h"

#include "FormMain.h"
#include "helper.h"
#include "threadData.h"

using namespace System::IO;
using namespace Ambiesoft;
namespace retrycopy {

	void ThreadDataMaster::PrepareDstDirs()
	{
		for each (String ^ dir in dstdirs_)
		{
			try
			{
				Directory::CreateDirectory(dir);
			}
			catch(Exception^){}
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
				if ((bool)FormMain::theForm_->EndInvoke(FormMain::theForm_->BeginInvoke(
					gcnew BSDelegate(FormMain::theForm_, &FormMain::AskOverwrite), dstFile_)))
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
			lst.Add(L"Not Done");
		if (leRead_ != 0)
			lst.Add(gcnew String(GetLastErrorString(leRead_).c_str()));
		if (leWrite_ != 0)
			lst.Add(gcnew String(GetLastErrorString(leWrite_).c_str()));
		if (SrcSize == -1)
			lst.Add(L"Source size is not obtained");
		if (SrcSize != allProcessed_)
		{
			lst.Add(String::Format(I18N(L"Source size not equal to written size {0} != {1}"),
				SrcSize, allProcessed_));
		}
		return String::Join(L",", % lst);
	}
	//// static
	//void ThreadTransitory::OnUpdate()
	//{
	//	FormMain::theForm_->BeginInvoke(
	//		gcnew VVDelegate(FormMain::theForm_, &FormMain::OnUpdateSize));
	//}
	
	// static
	void ThreadTransitory::SetFileLastError(LONGLONG pos, LONGLONG allSize, DWORD le, int retried) 
	{
		fileLastError_ = (
			String::Format(I18N(L"{0} Failed to ReadFile at {1} ({2})"),
				retried,
				pos,
				gcnew String(GetLastErrorString(le).c_str())));
	}

}