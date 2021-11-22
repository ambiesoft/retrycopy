// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#include <Windows.h>
#include <string>
#include <vcclr.h>

delegate void VVDelegate();
delegate void VLLDelegate(LONGLONG);
delegate System::Object^ RDLLDelegate(LONGLONG);
delegate void VDataDelegate(System::Object^);
#define I18N(s) (s)
