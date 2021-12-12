#include "stdafx.h"

#include "helper.h"
#include "ReadErrorDialog.h"
#include "threadData.h"
#include "FormMain.h"
using namespace System::Collections::Generic;

#pragma comment(lib, "User32.lib")

using namespace System::Text;

using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

namespace Ambiesoft {
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

			AppendLog(thData->TaskNo.ToString() + L":" +
				String::Format(I18N(L"File copy started:\"{0}\" -> \"{1}\""),
				thData->SrcFile, thData->DstFile));
		}
		bool ThreeEqual(LONGLONG l1, LONGLONG l2, LONGLONG l3)
		{
			return l1 == l2 && l2 == l3;
		}
		void RemoveFileCommon(ThreadDataFile^ thData, List<String^>^ results, bool bRecycle)
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
					results->Add(String::Format(I18N(L"{0} cancelled because size is not same"),
						recyleORdelete));
					return;
				}

				if (bRecycle)
				{
					int err = SHDeleteFileEx(TO_LPCWSTR(thData->SrcFile), FOF_SILENT | FOF_NOCONFIRMATION | FOF_ALLOWUNDO);
					if (err != 0)
					{
						results->Add(String::Format(
							I18N(L"Failed to delete the source file '{0}' ({1})"),
							thData->SrcFile, gcnew String(GetSHFileOpErrorString(err).c_str())));
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
						results->Add(String::Format(
							I18N(L"Failed to delete the source file '{0}' ({1})"),
							thData->SrcFile, ex->Message));
					}
				}

				if (!File::Exists(thData->SrcFile))
				{
					results->Add(String::Format(I18N(L"The source file '{0}' {1}"),
						thData->SrcFile, recyleORdeleted));
				}
			}
			else
			{
				results->Add(String::Format(I18N(L"The source file '{0}' already gone."),
					thData->SrcFile));
			}
		}
		void RemoveDirCommon(String^ dir, List<String^>^ results, bool bRecycle)
		{
			if (Directory::Exists(dir))
			{
				String^ recyleORdelete = bRecycle ? I18N(L"recycle") : I18N(L"delete");
				String^ recyleORdeleted = bRecycle ? I18N(L"recycled") : I18N(L"deleted");

				if (bRecycle)
				{
					if (0 == stdGetFileCount(TO_LPCWSTR(dir)))
					{
						int err = CppUtils::DeleteFile(dir);
						if (err != 0)
						{
							results->Add(String::Format(L"Failed to delete '{0}' ({1})",
								dir, gcnew String(GetSHFileOpErrorString(err).c_str())));
						}
					}
					else
					{
						results->Add(String::Format( // TODO: count file and check
							I18N(L"The source directory '{0}' has not been recycled because it is not empty."),
							dir));
					}
				}
				else
				{
					List<KeyValuePair<String^, Exception^>> daedError;
					AmbLib::DeleteAllEmptyDirectory(dir, % daedError);
					for each (KeyValuePair<String^, Exception^> kv in daedError)
					{
						results->Add(String::Format(I18N(L"Failed to delete directory '{0}' ({1})"),
							kv.Key, kv.Value->Message));
					}
				}

				if (Directory::Exists(dir))
				{
					results->Add(String::Format(I18N(L"Failed to {0} some directories"),
						recyleORdelete));
				}
				else
				{
					results->Add(String::Format(I18N(L"The source directory '{0}' {1}"),
						dir, recyleORdeleted));
				}
			}
			else
			{
				results->Add(String::Format(I18N(L"The source directory '{0}' already gone."),
					dir));
			}
		}
		void FormMain::ThreadFileEnded(ThreadDataFile^ thDataFile)
		{
			if (ThreadTransitory::ThreadNumber != thDataFile->ThreadNumber)
				return;

			{
				String^ logMessage = thDataFile->TaskNo.ToString() + L":" +
					String::Format(I18N(L"File copy ended:\"{0}\" -> \"{1}\""),
					thDataFile->SrcFile, thDataFile->DstFile);
				AppendLog(logMessage);
			}


			AppendLog(thDataFile->TaskNo.ToString() + L":" +
				String::Format(I18N(L"Result:{0}"),
				thDataFile->IsOK ? L"OK" : L"NG"));
			if (!thDataFile->IsOK)
			{
				StringBuilder sbResult;
				sbResult.Append(L"\t");
				sbResult.Append(I18N(L"NG Reason:"));
				DASSERT(!String::IsNullOrEmpty(thDataFile->GetNGReason()));
				sbResult.Append(thDataFile->GetNGReason());
				AppendLog(sbResult.ToString());
				return;
			}
			DASSERT(String::IsNullOrEmpty(thDataFile->GetNGReason()));

			List<String^> results;
			switch ((OPERATION)cmbOperation->SelectedIndex)
			{
			case OPERATION::ASK:
				if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(
					this,
					String::Format(I18N(L"Copy finished. Do you want to recycle source file '{0}'?"),
						thDataFile->SrcFile),
					MessageBoxDefaultButton::Button2))
				{
					break;
				}
				// fall through
			case OPERATION::MOVERECYCLE:
				RemoveFileCommon(thDataFile, % results, true);
				break;
			case OPERATION::MOVE:
				RemoveFileCommon(thDataFile, % results, false);
				break;
			case OPERATION::COPY:
				// results.Add(I18N(L"copying (file not removed)"));
				break;
			default:
				DASSERT(false);
			}
			AppendLog(% results);
		}

		void FormMain::ThreadStarted(int tn)
		{
			if (tn != ThreadTransitory::ThreadNumber)
				return;
			ThreadState = ThreadStateType::RUNNING;
		}

		void FormMain::ThreadPathFinished(ThreadDataPath^ thPath)
		{
			if (!thPath->IsOK)
				return;

			List<String^> results;
			if (!ThreadTransitory::HasUserRemoveChanged && thPath->HasSrcDir)
			{
				switch (ThreadTransitory::UserOperation)
				{
				case OPERATION::ASK:
					if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(
						this,
						String::Format(I18N(L"All Copy finished. Do you want to recycle source directories '{0}'?"),
							thPath->SrcDir),
						MessageBoxDefaultButton::Button2))
					{
						break;
					}
					// fall
				case OPERATION::MOVERECYCLE:
					RemoveDirCommon(thPath->SrcDir, % results, true);
					break;
				case OPERATION::MOVE:
					RemoveDirCommon(thPath->SrcDir, % results, false);
					break;
				case OPERATION::COPY:
					break;
				default:
					DASSERT(false);
					ExitProcess(1);
				}
			}
			AppendLog(% results, true);
		}

		List<String^>^ AppendResultToList(List<String^>^ list)
		{
			List<String^>^ ret = gcnew List<String^>();
			for each (String ^ s in list)
			{
				ret->Add(I18N(L"Result:") + L"\t" + s);
			}
			return ret;
		}
		void FormMain::ThreadFinished(ThreadDataMaster^ thData)
		{
			bool bClose = false;
			try
			{
				if (thData->ThreadNumber != ThreadTransitory::ThreadNumber)
					return;

				if (!thData->TaskStarted)
					return;

				List<String^>^ messages = gcnew List<String^>();
				if (thData->IsOK)
				{
					messages->Add(I18N(L"All operation successfully finished"));
				}
				else
				{
					messages->Add(thData->HasFailed ? I18N(L"Failed:") : I18N(L"Some files were skipped:"));

				}
				messages->Add(String::Format(
					I18N(L"Total number of files = {0}"),
					thData->TotalInputFileCount));
				messages->Add(String::Format(
					I18N(L"Success = {0}"),
					thData->TotalOKCount));
				messages->Add(String::Format(
					I18N(L"Skip = {0}"),
					thData->TotalSkipCount));
				messages->Add(String::Format(
					I18N(L"Fail = {0}"),
					thData->TotalFailCount));
				messages->Add(String::Format(
					I18N(L"Total file size = {0}"),
					thData->TotalInputSize));
				messages->Add(String::Format(
					I18N(L"Total written size = {0}"),
					thData->TotalWrittenSize));

				AppendLog(AppendResultToList(messages), true);

				if (thData->IsOK)
				{
					if (IsCloseOnFinish)
					{
						bClose = true;
						return;
					}
					else
					{
						CppUtils::Info(this, String::Join(Environment::NewLine, messages->ToArray()));
					}
				}
				else
				{
					if (thData->HasFailed)
						CppUtils::Alert(this, String::Join(Environment::NewLine, messages->ToArray()));
					else
						CppUtils::Info(this, String::Join(Environment::NewLine, messages->ToArray()));
				}
			}
			finally
			{
				ThreadState = ThreadStateType::NONE;
				if (bClose)
					Close();
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
				I18N(L"Written zero from position {0} with size {1}."), pos, bufferSize));
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
				String::Format(I18N(L"Failed to SetFilePointer at {0} on the file '{1}' {2} times."),
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

			UserResponceOfFail^ rfd;
			StringBuilder sbMessage;

			// Get Disk Info
			{
				sbMessage.AppendLine(String::Format(I18N(L"Failed to read {0} bytes from {1} on the file '{2}' {3} times."),
					bufferSize,
					pos,
					file,
					retried));
				DWORD dwSectorsPerCluster = 0;
				DWORD dwBytesPerSector = 0;
				DWORD dwNumberOfFreeClusters = 0;
				DWORD dwTotalNumberOfClusters = 0;
				sbMessage.AppendLine();
				sbMessage.AppendLine(I18N(L"Disk Information:"));
				if (!GetDiskFreeSpace(TO_LPCWSTR(Path::GetPathRoot(file)),
					&dwSectorsPerCluster,
					&dwBytesPerSector,
					&dwNumberOfFreeClusters,
					&dwTotalNumberOfClusters))
				{
					sbMessage.AppendLine(I18N(L"Failed to get Disk Infomation"));
				}
				else
				{
					sbMessage.AppendLine(String::Format(I18N(L"The total number of clusters of the dirve '{0}' is {1}."),
						Path::GetPathRoot(file), dwTotalNumberOfClusters));
					sbMessage.AppendLine(String::Format(I18N(L"The number of free clusters of the dirve '{0}' is {1}."),
						Path::GetPathRoot(file), dwNumberOfFreeClusters));
					sbMessage.AppendLine(String::Format(I18N(L"The number of sectors per cluster of the dirve '{0}' is {1}."),
						Path::GetPathRoot(file), dwSectorsPerCluster));
					sbMessage.AppendLine(String::Format(I18N(L"The sector size of the dirve '{0}' is {1} bytes."),
						Path::GetPathRoot(file), dwBytesPerSector));
				}
			}
			ReadErrorDialog dlg(sbMessage.ToString(), Path::GetPathRoot(file),
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
}