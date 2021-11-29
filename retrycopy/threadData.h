#pragma once

#include <Windows.h>

using namespace System;

namespace retrycopy {
	ref class ThreadDataFile;
	enum class OVERWRITE_TYPE
	{
		OVERWRITE_YES,
		OVERWRITE_NO,
		OVERWRITE_ASK,
	};
	enum class REMOVE_TYPE
	{
		REMOVE_NO,
		REMOVE_YES_RECYCLE,
		REMOVE_YES_DELETE,
		REMOVE_ASK,
	};
	public ref class ThreadTransitory abstract sealed
	{
		static System::Threading::ReaderWriterLock^ rwl_ = gcnew System::Threading::ReaderWriterLock();

		static LONGLONG totalSize_;
		static LONGLONG processedTotalSize_;

		static int totalCount_;
		static int processedCount_;

		static LONGLONG curSize_;
		static LONGLONG curProcessedSize_;

		static OVERWRITE_TYPE userOverwrite_;
		static REMOVE_TYPE userRemove_;

		static int userBuffer_ = 40960;
		static int userRetry_ = 10;

		static String^ fileLastError_ = String::Empty;

		ref struct RLocker
		{
			System::Threading::ReaderWriterLock^ rwl_;
			RLocker(System::Threading::ReaderWriterLock^ rwl) :rwl_(rwl) {
				rwl->AcquireReaderLock(-1);
			}
			~RLocker() {
				rwl_->ReleaseReaderLock();
			}
		};
		ref struct WLocker
		{
			System::Threading::ReaderWriterLock^ rwl_;
			WLocker(System::Threading::ReaderWriterLock^ rwl) :rwl_(rwl) {
				rwl->AcquireWriterLock(-1);
			}
			~WLocker() {
				rwl_->ReleaseWriterLock();
			}
		};
		static void OnUpdate() {}

	public:
		static void init() {
			totalSize_ = 0;
			processedTotalSize_ = 0;

			totalCount_ = 0;
			processedCount_ = 0;

			curSize_ = 0;
			curProcessedSize_ = 0;

			fileLastError_ = String::Empty;
		}
		static property LONGLONG TotalSize
		{
			LONGLONG get() {
				RLocker r(rwl_);
				return totalSize_;
			}
			void set(LONGLONG v) {
				WLocker w(rwl_);
				totalSize_ = v;
				OnUpdate();
			}
		}
		static property LONGLONG ProcessedTotalSize
		{
			LONGLONG get() {
				RLocker r(rwl_);
				return processedTotalSize_;
			}
			void set(LONGLONG v) {
				WLocker w(rwl_);
				processedTotalSize_ = v;
				OnUpdate();
			}
		}

		static property int TotalCount
		{
			int get() {
				RLocker r(rwl_);
				return totalCount_;
			}
			void set(int v) {
				WLocker w(rwl_);
				totalCount_ = v;
				OnUpdate();
			}
		}
		static property int TotalPercent
		{
			int get() {
				if (TotalSize == 0)
					return 0;
				return (int)((double)ProcessedTotalSize*100 / (double)TotalSize);
			}
		}
		static property int ProcessedTotalCount
		{
			int get() {
				RLocker r(rwl_);
				return processedCount_;
			}
			void set(int v) {
				WLocker w(rwl_);
				processedCount_ = v;
				OnUpdate();
			}
		}

		static property LONGLONG CurSize
		{
			LONGLONG get() {
				RLocker r(rwl_);
				return curSize_;
			}
			void set(LONGLONG v) {
				WLocker w(rwl_);
				curSize_ = v;
				OnUpdate();
			}
		}
		static property LONGLONG ProcessedCurSize
		{
			LONGLONG get() {
				RLocker r(rwl_);
				return curProcessedSize_;
			}
			void set(LONGLONG v) {
				WLocker w(rwl_);
				curProcessedSize_ = v;
				OnUpdate();
			}
		}
		static property int UserBuffer
		{
			int get() {
				RLocker r(rwl_);
				return userBuffer_ < 0 ? 1 : userBuffer_;
			}
			void set(int v) {
				WLocker w(rwl_);
				userBuffer_ = v;
			}
		}
		static property int UserRetry
		{
			int get() {
				RLocker r(rwl_);
				return userRetry_;
			}
			void set(int v) {
				WLocker w(rwl_);
				userRetry_ = v;
			}
		}
		static property OVERWRITE_TYPE UserOverWrite
		{
			OVERWRITE_TYPE get() {
				RLocker r(rwl_);
				return userOverwrite_;
			}
			void set(OVERWRITE_TYPE v) {
				WLocker w(rwl_);
				userOverwrite_ = v;
			}
		}
		static property REMOVE_TYPE UserRemove
		{
			REMOVE_TYPE get() {
				RLocker r(rwl_);
				return userRemove_;
			}
			void set(REMOVE_TYPE v) {
				WLocker w(rwl_);
				userRemove_ = v;
			}
		}
		static void SetFileLastError(LONGLONG pos, LONGLONG allSize, DWORD le, int retried);
		static void ClearFileLastError() {
			fileLastError_ = String::Empty;
		}
		static property String^ FileLastError
		{
			String^ get() { return fileLastError_; }
		}
	};

