#include "stdafx.h"
#include "FormMain.h"
#include "FormLog.h"
namespace Ambiesoft {
	namespace retrycopy {

		System::Void FormLog::FormLog_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
			if (!forceClose_)
				return;
			e->Cancel = true;
			Visible = false;
		}

	}
}