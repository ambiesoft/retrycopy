#include "stdafx.h"

#include "helper.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
using namespace std;

namespace Ambiesoft {
	namespace retrycopy {

		String^ SizeToUser(LONGLONG size)
		{
			return String::Format(L"{0} ({1})",
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
		List<String^>^ GetDirsFromDstFiles(KVS^ sds)
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
		String^ V2S(const vector<wstring>& vs)
		{
			wstring all;
			for (auto&& ws : vs)
			{
				all += stdAddDQIfNecessary(ws);
				all += L" ";
			}
			all = stdTrim(all);
			return gcnew String(all.c_str());
		}
		cli::array<String^>^ S2A(String^ args)
		{
			List<String^> ret;
			if (String::IsNullOrWhiteSpace(args))
				return ret.ToArray();
			CCommandLineString cmd(TO_LPCWSTR(args));
			for (size_t i = 0; i < cmd.getCount(); ++i)
			{
				ret.Add(gcnew String(cmd.getArg(i).c_str()));
			}
			return ret.ToArray();
		}
	} // namespace
}