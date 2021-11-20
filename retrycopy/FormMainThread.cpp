#include "stdafx.h"

#include "FormMain.h"

using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
namespace retrycopy {

	ref class ThreadData
	{
		String^ src_;
		String^ dst_;
	public:
		ThreadData(String^ src, String^ dst) :
			src_(src), dst_(dst){}
	};

	void FormMain::StartOfThread(Object^ obj)
	{

	}
	void FormMain::ClearThread()
	{
		if (hSource_ != NULL && hSource_ != INVALID_HANDLE_VALUE)
			CloseHandle(hSource_);
		if (hDestination_ != NULL && hDestination_ != INVALID_HANDLE_VALUE)
			CloseHandle(hDestination_);

		if (theThread_)
		{
			theThread_->Abort();
			theThread_->Join();
		}

	}
	System::Void FormMain::btnCopy_Click(System::Object^ sender, System::EventArgs^ e)
	{
		ClearThread();
		if (String::IsNullOrEmpty(txtSource->Text))
		{
			CppUtils::Alert(I18N(L"Source file is empty."));
			return;
		}
		if (!File::Exists(txtSource->Text))
		{
			CppUtils::Alert(I18N(L"Source file does not exist."));
			return;
		}

		ThreadData^ thData = gcnew ThreadData(txtSource->Text, txtDestination->Text);
		theThread_ = gcnew Thread(
			gcnew ParameterizedThreadStart(this, &FormMain::StartOfThread));
		thFFMpeg_->Start(% param);

		gcnew System::Threading::Thread()
			);
	}

}