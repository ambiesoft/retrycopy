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

		AppendLog(String::Format(I18N(L"{0}:File copy started:\"{1}\" -> \"{2}\""),
			thData->TaskNo, thData->SrcFile, thData->DstFile));
	}
	bool ThreeEqual(LONGLONG l1, LONGLONG l2, LONGLONG l3)
	{
		return l1 == l2 && l2 == l3;
	}
	void RemoveFileCommon(ThreadDataFile^ thData, StringBuilder^ sbResult, bool bRecycle)
	{
		if (File::Exists(thData->SrcFile))
		{
			String^ recyleORdelete = bRecycle ? I18N(L"recycle") : I18N(L"delete");
			String^ recyleORdeleted = bRecycle ? I18N(L"recycled") : I18N(L"deleted");
			if (!ThreeEqual(
				FileInfo(thData->SrcFile).Length,
				FileInfo(thData->DstFile).Length,
				thData->ProcessedSize))
			{
				sbResult->AppendFormat(I18N(L"{0} cancelled because size is not same"),
					recyleORdelete);
				return;
			}
			if (bRecycle)
			{
				CppUtils::DeleteFile(thData->SrcFile);
			}
			else
			{
				try
				{
					File::Delete(thData->SrcFile);
				}
				catch(Exception^){}
			}

			if (File::Exists(thData->SrcFile))
			{
				sbResult->AppendFormat(I18N(L"Failed to {0} source file"),
					recyleORdelete);
			}
			else
			{
				sbResult->AppendFormat(I18N(L"Source file {0}"),
					recyleORdeleted);
			}
		}
		else
		{
			sbResult->Append(I18N(L"Source file already gone."));
		}
	}
	void FormMain::ThreadFileEnded(ThreadDataFile^ thData)
	{
		{
			String^ logMessage = String::Format(I18N(L"{0}:File copy ended:\"{1}\" -> \"{2}\""),
				thData->TaskNo, thData->SrcFile, thData->DstFile);
			AppendLog(logMessage);
		}

		StringBuilder sbResult;
		sbResult.Append(String::Format(I18N(L"{0}:Result:{1}"),
			thData->TaskNo, thData->IsOK ? L"OK" : L"NG"));
		if (!thData->IsOK)
		{
			sbResult.Append(L"\t");
			sbResult.Append(I18N(L"NG Reason:"));
			sbResult.Append(thData->GetNGReason());
			AppendLog(sbResult.ToString());
			return;
		}
		switch ((REMOVE_TYPE)cmbRemove->SelectedIndex)
		{
		case REMOVE_TYPE::REMOVE_ASK:
			if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(
				this,
				String::Format(I18N(L"Copy finished. Do you want to remove source file \"{0}\"?"),
					thData->SrcFile),
				MessageBoxDefaultButton::Button2))
			{
				break;
			}
			// fall through
		case REMOVE_TYPE::REMOVE_YES_RECYCLE:
			sbResult.Append(L"\t");
			RemoveFileCommon(thData, % sbResult, true);
			break;
		case REMOVE_TYPE::REMOVE_YES_DELETE:
			sbResult.Append(L"\t");
			RemoveFileCommon(thData, % sbResult, false);
			break;
		case REMOVE_TYPE::REMOVE_NO:
			sbResult.Append(L"\t");
			sbResult.Append(I18N(L"No file remove"));
			break;
		default:
			DASSERT(false);
		}
		AppendLog(sbResult.ToString());
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
			CppUtils::Alert(this, String::Format(
				I18N(L"Failed: Total Input size={0}, Success Count={1}, Total Written size={2}"),
				thData->TotalInputSize, 
				thData->TotalOKCount,
				thData->TotalWrittenSize));
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