#include "stdafx.h"

#include "helper.h"

using namespace System;
using namespace System::Text;
using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
namespace retrycopy {

	String^ SizeToUser(LONGLONG size)
	{
		return String::Format(I18N(L"{0} ({1})"),
			size,
			AmbLib::FormatSize(size));
	}

	bool IsGiveUpError(DWORD le, bool bFirstTry)
	{
		if (bFirstTry)
		{
			if (le == ERROR_ACCESS_DENIED ||
				le == ERROR_FILE_NOT_FOUND)
			{
				return true;
			}

			if (le == ERROR_NOT_READY ||
				le == ERROR_NO_SUCH_DEVICE)
			{
				return true;
			}
		}
		else
		{
			if (le == ERROR_ACCESS_DENIED ||
				le == ERROR_FILE_NOT_FOUND)
			{
				return true;
			}
		}
	}
} // namespace