	ref class ThreadDataMaster
	{
		initonly KVS^ sds_;

		LONGLONG totalInputSize_;
		LONGLONG totalProcessed_;
		int totalOK_;

	public:
		ThreadDataMaster(KVS^ sds) : sds_(sds)
		{}

		property KVS^ SDS
		{
			KVS^ get() { return sds_; }
		}
		void IncrementOK() {
			++totalOK_;
		}

		property bool IsOK
		{
			bool get() { return totalOK_==sds_->Count; }
		}
		property int TotalOKCount
		{
			int get() { return totalOK_; }
		}
		property LONGLONG TotalWrittenSize
		{
			LONGLONG get() { return totalProcessed_; }
		}
		
		void SetTotalInputSize(LONGLONG size) {
			totalInputSize_ = size;
			ThreadTransitory::TotalSize = size;
		}
		property LONGLONG TotalInputSize
		{
			LONGLONG get() { return totalInputSize_; }
		}
		
		property LONGLONG TotalProcessedSize
		{
			LONGLONG get() { return totalProcessed_; }
			void set(LONGLONG v) {
				totalProcessed_ = v;
				ThreadTransitory::ProcessedTotalSize = v;
			}
		}

		System::Collections::Generic::List<ThreadDataFile^> ended_;
		void AppendEnded(ThreadDataFile^ tdf) {
			ended_.Add(tdf);
		}
	};

	ref class ThreadDataFile
	{
		String^ srcFile_;
		HANDLE hSrc_;
		DWORD srcLE_;

		String^ dstFile_;
		HANDLE hDst_;

		LONGLONG srcSize_;
		LONGLONG allProcessed_;

		DWORD leRead_ = 0;
		DWORD leWrite_ = 0;
		bool done_ = false;
		int taskNo_ = -1;
		DWORD dwCDForWrite_ = 0;
	public:
		ThreadDataFile(int taskNo, String^ srcFile, String^ dstFile):
			taskNo_(taskNo), srcFile_(srcFile), dstFile_(dstFile){}

		property int TaskNo
		{
			int get() { return taskNo_; }
		}
		void setReadError(DWORD le) {
			leRead_ = le;
		}
		void setWriteError(DWORD le) {
			leWrite_ = le;
		}
		property String^ SrcFile
		{
			String^ get() { return srcFile_; }
		}
		property HANDLE HSrc
		{
			HANDLE get();
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
				return done_ &&
					leRead_ == 0 && leWrite_ == 0 &&
					SrcSize != 0 && SrcSize == allProcessed_;
			}
		}
		String^ GetNGReason();
		property LONGLONG SrcSize
		{
			LONGLONG get() { return srcSize_; }
			void set(LONGLONG v) {
				srcSize_ = v;
				ThreadTransitory::CurSize = v;
			}
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
			if (hSrc_ != nullptr && hSrc_ != INVALID_HANDLE_VALUE)
			{
				CloseHandle(hSrc_);
				hSrc_ = nullptr;
			}
			LONGLONG prevSize = SrcSize;
			HANDLE hTmp = HSrc;
			if (hTmp == nullptr || hTmp == INVALID_HANDLE_VALUE ||
				SrcSize == 0)
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
			DWORD get() { return srcLE_; }
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
			if (hSrc_ != nullptr && hSrc_ != INVALID_HANDLE_VALUE)
			{
				CloseHandle(hSrc_);
				hSrc_ = nullptr;
			}

			if (hDst_ != nullptr && hDst_ != INVALID_HANDLE_VALUE)
			{
				CloseHandle(hDst_);
				hDst_ = nullptr;
			}
		}
	};
	delegate void VTmDelegate(ThreadDataMaster^ thData);
	delegate void VTfDelegate(ThreadDataFile^ thDataFile);


}