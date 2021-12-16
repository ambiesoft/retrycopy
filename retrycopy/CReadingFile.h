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
			// 100Mb file = 100 * 1024 * 1024;
			// 0-100k=OK (100*1024)
			// (100*1024) + 1024 => bad
			// bat-end = OK
			// all good part will filled with 0xFF
			static initonly LONGLONG FILESIZE = 10 * 1024 * 1024;
			static initonly LONGLONG BADSTARTPOS = (100 * 1024);
			static initonly int BADSIZE = 4096;
			enum class WHERE{
				NONE,
				TOPGOOD,
				BAD,
				BOTTOMGOOD
			};
			static WHERE whereis(LONGLONG pos)
			{
				if (0 <= pos && pos < BADSTARTPOS)
					return WHERE::TOPGOOD;
				if (BADSTARTPOS <= pos && pos < (BADSTARTPOS + BADSIZE))
					return WHERE::BAD;
				if ((BADSTARTPOS + BADSIZE) < pos && pos <= FILESIZE)
					return WHERE::BOTTOMGOOD;
				
				return WHERE::BOTTOMGOOD;
			}
			static bool isGoodPos(LONGLONG pos, int buffSize) {
				DASSERT(0 <= pos);
				DASSERT(pos <= FILESIZE);
				WHERE w1 = whereis(pos);
				WHERE w2 = whereis(pos + buffSize - 1);
				//if (0 <= pos && pos <= BADSTARTPOS)
				//{
				//	if (endPos < BADSTARTPOS)
				//		return true;
				//	return false;
				//}
				//if (BADSTARTPOS <= pos && pos < (BADSTARTPOS + BADSIZE - 1))
				//	return false;
				//if ((BADSTARTPOS + BADSIZE) < pos && pos <= FILESIZE )
				//	return true;
				//
				//return true;
				return w1 == w2 && w1 != WHERE::BAD;
			}
		private:
			LONGLONG pos_ = -1;
			bool bOpened_ = false;
		public:
			virtual bool isValid() {
				return bOpened_;
			}
			property LONGLONG FileSize
			{
				virtual LONGLONG get() { return FILESIZE; }
			}
			property DWORD LastError
			{
				virtual DWORD get() { return 0; }
			}
			virtual bool Open() { 
				bOpened_ = true;
				return true; 
			}
			virtual bool SetPointer(const LONGLONG& pos) {
				DASSERT(0 <= pos && pos <= FILESIZE);
				pos_ = pos;
				return true;
			}
			virtual bool Read(int buffSize, LPVOID pBuff, DWORD* pdwRead) {
				if (!isGoodPos(pos_, buffSize))
				{
					buffSize = 1;
					if (!isGoodPos(pos_, 1))
					{
						SetLastError(3);
						return false;
					}
				}
				DASSERT(FILESIZE - pos_ >= 0);
				int writeSize = buffSize;
				if (FILESIZE - pos_ < buffSize)
					writeSize = (int)(FILESIZE - pos_);

				memset(pBuff, 0xFF, writeSize);
				*pdwRead = writeSize;
				pos_ += writeSize;
				return true;
			}
			virtual void Close(){
				bOpened_ = false;
			}
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