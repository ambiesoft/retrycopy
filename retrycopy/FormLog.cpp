#include "stdafx.h"
#include "FormMain.h"
#include "FormLog.h"
namespace Ambiesoft {
	namespace retrycopy {

		System::Void FormLog::FormLog_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
			e->Cancel = true;
			Visible = false;
		}

	}
}