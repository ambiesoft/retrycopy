#pragma once

#include "stdafx.h"

#include "FormMain.h"
#include "helper.h"
#include "ThreadDataMaster.h"

using namespace System::IO;
namespace Ambiesoft {
	namespace retrycopy {

		ThreadDataMaster::ThreadDataMaster(int threadNumber, String^ srces, String^ dst) :
			threadNumber_(threadNumber), dst_(dst)
		{
			srcPaths_ = S2A(srces);
		}


		int ThreadDataMaster::TotalOKCount::get()
		{
			int ret = 0;
			for each (ThreadDataPath ^ thPath in Tasks) {
				ret += thPath->ToTatalOKCount;
			}
			return ret;
		}

		int ThreadDataMaster::TotalSkipCount::get()
		{
			int ret = 0;
			for each (ThreadDataPath ^ thPath in Tasks) {
				ret += thPath->ToTatalSkipCount;
			}
			return ret;
		}

	}
}