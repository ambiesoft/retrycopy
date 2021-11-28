#include "stdafx.h"

#include "FormMain.h"

#pragma comment(lib, "User32.lib")

using namespace System::Text;

using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
namespace retrycopy {
	void FormMain::ThreadFileStarted(ThreadDataFile^ thData)
	{
		txtCurSrc->Text = thData->SrcFile;
		txtCurDst->Text = thData->DstFile;

		AppendLog(String::Format(I18N(L"File started:'{0}' -> '{1}'"),
			thData->SrcFile, thData->DstFile));
	}
	void FormMain::ThreadFileEnded(ThreadDataFile^ thData)
	{
		String^ logMessage = String::Format(I18N(L"File eneded:'{0}' -> '{1}'"),
			thData->SrcFile,thData->DstFile);
		AppendLog(logMessage);
		logMessage = String::Format(I18N(L"Result:{0}"),
			thData->IsOK ? L"OK" : L"NG");
		if (!thData->IsOK)
		{
			logMessage = I18N(L"NG Reason:");
			logMessage += thData->GetNGReason();
			AppendLog(logMessage);
		}
		if (!thData->IsOK)
		{
			CppUtils::Alert(L"NOT OK");
			return;
		}
		switch ((REMOVE_TYPE)cmbRemove->SelectedIndex)
		{
		case REMOVE_TYPE::REMOVE_YES_RECYCLE:
			CppUtils::DeleteFile(this, thData->SrcFile);
			break;
		case REMOVE_TYPE::REMOVE_YES_DELETE:
			try
			{
				File::Delete(thData->SrcFile);
			}
			catch (Exception^ ex)
			{
				CppUtils::Alert(ex);
			}
			break;
		case REMOVE_TYPE::REMOVE_NO:
			break;
		case REMOVE_TYPE::REMOVE_ASK:
			if (System::Windows::Forms::DialogResult::Yes == CppUtils::YesOrNo(
				this,
				String::Format(I18N(L"Do you want to remove '{0}'?"),
					thData->SrcFile),
				MessageBoxDefaultButton::Button2))
			{
				CppUtils::DeleteFile(this, thData->SrcFile);
			}
			break;
		}

	}

	void FormMain::ThreadStarted()
	{
		AppendLog(I18N(L"Thread Started"));
	}
	void FormMain::ThreadFinished(ThreadDataMaster^ thData)
	{
		ThreadState = ThreadStateType::NONE;
		if (thData->IsOK)
		{
			CppUtils::Info(this, String::Format(
				I18N(L"success Total Input size={0}, Total Written size={1}"),
				thData->TotalInputSize, thData->TotalWrittenSize));
		}
		else
		{
			CppUtils::Fatal("failed");
		}
	}
	bool FormMain::AskOverwrite(String^ fileTobeOverwritten)
	{
		return System::Windows::Forms::DialogResult::Yes == CppUtils::YesOrNo(this,
			String::Format(I18N(L"'{0}' is already exists. Do you want to overwrite?"),
				fileTobeOverwritten),
			MessageBoxDefaultButton::Button2);
	}
	void FormMain::ProcessProgressed_obsolete(LONGLONG pos)
	{
		txtLog->Text = (String::Format(I18N(L"{0} bytes ({1}) done."),
			pos,
			AmbLib::FormatSize(pos)
		));
	}
	void FormMain::ProgressWriteWithZero(LONGLONG pos, int bufferSize)
	{
		txtLog->Text = (String::Format(I18N(L"Write 0 at {0} with size {1}."), pos, bufferSize));
	}

	bool FormMain::OpenFileFailedGetUserAction(DWORD le)
	{
		String^ message = String::Format(I18N(L"Failed to open file. ({0})"),
			gcnew String(GetLastErrorString(le).c_str()));
		txtLog->Text = (message);
		// txtLog->AppendText(Environment::NewLine);

		return System::Windows::Forms::DialogResult::Retry == CppUtils::CenteredMessageBox(this,
			message,
			Application::ProductName,
			MessageBoxButtons::RetryCancel);
	}
	void FormMain::ReadFileFailed_obsolete(LONGLONG pos, LONGLONG allSize, DWORD le, int retried)
	{
		//txtLog->Text = (
		//	String::Format(I18N(L"{0} Failed to ReadFile at {1} ({2})"),
		//		retried,
		//		pos,
		//		gcnew String(GetLastErrorString(le).c_str())));
	}
	Object^ FormMain::ReadFileFailedGetUserAction(LONGLONG pos, LONGLONG allSize, DWORD le, int retried)
	{
		ReadFileFailed_obsolete(pos, allSize, le, retried);
		ReadFailData^ rfd;
		switch (CppUtils::CenteredMessageBox(
			this,
			String::Format(I18N(L"Failed to ReadFile at {0}"), pos),
			Application::ProductName,
			MessageBoxButtons::AbortRetryIgnore,
			MessageBoxIcon::Warning,
			MessageBoxDefaultButton::Button2))
		{
		case System::Windows::Forms::DialogResult::Abort:
			rfd = gcnew ReadFailData(ReadFailData::ACTION::CANCEL);
			break;
		case System::Windows::Forms::DialogResult::Retry:
			rfd = gcnew ReadFailData(ReadFailData::ACTION::RETRY);
			break;
		case System::Windows::Forms::DialogResult::Ignore:
			if (GetAsyncKeyState(VK_SHIFT) < 0)
				rfd = gcnew ReadFailData(ReadFailData::ACTION::IGNOREALL);
			break;
		default:
			CppUtils::Fatal(L"Illegal");
			Environment::Exit(-1);
		}
		return rfd;
	}

} // namespace