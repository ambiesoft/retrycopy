#pragma once

namespace retrycopy {
	System::String^ SizeToUser(LONGLONG size);
	void EnsureDirectory(System::String^ path);
	System::Collections::Generic::List<System::String^>^ GetDirsFromDstFiles(KVS^ sds);
	bool DeleteAllDirectory(System::String^ dir);
}