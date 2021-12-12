#pragma once
namespace Ambiesoft {
	namespace retrycopy {
		System::String^ SizeToUser(LONGLONG size);
		bool ShouldReopenError(DWORD le);
		void EnsureDirectory(String^ path);
		System::Collections::Generic::List<System::String^>^ GetDirsFromDstFiles(KVS^ sds);

		cli::array<String^>^ S2A(String^ args);
		String^ V2S(const std::vector<std::wstring>& vs);
	}
}