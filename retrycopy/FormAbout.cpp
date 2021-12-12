#include "stdafx.h"
#include "FormMain.h"
#include "FormAbout.h"

using namespace System::Diagnostics;
namespace Ambiesoft {
	namespace retrycopy {
		System::Void FormAbout::FormAbout_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
		{
			if (FormMain::AppClosing)
				return;
			e->Cancel = true;
			Visible = false;
		}
		System::Void FormAbout::linkHomepage_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
		{
			try
			{
				Process::Start(L"https://ambiesoft.github.io/webjumper/?target=retrycopy");
			}
			catch (Exception^ ex)
			{
				CppUtils::Alert(ex);
			}
		}
	}
}