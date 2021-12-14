
#include <Windows.h>
#include "ThreadTransitory.h"
#include "CReadingFile.h"
#include "CDebug.h"
using namespace System;

namespace Ambiesoft {
	namespace retrycopy {
		ref class ThreadDataFile
		{
			initonly int threadNumber_;

			String^ srcFile_;
			IReadingFile^ readingFile_;

			String^ dstFile_;
			HANDLE hDst_;

			LONGLONG allProcessed_;

			bool bTransferStarted_;
			DWORD leRead_ = 0;
			DWORD leWrite_ = 0;
			bool done_ = false;
			int taskNo_ = -1;
			DWORD dwCDForWrite_ = 0;
			bool skipped_ = false;
			LONGLONG zeroWritten_ = 0;
		public:
			ThreadDataFile(int threadNumber, int taskNo, String^ srcFile, String^ dstFile) :
				threadNumber_(threadNumber), taskNo_(taskNo), srcFile_(srcFile), dstFile_(dstFile)
			{
				if (CDebug::IsMockReadFile)
					readingFile_ = gcnew CReadingFileMock();
				else
					readingFile_ = gcnew CReadingFile(srcFile);
			}

			void SetTransferStarted() {
				DASSERT(!bTransferStarted_);
				bTransferStarted_ = true;
			}
			property int ThreadNumber
			{
				int get() { return threadNumber_; }
			}
			void AddZeroWritten(int size) {
				DASSERT(size > 0);
				zeroWritten_ += size;
			}
			void SetSkipped() {
				DASSERT(!skipped_);
				skipped_ = true;
			}
			property bool IsSkipped
			{
				bool get() { return skipped_; }
			}
			property int TaskNo
			{
				int get() { return taskNo_; }
			}
			void setReadError(DWORD le) {
				leRead_ = le;
			}
			void setWriteError(DWORD le) {
				if (leWrite_ == 0)
					leWrite_ = le;
			}
			property String^ SrcFile
			{
				String^ get() { return srcFile_; }
			}
			property String^ DstFile
			{
				String^ get() { return dstFile_; }
			}
			property HANDLE HDst
			{
				HANDLE get();
			}
			void SetDone() {
				done_ = true;
			}
			property bool IsOK
			{
				bool get() {
					return done_ && zeroWritten_ == 0 &&
						leRead_ == 0 && leWrite_ == 0 &&
						SrcSize != -1 && SrcSize == allProcessed_;
				}
			}
			String^ GetNGReason();
			property LONGLONG SrcSize
			{
				LONGLONG get() { return readingFile_->FileSize; }
			}
			property LONGLONG ProcessedSize
			{
				LONGLONG get() { return allProcessed_; }
				void set(LONGLONG v) {
					allProcessed_ = v;
					ThreadTransitory::ProcessedCurSize = v;
				}
			}
			enum class INITSRCRET {
				INITSRC_CANNOTOPEN,
				INITSRC_OK,
				INITSRC_SIZECHANGED,
			};
			bool FirstCheck();
			INITSRCRET InitSrc()
			{
				LONGLONG prevSize = SrcSize;
				readingFile_->Close();

				if (!readingFile_->Open() || SrcSize == -1)
				{
					return INITSRCRET::INITSRC_CANNOTOPEN;
				}

				if (prevSize != SrcSize)
				{
					return INITSRCRET::INITSRC_SIZECHANGED;
				}
				return INITSRCRET::INITSRC_OK;
			}
			property DWORD SrcLE
			{
				DWORD get() {
					return readingFile_->LastError;
				}
			}
			void InitDst()
			{
				if (hDst_ != nullptr && hDst_ != INVALID_HANDLE_VALUE)
				{
					CloseHandle(hDst_);
					hDst_ = nullptr;
				}
			}
			void CloseFiles()
			{
				readingFile_->Close();

				if (hDst_ != nullptr && hDst_ != INVALID_HANDLE_VALUE)
				{
					CloseHandle(hDst_);
					hDst_ = nullptr;
				}
			}
			bool SetReadingPos(const LONGLONG& pos)
			{
				return readingFile_->SetPointer(pos);
			}
			bool ReadFromFile(int buffSize, LPVOID pBuff, DWORD* pdwRead)
			{
				return readingFile_->Read(buffSize, pBuff, pdwRead);
			}
		};
	}
}