#include "stdafx.h"

#include "FormMain.h"

using namespace Ambiesoft;
namespace retrycopy {

	System::Void FormMain::btnNavSource_Click(System::Object^ sender, System::EventArgs^ e)
	{
		String^ source = AmbLib::GetOpenFileDialog(I18N(L"Select source file"));
		if (String::IsNullOrEmpty(source))
			return;
		txtSource->Text = source;
	}
	System::Void FormMain::btnNavDestination_Click(System::Object^ sender, System::EventArgs^ e)
	{
		String^ destination = AmbLib::GetSaveFileDialog(I18N(L"Select destination file"));
		if (String::IsNullOrEmpty(destination))
			return;
		txtDestination->Text = destination;
	}


}