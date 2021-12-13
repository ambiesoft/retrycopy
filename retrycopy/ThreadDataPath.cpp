#pragma once

#include "stdafx.h"

#include "FormMain.h"
#include "helper.h"
#include "ThreadDataPath.h"

using namespace System::IO;
namespace Ambiesoft {
	namespace retrycopy {

		void ThreadDataPath::PrepareDstDirs()
		{
			for each (String ^ dir in dstdirs_)
			{
				try
				{
					Directory::CreateDirectory(dir);
				}
				catch (Exception^) {}
			}
		}
	}
}