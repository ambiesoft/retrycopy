#pragma once
#include "stdafx.h"
#include <Windows.h>
#include "CReadingFile.h"

using namespace System;

namespace Ambiesoft {
	namespace retrycopy {

		bool CReadingFile::Open()
		{
			if (isValid())
				return true;
			DASSERT(!String::IsNullOrEmpty(fileName_));

			h_ = CreateFile(
				TO_LPCWSTR(fileName_),
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL |
				// FILE_FLAG_NO_BUFFERING|
				FILE_FLAG_RANDOM_ACCESS |
				0,
				NULL);
			if (!isValid())
				le_ = GetLastError();

			LARGE_INTEGER li;
			if (GetFileSizeEx(h_, &li))
			{
				fileSize_ = li.QuadPart;
			}

			DASSERT(isValid());
			return true;
		}

		bool CReadingFile::SetPointer(const LONGLONG& pos)
		{
			DASSERT(pos >= 0);
			LARGE_INTEGER li;
			li.QuadPart = pos;
			DASSERT(isValid());
			if (SetFilePointerEx(h_, li, NULL, FILE_BEGIN))
				return true;
			return false;
		}
	}
}