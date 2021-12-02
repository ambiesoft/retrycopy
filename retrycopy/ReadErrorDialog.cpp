#include "stdafx.h"
#include "ReadErrorDialog.h"

using namespace Ambiesoft;

namespace retrycopy {

	READERROR_RESPONSE ReadErrorDialog::ShowDialogAndGetResponce()
	{
		if (System::Windows::Forms::DialogResult::Cancel == this->ShowDialog())
			return READERROR_RESPONSE::RR_CANCEL;
		return responce_;
	}
	System::Void ReadErrorDialog::btnRetry_Click(System::Object^ sender, System::EventArgs^ e)
	{
		responce_ = READERROR_RESPONSE::RR_RETRY;
		this->DialogResult = System::Windows::Forms::DialogResult::OK;
		Close();
	}
	System::Void ReadErrorDialog::btnChangeBufferSize_Click(System::Object^ sender, System::EventArgs^ e)
	{
		System::Decimal ret = bufferSize_;
		if (!NumericTextDialog::UIGetNumeric(
			I18N(L"Enter new buffer size"),
			I18N(L"Buffer size:"),
			1, MAXREADBUFFERSIZE,
			ret))
		{
			return;
		}
		bufferSize_ = Decimal::ToInt32(ret);
	}
	System::Void ReadErrorDialog::btnGiveupAndWriteZero_Click(System::Object^ sender, System::EventArgs^ e)
	{
		String^ message = String::Format(
			I18N(L"Are you sure to give up reading {0} bytes from the file and write {0} bytes of zeros?"),
			BufferSize);
		if (System::Windows::Forms::DialogResult::Yes !=
			CppUtils::YesOrNo(this, message, MessageBoxDefaultButton::Button2))
		{
			return;
		}
		responce_ = READERROR_RESPONSE::RR_WRITEZERO;
		this->DialogResult = System::Windows::Forms::DialogResult::OK;
		Close();
	}

}