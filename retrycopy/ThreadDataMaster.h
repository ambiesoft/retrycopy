#pragma once
#include <Windows.h>
#include "ThreadDataPath.h"

using namespace System;

namespace Ambiesoft {
	namespace retrycopy {

		ref class ThreadDataPath;
		ref class ThreadDataMaster
		{
			initonly int threadNumber_;
			cli::array<String^>^ srcPaths_;
			initonly String^ dst_;

			bool taskStarted_ = false;
			LONGLONG totalInputSize_;
			LONGLONG totalProcessed_;

			int totalInputFilesCount_;
		public:
			ThreadDataMaster(int threadNumber, String^ srces, String^ dst);

			property bool IsSrcPathsEmpty
			{
				bool get() {
					if (!srcPaths_)
						return true;
					if (srcPaths_->Length == 0)
						return true;
					bool has = false;
					for each (String ^ s in SrcPaths) {
						if (!String::IsNullOrEmpty(s))
							has = true;
					}
					return !has;
				}
			}
			property int ThreadNumber
			{
				int get() { return threadNumber_; }
			}
			property cli::array<String^>^ SrcPaths
			{
				cli::array<String^>^ get() { return srcPaths_; }
			}
			property String^ Dst
			{
				String^ get() { return dst_; }
			}
			void SetTaskStarted() {
				taskStarted_ = true;
			}
			property bool TaskStarted
			{
				bool get() { return taskStarted_; }
			}

			property bool IsOK
			{
				bool get() { return TotalOKCount == totalInputFilesCount_; }
			}
			property int TotalInputFileCount
			{
				int get() { return totalInputFilesCount_; }
			}
			property int TotalOKCount
			{
				int get();
			}
			property int TotalSkipCount
			{
				int get();
			}
			property int TotalFailCount
			{
				int get() { return totalInputFilesCount_ - TotalOKCount - TotalSkipCount; }
			}
			property LONGLONG TotalWrittenSize
			{
				LONGLONG get() { return totalProcessed_; }
			}
			property bool HasFailed
			{
				bool get() { return TotalFailCount != 0; }
			}
			void SetTotalSize(LONGLONG size, int fileCount) {
				totalInputSize_ = size;
				totalInputFilesCount_ = fileCount;
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

		private:
			System::Collections::Generic::List<ThreadDataPath^> pathTasks_;
		public:
			void AddTask(ThreadDataPath^ thPath) {
				pathTasks_.Add(thPath);
			}
			property cli::array<ThreadDataPath^>^ Tasks
			{
				cli::array<ThreadDataPath^>^ get() {
					return pathTasks_.ToArray();
				}
			}
		};
	}
}