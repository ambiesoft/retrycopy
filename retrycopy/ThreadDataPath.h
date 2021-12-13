#pragma once

#include <Windows.h>
#include "OverwriteInfo.h"
#include "RemoveInfo.h"
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

			int totalOK_;
			int totalSkipped_;

		public:
			ThreadDataPath(String^ src, KVS^ sds, System::Collections::Generic::List<String^>^ dstdirs)
			{
				DASSERT(sds_ == nullptr);
				DASSERT(dstdirs_ == nullptr);
				DASSERT(sds);
				DASSERT(dstdirs);
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
			property bool HasSrcDir
			{
				// from here srces_ has many src_
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

		};
		delegate void VTmDelegate(ThreadDataMaster^ thData);
		delegate void VTpDelegate(ThreadDataPath^ thDataPath);
		delegate void VTfDelegate(ThreadDataFile^ thDataFile);
	}
}