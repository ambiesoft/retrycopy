#pragma once

#include <Windows.h>

namespace retrycopy {

	ref class ThreadData
	{
		HANDLE hSrc_;
		HANDLE hDst_;
		LONGLONG srcSize_;
		bool ok_;
		
	public:
		ThreadData(HANDLE hSrc, HANDLE hDst, LONGLONG srcSize) :
			hSrc_(hSrc), hDst_(hDst), srcSize_(srcSize) {}

		property HANDLE HSrc
		{
			HANDLE get() { return hSrc_; }
		}
		property HANDLE HDst
		{
			HANDLE get() { return hDst_; }
		}

		void SetOK() {
			ok_ = true;
		}
		property bool IsOK
		{
			bool get() { return ok_; }
		}
		property LONGLONG SrcSize
		{
			LONGLONG get() { return srcSize_; }
		}
		property LONGLONG WrittenSize
		{
			LONGLONG get() { return allProcessed_; }
		}
		LONGLONG allProcessed_;
	};
}