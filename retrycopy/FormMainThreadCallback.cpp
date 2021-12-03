#include "stdafx.h"

#include "helper.h"
#include "ReadErrorDialog.h"
#include "threadData.h"
#include "FormMain.h"

#pragma comment(lib, "User32.lib")

using namespace System::Text;

using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
namespace retrycopy {
	bool FormMain::OnThreadYesNo(int tn, String^ question)
	{
		if (tn != ThreadTransitory::ThreadNumber)
			return false;
		return System::Windows::Forms::DialogResult::Yes == CppUtils::YesOrNo(this, question,
			MessageBoxDefaultButton::Button2);
	}
	void FormMain::OnThreadError(int tn, String^ error)
	{
		if (tn != ThreadTransitory::ThreadNumber)
			return;
		CppUtils::Alert(this, error);
	}
	void FormMain::ThreadFileStarted(ThreadDataFile^ thData)
	{
		if (thData->ThreadNumber != ThreadTransitory::ThreadNumber)
			return;
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
			
			// for (int i = 0; i < 10; ++i)
			String^ error;
			{
				if (bRecycle)
				{
					//CppUtils::DeleteFile(thData->SrcFile);
					int err = SHDeleteFileEx(TO_LPCWSTR(thData->SrcFile), FOF_SILENT | FOF_NOCONFIRMATION | FOF_ALLOWUNDO);
					if (err != 0)
					{
						error = gcnew String(GetSHFileOpErrorString(err).c_str());
					}
				}
				else
				{
					try
					{
						File::Delete(thData->SrcFile);
					}
					catch (Exception^ ex) 
					{
						error = ex->Message;
					}
				}
				//if (!File::Exists(thData->SrcFile))
				//	break;
			}
			if (File::Exists(thData->SrcFile))
			{
				sbResult->AppendFormat(I18N(L"Failed to {0} source file ({1})"),
					recyleORdelete, error);
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
	void RemoveDirCommon(String^ dir, StringBuilder^ sbResult, bool bRecycle)
	{
		if (Directory::Exists(dir))
		{
			String^ recyleORdelete = bRecycle ? I18N(L"recycle") : I18N(L"delete");
			String^ recyleORdeleted = bRecycle ? I18N(L"recycled") : I18N(L"deleted");
			if (bRecycle)
			{
				CppUtils::DeleteFile(dir);
			}
			else
			{
				AmbLib::DeleteAllEmptyDirectory(dir);
			}

			if (Directory::Exists(dir))
			{
				sbResult->AppendFormat(I18N(L"Failed to {0} some directories"),
					recyleORdelete);
			}
			else
			{
				sbResult->AppendFormat(I18N(L"Source directory {0}"),
					recyleORdeleted);
			}
		}
		else
		{
			sbResult->Append(I18N(L"Source directory already gone."));
		}
	}
	void FormMain::ThreadFileEnded(ThreadDataFile^ thDataFile)
	{
		if (ThreadTransitory::ThreadNumber != thDataFile->ThreadNumber)
			return;

		{
			String^ logMessage = String::Format(I18N(L"{0}:File copy ended:\"{1}\" -> \"{2}\""),
				thDataFile->TaskNo, thDataFile->SrcFile, thDataFile->DstFile);
			AppendLog(logMessage);
		}

		StringBuilder sbResult;
		sbResult.Append(String::Format(I18N(L"{0}:Result:{1}"),
			thDataFile->TaskNo, thDataFile->IsOK ? L"OK" : L"NG"));
		if (!thDataFile->IsOK)
		{
			sbResult.Append(L"\t");
			sbResult.Append(I18N(L"NG Reason:"));
			DASSERT(!String::IsNullOrEmpty(thDataFile->GetNGReason()));
			sbResult.Append(thDataFile->GetNGReason());
			AppendLog(sbResult.ToString());
			return;
		}
		DASSERT(String::IsNullOrEmpty(thDataFile->GetNGReason()));

		switch ((REMOVE_TYPE)cmbRemove->SelectedIndex)
		{
		case REMOVE_TYPE::REMOVE_ASK:
			if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(
				this,
				String::Format(I18N(L"Copy finished. Do you want to remove source file \"{0}\"?"),
					thDataFile->SrcFile),
				MessageBoxDefaultButton::Button2))
			{
				break;
			}
			// fall through
		case REMOVE_TYPE::REMOVE_YES_RECYCLE:
			sbResult.Append(L"\t");
			RemoveFileCommon(thDataFile, % sbResult, true);
			break;
		case REMOVE_TYPE::REMOVE_YES_DELETE:
			sbResult.Append(L"\t");
			RemoveFileCommon(thDataFile, % sbResult, false);
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

	void FormMain::ThreadStarted(int tn)
	{
		if (tn != ThreadTransitory::ThreadNumber)
			return;
		ThreadState = ThreadStateType::RUNNING;
		AppendLogNow(I18N(L"Thread Started"));
	}

	void FormMain::ThreadFinished(int tn)
	{
		if (tn != ThreadTransitory::ThreadNumber)
			return;
		ThreadState = ThreadStateType::NONE;
	}
	void FormMain::ThreadTaskFinished(ThreadDataMaster^ thData)
	{
		if (thData->ThreadNumber != ThreadTransitory::ThreadNumber)
			return;
		if (thData->IsOK)
		{
			StringBuilder sbResult;
			sbResult.Append(I18N(L"All copy successfully finished"));
			sbResult.Append(L"\t");
			if (!ThreadTransitory::HasUserRemoveChanged && thData->HasSrcDir)
			{
				switch (ThreadTransitory::UserRemove)
				{
				case REMOVE_TYPE::REMOVE_ASK:
					if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(
						this,
						String::Format(I18N(L"All Copy finished. Do you want to remove source directories \"{0}\"?"),
							thData->SrcDir),
						MessageBoxDefaultButton::Button2))
					{
						break;
					}
					// fall
				case REMOVE_TYPE::REMOVE_YES_RECYCLE:
					RemoveDirCommon(thData->SrcDir, % sbResult, true);
					break;
				case REMOVE_TYPE::REMOVE_YES_DELETE:
					RemoveDirCommon(thData->SrcDir, % sbResult, false);
					break;
				}
			}
			AppendLogNow(sbResult.ToString());
			CppUtils::Info(this, String::Format(
				I18N(L"success Total Input size={0}, Total Written size={1}"),
				thData->TotalInputSize, thData->TotalWrittenSize));
		}
		else
		{
			StringBuilder message;
			message.AppendLine(thData->HasFailed ? I18N(L"Failed:") : I18N(L"Some files were skipped:"));
			message.AppendLine(String::Format(I18N(L"Total Input size = {0}"),
				thData->TotalInputSize));
			message.AppendLine(String::Format(I18N(L"success = {0}"),
				thData->TotalOKCount));
			message.AppendLine(String::Format(I18N(L"skip = {0}"),
				thData->TotalSkipCount));
			message.AppendLine(String::Format(I18N(L"fail = {0}"),
				thData->TotalFailCount));
			message.AppendLine(String::Format(I18N(L"Total Written size={0}"),
				thData->TotalWrittenSize));

			if (thData->HasFailed)
				CppUtils::Alert(this, message.ToString());
			else
				CppUtils::Info(this, message.ToString());
		}
	}
	bool FormMain::AskOverwrite(int tn, String^ fileTobeOverwritten)
	{
		if (tn != ThreadTransitory::ThreadNumber)
			return false;
		return System::Windows::Forms::DialogResult::Yes == CppUtils::YesOrNo(this,
			String::Format(I18N(L"'{0}' is already exists. Do you want to overwrite?"),
				fileTobeOverwritten),
			MessageBoxDefaultButton::Button2);
	}
	void FormMain::ProgressWriteWithZero(int tn, LONGLONG pos, int bufferSize)
	{
		if (tn != ThreadTransitory::ThreadNumber)
			return;
		AppendLog(String::Format(
			I18N(L"Write zero from {0} with size {1}."), pos, bufferSize));
	}

	bool FormMain::OpenFileFailedGetUserAction(int tn, DWORD le)
	{
		if (tn != ThreadTransitory::ThreadNumber)
			return false;

		String^ message = String::Format(I18N(L"Failed to open file. ({0})"),
			gcnew String(GetLastErrorString(le).c_str()));
		txtLastError->Text = message;

		return System::Windows::Forms::DialogResult::Retry == CppUtils::CenteredMessageBox(this,
			message,
			Application::ProductName,
			MessageBoxButtons::RetryCancel);
	}
	UserResponceOfFail^ FormMain::SFPFailedGetUserAction(int tn, String^ file, LONGLONG pos, LONGLONG allSize, DWORD le, int retried)
	{
		if (tn != ThreadTransitory::ThreadNumber)
			return nullptr;

		UserResponceOfFail^ rfd;
		switch (CppUtils::CenteredMessageBox(
			this,
			String::Format(I18N(L"Failed to SetFilePointer at {0} on the file \"{1}\" {2} times."),
				pos, file, retried),
			Application::ProductName,
			MessageBoxButtons::RetryCancel,
			MessageBoxIcon::Error,
			MessageBoxDefaultButton::Button2))
		{
		case System::Windows::Forms::DialogResult::Retry:
			rfd = gcnew UserResponceOfFail(USERACTION::UA_RETRY);
			break;
		default:
			rfd = gcnew UserResponceOfFail(USERACTION::UA_CANCEL);
			break;
		}
		return rfd;
	}
	UserResponceOfFail^ FormMain::ReadFileFailedGetUserAction(
		int tn,
		String^ file, 
		LONGLONG pos,
		LONGLONG allSize,
		DWORD le,
		int retried,
		int retryCount,
		int bufferSize)
	{
		if (tn != ThreadTransitory::ThreadNumber)
			return nullptr;

		DASSERT(udBuffer->Text == bufferSize.ToString());
		UserResponceOfFail^ rfd;
		ReadErrorDialog dlg(
			String::Format(I18N(L"Failed to read {0} bytes from {1} on the file \"{2}\" {3} times."),
				bufferSize,
				pos,
				file,
				retried),
			bufferSize, retryCount);

		const READERROR_RESPONSE res = dlg.ShowDialogAndGetResponce(this);
		udBuffer->Text = dlg.BufferSize.ToString();
		DASSERT(udBuffer->Text == dlg.BufferSize.ToString());
		switch (res)
		{
		case READERROR_RESPONSE::RR_RETRY:
			rfd = gcnew UserResponceOfFail(USERACTION::UA_RETRY, dlg.BufferSize);
			break;
		case READERROR_RESPONSE::RR_WRITEZERO:
			rfd = gcnew UserResponceOfFail(USERACTION::UA_IGNORE, dlg.BufferSize);
			break;
		case READERROR_RESPONSE::RR_WZOMODE:
			rfd = gcnew UserResponceOfFail(USERACTION::UA_WZOMODE, dlg.BufferSize);
			udBuffer->Value = 1;
			ThreadTransitory::UserBuffer = 1;
			break;
		case READERROR_RESPONSE::RR_CANCEL:
			rfd = gcnew UserResponceOfFail(USERACTION::UA_CANCEL);
			break;
		default:
			CppUtils::Fatal(L"Illegal");
			Environment::Exit(-1);
		}
		return rfd;
	}

} // namespace