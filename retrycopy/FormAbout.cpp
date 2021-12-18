#include "stdafx.h"
#include "FormMain.h"
#include "FormAbout.h"

using namespace System::Diagnostics;
namespace Ambiesoft {
	namespace retrycopy {
		System::Void FormAbout::FormAbout_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
		{
			if (!forceClose_)
				return;
			e->Cancel = true;
			Visible = false;
		}
		void OpenUrl(String^ url)
		{
			try
			{
				Process::Start(url);
			}
			catch (Exception^ ex)
			{
				CppUtils::Alert(ex);
			}
		}

		System::Void FormAbout::linkHomepage_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
		{
			OpenUrl(L"https://ambiesoft.github.io/webjumper/?target=retrycopy");
		}
		System::Void FormAbout::linkDonation_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
		{
			OpenUrl(L"https://ambiesoft.github.io/webjumper/?target=donate");
		}
	}
}