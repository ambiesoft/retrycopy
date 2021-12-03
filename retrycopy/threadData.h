#pragma once

#include <Windows.h>
#include "OverwriteInfo.h"
#include "RemoveInfo.h"
using namespace System;

namespace retrycopy {
	ref class ThreadDataFile;


	public ref class ThreadTransitory abstract sealed
	{
		static System::Threading::ReaderWriterLock^ rwl_ = gcnew System::Threading::ReaderWriterLock();

		static int threadNumber_ = 1;
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
		static bool userRemoveDirty_;

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
		static void OnUpdate() 
		{
		}

	public:
		static void init() {
			totalSize_ = 0;
			processedTotalSize_ = 0;

			totalCount_ = 0;
			processedCount_ = 0;

			curSize_ = 0;
			curProcessedSize_ = 0;

			fileLastError_ = String::Empty;
			userRemoveDirty_ = false;
		}
		static int IncrementThreadNumber() {
			threadNumber_++;
			return threadNumber_;
		}
		static property int ThreadNumber {
			int get() { return threadNumber_; }
		}
		static property bool HasUserRemoveChanged
		{
			bool get() { return userRemoveDirty_; }
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
				return (int)TotalPercentAsDouble;
			}
		}
		static property double TotalPercentAsDouble
		{
			double get() {
				return ((double)ProcessedTotalSize * 100 / (double)TotalSize);
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
				int t = userBuffer_;
				return std::clamp(t, MINREADBUFFERSIZE, MAXREADBUFFERSIZE);
			}
			void set(int v) {
				WLocker w(rwl_);
				userBuffer_ = std::clamp(v, MINREADBUFFERSIZE, MAXREADBUFFERSIZE);
			}
		}
		static property int UserRetry
		{
			int get() {
				RLocker r(rwl_);
				int t = userRetry_;
				return std::clamp(t, MINRETRYCOUNT, MAXRETRYCOUNT);
			}
			void set(int v) {
				WLocker w(rwl_);
				userRetry_ = std::clamp(v, MINRETRYCOUNT, MAXRETRYCOUNT);
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
				if (userRemove_ != v)
					userRemoveDirty_ = true;
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
		initonly int threadNumber_;
		initonly String^ src_;
		initonly String^ dst_;
		KVS^ sds_ = nullptr;
		System::Collections::Generic::List<String^>^ dstdirs_ = nullptr;
		LONGLONG totalInputSize_;
		LONGLONG totalProcessed_;
		int totalOK_;
		int totalSkipped_;
	public:
		ThreadDataMaster(int threadNumber, String^ src, String^ dst):
			threadNumber_(threadNumber), src_(src), dst_(dst)
		{}
		property int ThreadNumber
		{
			int get() { return threadNumber_; }
		}
		property String^ Src
		{
			String^ get() { return src_; }
		}
		property String^ Dst
		{
			String^ get() { return dst_; }
		}
		void SetSDS(KVS^ sds, System::Collections::Generic::List<String^>^ dstdirs)
		{
			DASSERT(sds_ == nullptr);
			DASSERT(dstdirs_ == nullptr);
			DASSERT(sds);
			DASSERT(dstdirs);
			sds_ = sds;
			dstdirs_ = dstdirs;
		}
		void PrepareDstDirs();
		property bool HasSrcDir
		{
			bool get() { return System::IO::Directory::Exists(src_); }
		}
		property String^ SrcDir
		{
			String^ get() {
				if (HasSrcDir)
					return src_;
				DASSERT(false);
				return nullptr;
			}
		}
		property KVS^ SDS
		{
			KVS^ get() { return sds_; }
		}
		void IncrementOK() {
			++totalOK_;
		}
		void IncrementSkipCount() {
			++totalSkipped_;
		}
		property bool IsOK
		{
			bool get() { return totalOK_ == sds_->Count; }
		}
		property int TotalOKCount
		{
			int get() { return totalOK_; }
		}
		property int TotalSkipCount
		{
			int get() { return totalSkipped_; }
		}
		property int TotalFailCount
		{
			int get() { return sds_->Count - TotalOKCount - TotalSkipCount; }
		}
		property LONGLONG TotalWrittenSize
		{
			LONGLONG get() { return totalProcessed_; }
		}
		property bool HasFailed
		{
			bool get() { return TotalFailCount != 0; }
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
		initonly int threadNumber_;

		String^ srcFile_;
		HANDLE hSrc_;
		DWORD srcLE_;

		String^ dstFile_;
		HANDLE hDst_;

		LONGLONG srcSize_ = -1;
		LONGLONG allProcessed_;

		DWORD leRead_ = 0;
		DWORD leWrite_ = 0;
		bool done_ = false;
		int taskNo_ = -1;
		DWORD dwCDForWrite_ = 0;
		bool skipped_ = false;
		LONGLONG zeroWritten_ = 0;
	public:
		ThreadDataFile(int threadNumber, int taskNo, String^ srcFile, String^ dstFile):
			threadNumber_(threadNumber), taskNo_(taskNo), srcFile_(srcFile), dstFile_(dstFile){}

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
				return done_ && zeroWritten_ == 0 &&
					leRead_ == 0 && leWrite_ == 0 &&
					SrcSize != -1 && SrcSize == allProcessed_;
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
				SrcSize == -1)
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