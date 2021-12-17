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
		System::Void ReadErrorDialog::ReadErrorDialog_Load(System::Object^ sender, System::EventArgs^ e)
		{
			txtMessage->Select(txtMessage->TextLength, 0);
		}
		void ReadErrorDialog::SetInfoText(Object^ sender)
		{
			DASSERT_IS_CLASS(sender, System::Windows::Forms::Button);
			String^ text;
			if (false)
			{
			}
			else if (sender == btnChangeRetryCount)
			{
				text = String::Format(I18N(L"Changes Retry Count. Current value={0}."),
					RetryCount);
			}
			else if (sender == btnChangeBufferSize)
			{
				text = String::Format(I18N(L"Changes Buffer Size. Current value={0}."),
					BufferSize);
			}
			else if (sender == btnShowDriveInfo)
			{
				text = I18N(L"Shows Drive Info. It requires administration priviledge.");
			}
			else if (sender == btnGiveupAndWriteZero)
			{
				text = I18N(L"Gives up reading from the file and write zeroes.");
			}
			else if (sender == btnGiveUpAndWZOmode)
			{
				text = I18N(L"Gives up on reading from the file and write zeroes automatically that fails until it succeeds to read.");
			}
			else if (sender == btnRetry)
			{
				text = String::Empty;
			}
			else
			{
				DASSERT(false);
			}
			txtInfo->Text = text;
		}
		System::Void ReadErrorDialog::btnRetry_Click(System::Object^ sender, System::EventArgs^ e)
		{
			responce_ = READERROR_RESPONSE::RR_RETRY;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			Close();
		}
		System::Void ReadErrorDialog::btnChangeBufferSize_Click(System::Object^ sender, System::EventArgs^ e)
		{
			System::Decimal ret = BufferSize;
			if (!NumericTextDialog::UIGetNumeric(
				I18N(L"Enter new buffer size"),
				I18N(L"Buffer size")
				+ L":",
				MINREADBUFFERSIZE, MAXREADBUFFERSIZE,
				ret))
			{
				return;
			}
			BufferSize = Decimal::ToInt32(ret);
		}
		System::Void ReadErrorDialog::btnChangeRetryCount_Click(System::Object^ sender, System::EventArgs^ e)
		{
			System::Decimal ret = RetryCount;
			if (!NumericTextDialog::UIGetNumeric(
				I18N(L"Enter new retry count"),
				I18N(L"Retry count")
				+ L":",
				MINRETRYCOUNT, MAXRETRYCOUNT,
				ret))
			{
				return;
			}
			RetryCount = Decimal::ToInt32(ret);
		}
		System::Void ReadErrorDialog::btnGiveupAndWriteZero_Click(System::Object^ sender, System::EventArgs^ e)
		{
			String^ message = String::Format(
				I18N(L"Are you sure to give up reading {0} bytes from the file and write {0} bytes of zeroes?"),
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
			//if (initialBufferSize_ != 1)
			//{
			//	CppUtils::Info(this, I18N(L"Only available when 1-byte read fails. Please set the buffer size to 1 and Retry."));
			//	return;
			//}
			String^ message = String::Format(
				I18N(L"Are you sure to give up on reading {0} bytes and write {0} zeroes automatically that fails {1} times?"),
				BufferSize,
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