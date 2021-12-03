#include "stdafx.h"
#include "FormMain.h"
#include "FormAbout.h"

namespace retrycopy {
	System::Void FormAbout::FormAbout_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) 
	{
		if (FormMain::AppClosing)
			return;
		e->Cancel = true;
		Visible = false;
	}

}
