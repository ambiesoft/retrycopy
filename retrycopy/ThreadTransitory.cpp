#pragma once

#include "stdafx.h"

#include "FormMain.h"
#include "helper.h"
#include "ThreadTransitory.h"

using namespace System::IO;
namespace Ambiesoft {
	namespace retrycopy {

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