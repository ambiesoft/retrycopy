#pragma once
#include <Windows.h>

using namespace System;

namespace Ambiesoft {
	namespace retrycopy {

		interface class IReadingFile
		{
		public:
			bool isValid();
			property LONGLONG FileSize
			{
				LONGLONG get();
			}
			property DWORD LastError
			{
				DWORD get();
			}
			bool Open();
			bool SetPointer(const LONGLONG& pos);
			bool Read(int buffSize, LPVOID pBuff, DWORD* pdwRead);
			void Close();
		};

		ref class CReadingFileMock : IReadingFile
		{
		public:
			virtual bool isValid() {
				return true;
			}
			property LONGLONG FileSize
			{
				virtual LONGLONG get() { return 100; }
			}
			property DWORD LastError
			{
				virtual DWORD get() { return 0; }
			}
			virtual bool Open() { return true; }
			virtual bool SetPointer(const LONGLONG& pos) { return true; }
			virtual bool Read(int buffSize, LPVOID pBuff, DWORD* pdwRead) {
				return true;
			}
			virtual void Close(){}
		};
		ref class CReadingFile : IReadingFile
		{
			String^ fileName_;
			HANDLE h_;
			DWORD le_;
			LONGLONG fileSize_ = -1;
		public:
			CReadingFile(String^ fileName) :
				fileName_(fileName)
			{

			}
			~CReadingFile() {
				Close();
			}
			!CReadingFile() {
				DASSERT(!isValid());
			}
			virtual bool isValid() {
				return h_ != nullptr && h_ != INVALID_HANDLE_VALUE;
			}
			property DWORD LastError
			{
				virtual DWORD get() { return le_; }
			}
			property LONGLONG FileSize
			{
				virtual LONGLONG get() { return fileSize_; }
			}
			virtual bool Open();
			virtual void Close() {
				if (isValid())
					CloseHandle(h_);
				h_ = nullptr;
			}
			virtual bool SetPointer(const LONGLONG& pos);
			virtual bool Read(int buffSize, LPVOID pBuff, DWORD* pdwRead)
			{
				DASSERT(isValid());
				return ReadFile(h_,
					pBuff,
					buffSize,
					pdwRead,
					NULL);
			}
		};

	}
}