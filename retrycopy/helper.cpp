#include "stdafx.h"

#include "helper.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
namespace retrycopy {

	String^ SizeToUser(LONGLONG size)
	{
		return String::Format(I18N(L"{0} ({1})"),
			size,
			AmbLib::FormatSize(size));
	}

	bool ShouldReopenError(DWORD le)
	{
		return le == ERROR_NOT_READY || le == ERROR_DEV_NOT_EXIST || le == ERROR_NO_SUCH_DEVICE;
	}
	void EnsureDirectory(String^ path)
	{
		try
		{
			Directory::CreateDirectory(
				Path::GetDirectoryName(path)
			);
		}
		catch (Exception^) {}
	}
	List<String^>^ GetDirsFromDstFiles(KVS ^ sds)
	{
		List<String^>^ ret = gcnew List<String^>();
		System::Collections::Generic::SortedSet<String^> chkDup;
		for each (KV ^ kv in sds)
		{
			String^ dir = Path::GetDirectoryName(kv->Value);
			if (chkDup.Contains(dir))
				continue;
			chkDup.Add(dir);
			ret->Add(dir);
		}
		return ret;
	}
	
} // namespace