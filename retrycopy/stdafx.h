// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#include <Windows.h>
#include <string>
#include <vcclr.h>
#include <memory>
#include <algorithm>

#include "../../lsMisc/DebugMacro.h"
#include "../../lsMisc/GetLastErrorString.h"
#include "../../lsMisc/WaitCursor.h"
#include "../../lsMisc/SHMoveFile.h"
#include "../../lsMisc/stdosd/stdosd.h"
#include "../../lsMisc/BrowseFolder.h"
#include "../../lsMisc/CommandLineParser.h"
#include "../../lsMisc/HighDPI.h"
#include "../../lsMisc/stdosd/stdosd.h"
#include "../../lsMisc/CommandLineString.h"

delegate void VSDelegate(System::String^);
delegate void VIDelegate(int);
delegate void VISDelegate(int, System::String^);
delegate bool BISDelegate(int, System::String^);
delegate void VLLDelegate(LONGLONG);
delegate void VILLIDelegate(int, LONGLONG, int);
delegate void VLLLLDwIDelegate(LONGLONG,LONGLONG,DWORD,int);
delegate bool BIDwDelegate(int, DWORD);

#define I18N(q) Ambiesoft::retrycopy::ResUtil::getString(q)

using KV = System::Collections::Generic::KeyValuePair<System::String^, System::String^>;
using KVS = System::Collections::Generic::List<KV>;
#define TO_LPCWSTR(s) (getStdWstring(s).c_str())
inline std::wstring getStdWstring(System::String^ s)
{
	std::wstring ret;
	if (s == nullptr)
		return ret;

	pin_ptr<const wchar_t> p = PtrToStringChars(s);
	ret = p;
	return ret;
}
public enum class READERROR_RESPONSE {
	RR_NONE,
	RR_RETRY,
	RR_WRITEZERO,
	RR_WZOMODE,
	RR_WZOMODEALL,
	RR_CANCEL,
};
#define MINREADBUFFERSIZE 1
#define DEFAULTREADBUFFERSIZE 512
#define MAXREADBUFFERSIZE (10 * 1024 * 1024)

#define MINRETRYCOUNT -1
#define MAXRETRYCOUNT 65535

#ifndef ERROR_NO_SUCH_DEVICE
#define ERROR_NO_SUCH_DEVICE 433L
#endif
static_assert(ERROR_NO_SUCH_DEVICE == 433L, "");

// Thread::Suspend and Resume is obsolete
#pragma warning(disable:4947)

#include "ResUtil.h"

using namespace System;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
