#pragma once

#include <Windows.h>
#include "OverwriteInfo.h"
#include "OperationInfo.h"
#include "CReadingFile.h"
#include "ThreadDataFile.h"
#include "ThreadTransitory.h"
#include "ThreadDataMaster.h"

using namespace System;

namespace Ambiesoft {
	namespace retrycopy {

		ref class ThreadDataMaster;

		ref class ThreadDataPath
		{
			KVS^ sds_ = nullptr;
			System::Collections::Generic::List<String^>^ dstdirs_ = nullptr;
			String^ src_;

			LONGLONG processedSize_;
			int totalOK_;
			int totalSkipped_;

		public:
			ThreadDataPath(String^ src, KVS^ sds, System::Collections::Generic::List<String^>^ dstdirs)
			{
				DASSERT(sds_ == nullptr);
				DASSERT(dstdirs_ == nullptr);
				DASSERT(sds);
				DASSERT(dstdirs);

#ifdef _DEBUG
				if (System::IO::File::Exists(src))
				{
					DASSERT(sds->Count == 1);
				}
#endif
				src_ = src;
				sds_ = sds;
				dstdirs_ = dstdirs;
			}
			property bool IsOK
			{
				bool get() { return totalOK_ == sds_->Count; }
			}
			void IncrementOK() {
				totalOK_++;
			}
			property int ToTatalOKCount {
				int get() { return totalOK_; }
			}
			property int ToTatalSkipCount {
				int get() { return totalSkipped_; }
			}
			void IncrementSkipCount() {
				++totalSkipped_;
			}

			void PrepareDstDirs();
			property bool IsSrcDirectory
			{
				// from here srces_ has many src_
				bool get() { return System::IO::Directory::Exists(src_); }
			}
			property String^ SrcDirectory
			{
				String^ get() {
					if (IsSrcDirectory)
						return src_;
					DASSERT(false);
					return nullptr;
				}
			}
			property String^ SrcPath
			{
				String^ get() {
					return src_;
				}
			}
			property String^ SrcFile
			{
				String^ get() {
					DASSERT(System::IO::File::Exists(src_));
					DASSERT(sds_->Count == 1);
					return src_;
				}
			}
			property String^ DstFile
			{
				String^ get() {
					DASSERT(sds_->Count == 1);
					return sds_[0].Value;
				}
			}
			property KVS^ SDS
			{
				KVS^ get() { return sds_; }
			}

			void IncrementProcessedSize(LONGLONG size)
			{
				processedSize_ += size;
			}
			property LONGLONG ProcessedSize
			{
				LONGLONG get() { return processedSize_; }
			}
		};
		delegate void VTmDelegate(ThreadDataMaster^ thData);
		delegate void VTpDelegate(ThreadDataPath^ thDataPath);
		delegate void VTfDelegate(ThreadDataFile^ thDataFile);
	}
}