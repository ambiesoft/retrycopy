#pragma once

#include "stdafx.h"

#include "FormMain.h"
#include "threadData.h"

using namespace System::IO;
using namespace Ambiesoft;
namespace retrycopy {

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
		DWORD dwCD = 0;
		switch (ThreadTransitory::UserOverWrite)
		{
		case OVERWRITE_TYPE::OVERWRITE_YES:
			dwCD = OPEN_ALWAYS;
			break;
		case OVERWRITE_TYPE::OVERWRITE_NO:
			dwCD = CREATE_ALWAYS;
			break;
		case OVERWRITE_TYPE::OVERWRITE_ASK:
			if (!File::Exists(dstFile_))
			{
				dwCD = CREATE_ALWAYS;
				break;
			}
			if ((bool)FormMain::theForm_->EndInvoke(FormMain::theForm_->BeginInvoke(
				gcnew BSDelegate(FormMain::theForm_, &FormMain::AskOverwrite), dstFile_)))
			{
				dwCD = OPEN_ALWAYS;
			}
			else
			{
				return nullptr;
			}
			break;
		}
		hDst_ = CreateFile(
			TO_LPCWSTR(dstFile_),
			GENERIC_WRITE,
			0,
			NULL,
			dwCD,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		return hDst_;
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