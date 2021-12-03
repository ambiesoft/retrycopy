#pragma once

namespace retrycopy {
	System::String^ SizeToUser(LONGLONG size);
	bool ShouldReopenError(DWORD le);
	void EnsureDirectory(System::String^ path);
	System::Collections::Generic::List<System::String^>^ GetDirsFromDstFiles(KVS^ sds);
}