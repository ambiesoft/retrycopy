#include "stdafx.h"
#include "FormMain.h"
#include "FormLog.h"

namespace retrycopy {

	System::Void FormLog::FormLog_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
		if (FormMain::AppClosing)
			return;
		e->Cancel = true;
		Visible = false;
	}

}