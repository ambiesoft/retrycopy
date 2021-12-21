#pragma once

#include <Windows.h>
#include "OverwriteInfo.h"
#include "OperationInfo.h"
using namespace System;

namespace Ambiesoft {
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
			static OPERATION userOperation_;

			static int userBuffer_ = DEFAULTREADBUFFERSIZE;
			static int userRetry_ = 10;

			static String^ lastErrorDisp_ = String::Empty;
			static String^ progressDisp_ = String::Empty;
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

				progressDisp_ = String::Empty;
				lastErrorDisp_ = String::Empty;
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
			static property OPERATION UserOperation
			{
				OPERATION get() {
					RLocker r(rwl_);
					return userOperation_;
				}
				void set(OPERATION v) {
					WLocker w(rwl_);
					if (userOperation_ != v)
						userRemoveDirty_ = true;
					userOperation_ = v;
				}
			}
			static void SetLastErrorDisp(LONGLONG pos, LONGLONG allSize, DWORD le, int retried);
			static void SetProgress(String^ message) {
				progressDisp_ = message;
			}
			static void SetReadingProgress(LONGLONG pos, LONGLONG allSize, int retried);
			static void SetWrittingProgress(LONGLONG pos, LONGLONG allSize);
			static void ClearLastErrorDisp() {
				lastErrorDisp_ = String::Empty;
			}
			static property String^ ProgressDisp
			{
				String^ get() { return progressDisp_; }
			}
			static property String^ LastErrorDisp
			{
				String^ get() { return lastErrorDisp_; }
			}
		};
	}
}