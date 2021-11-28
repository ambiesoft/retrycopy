// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#include <Windows.h>
#include <string>
#include <vcclr.h>
#include <memory>

#include "../../lsMisc/DebugMacro.h"
#include "../../lsMisc/GetLastErrorString.h"
#include "../../lsMisc/WaitCursor.h"

delegate void VVDelegate();
delegate bool BSDelegate(System::String^);
delegate void VLLDelegate(LONGLONG);
delegate void VLLIDelegate(LONGLONG, int);
delegate void VLLLLDwIDelegate(LONGLONG,LONGLONG,DWORD,int);
delegate System::Object^ RDLLLLDwIDelegate(LONGLONG,LONGLONG,DWORD,int);
delegate bool BDwDelegate(DWORD);

#define I18N(s) (s)
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

// Thread::Suspend and Resume is obsolete
#pragma warning(disable:4947)