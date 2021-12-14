#include "stdafx.h"
#include "ReadErrorDialog.h"
using namespace System::Diagnostics;
using namespace System::IO;

namespace Ambiesoft {

	namespace retrycopy {

		READERROR_RESPONSE ReadErrorDialog::ShowDialogAndGetResponce(System::Windows::Forms::IWin32Window^ parent)
		{
			if (System::Windows::Forms::DialogResult::Cancel == this->ShowDialog(parent))
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
				I18N(L"Buffer size") + L":",
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
		System::Void ReadErrorDialog::btnGiveUpAndWZOmode_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (initialBufferSize_ != 1)
			{
				CppUtils::Info(this, I18N(L"Only available when 1-byte read fails. Please set the buffer size to 1 and Retry."));
				return;
			}
			String^ message = String::Format(
				I18N(L"Are you sure to give up on reading any more bytes, and read one byte at a time, writing zero for each one that fails {0} times?"),
				RetryCount + 1);
			if (System::Windows::Forms::DialogResult::Yes !=
				CppUtils::YesOrNo(this, message, MessageBoxDefaultButton::Button2))
			{
				return;
			}
			responce_ = READERROR_RESPONSE::RR_WZOMODE;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			Close();
		}
		System::Void ReadErrorDialog::btnShowDriveInfo_Click(System::Object^ sender, System::EventArgs^ e)
		{
			try
			{
				Process::Start(
					Path::Combine(Path::GetDirectoryName(Application::ExecutablePath),
						"GetDriveInfo.exe"),
					Drive);
			}
			catch (Exception^ ex)
			{
				CppUtils::Alert(ex);
			}
		}
		System::Void ReadErrorDialog::ReadErrorDialog_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
		{
			if (this->DialogResult != System::Windows::Forms::DialogResult::OK)
			{
				if (System::Windows::Forms::DialogResult::Yes !=
					CppUtils::YesOrNo(this, I18N("Are you sure to quit?"), MessageBoxDefaultButton::Button2))
				{
					e->Cancel = true;
					return;
				}
			}
		}
	}
}