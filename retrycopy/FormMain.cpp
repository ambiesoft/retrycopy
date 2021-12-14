#include "stdafx.h"

#include "gitrev.h"
#include "ThreadDataPath.h"
#include "helper.h"
#include "FormLog.h"
#include "FormAbout.h"
#include "OverwriteInfo.h"
#include "OperationInfo.h"
#include "ReadErrorDialog.h"
#include "FormMain.h"
#include "CDebug.h"

using namespace System::Collections::Generic;
using namespace System::Text;

using namespace System::IO;
using namespace System::Threading;
using namespace Ambiesoft;
using namespace std;

namespace Ambiesoft {
	namespace retrycopy {

		FormMain::FormMain()
		{
			InitializeComponent();

			theForm_ = this;
			logForm_ = gcnew FormLog();
			udBuffer->Minimum = MINREADBUFFERSIZE;
			udBuffer->Maximum = MAXREADBUFFERSIZE;
			udBuffer->TextChanged += gcnew System::EventHandler(this, &FormMain::OnBufferSizeChanged);

			udRetry->Minimum = MINRETRYCOUNT;
			udRetry->Maximum = MAXRETRYCOUNT;
			udRetry->TextChanged += gcnew System::EventHandler(this, &FormMain::OnRetryCountChanged);

			lblSourceOrig_ = lblSource->Text;

			// first, load from ini
			int intval;
			Ambiesoft::AmbLib::MakeTripleClickTextBox(txtSource);
			Ambiesoft::AmbLib::MakeTripleClickTextBox(txtDestination);

			HashIni^ ini = Profile::ReadAll(IniPath);
			Profile::GetInt(SECTION_OPTION, KEY_RETRY_COUNT,
				ThreadTransitory::UserRetry, intval, ini);
			udRetry->Value = std::clamp(intval, MINRETRYCOUNT, MAXRETRYCOUNT);
			ThreadTransitory::UserRetry = Decimal::ToInt32(udRetry->Value);

			Profile::GetInt(SECTION_OPTION, KEY_BUFFER_SIZE,
				ThreadTransitory::UserBuffer, intval, ini);
			udBuffer->Value = std::clamp(intval, MINREADBUFFERSIZE, MAXREADBUFFERSIZE);
			ThreadTransitory::UserBuffer = Decimal::ToInt32(udBuffer->Value);

			OverwriteInfo::AddComboItem(cmbOverwrite);
			Profile::GetInt(SECTION_OPTION, KEY_OVERWRITE,
				(int)OverwriteInfo::DefaultItem, intval, ini);
			if (0 <= intval && intval < cmbOverwrite->Items->Count)
				cmbOverwrite->SelectedIndex = intval;

			OperationInfo::AddComboItem(cmbOperation);
			Profile::GetInt(SECTION_OPTION, KEY_OPERATION,
				(int)OperationInfo::DefaultItem, intval, ini);
			if (0 <= intval && intval < cmbOperation->Items->Count)
				cmbOperation->SelectedIndex = intval;

			// Second, read from command line
			{
				CCommandLineParser parser(
					CaseFlags::CaseFlags_Default,
					TO_LPCWSTR(I18N(L"Copy data from broken HDD")),
					L"retrycopy");

				parser.setStrict();

				wstring dest;
				parser.AddOption(L"-d", ArgCount::ArgCount_One, &dest,
					ArgEncodingFlags::ArgEncodingFlags_Default,
					TO_LPCWSTR(I18N(L"Destination")));

				wstring retryCount;
				parser.AddOption(L"-r", ArgCount::ArgCount_One, &retryCount,
					ArgEncodingFlags::ArgEncodingFlags_Default,
					TO_LPCWSTR(I18N(L"Retry count")));

				wstring bufferSize;
				parser.AddOption(L"-b", ArgCount::ArgCount_One, &bufferSize,
					ArgEncodingFlags::ArgEncodingFlags_Default,
					TO_LPCWSTR(I18N(L"Buffer size")));

				wstring overwrite;
				parser.AddOption(L"-ov", ArgCount::ArgCount_One, &overwrite,
					ArgEncodingFlags::ArgEncodingFlags_Default,
					TO_LPCWSTR(String::Format(I18N(L"Overwrite: One of {0}"), OverwriteInfo::ONEOFOPERATION)));

				wstring operation;
				parser.AddOption(L"-op", ArgCount::ArgCount_One, &operation,
					ArgEncodingFlags::ArgEncodingFlags_Default,
					TO_LPCWSTR(String::Format(I18N(L"Operation: One of {0}"), OperationInfo::ONEOFOPERATION)));



				bool bStart = false;
				parser.AddOption(L"-start", 0, &bStart,
					ArgEncodingFlags::ArgEncodingFlags_Default,
					TO_LPCWSTR(I18N(L"Start operation")));

				bool bShowGitRev = false;
				parser.AddOption(L"--show-gitrev", 0, &bShowGitRev,
					ArgEncodingFlags::ArgEncodingFlags_Default,
					TO_LPCWSTR(I18N(L"Show Gitrev")));

				bool bIsCloseOnFinish = false;
				parser.AddOption(L"-close", 0, &bIsCloseOnFinish,
					ArgEncodingFlags::ArgEncodingFlags_Default,
					TO_LPCWSTR(I18N(L"Close when finished")));

				bool bShowHelp = false;
				parser.AddOptionRange({ L"-h",L"--help",L"/h",L"/?" }, 0, &bShowHelp,
					ArgEncodingFlags::ArgEncodingFlags_Default,
					TO_LPCWSTR(I18N(L"Show Help")));

				bool bTestShowReadErrorDialog = false;
				constexpr wchar_t* pOpStringTestShowReadErrorDlg = L"--test-showreaderrordlg";
				parser.AddOption(pOpStringTestShowReadErrorDlg, 0, &bTestShowReadErrorDialog);

				vector<wstring> mainArgs;
				parser.AddOption(L"", ArgCount::ArgCount_ZeroToInfinite,
					&mainArgs,
					ArgEncodingFlags::ArgEncodingFlags_Default,
					TO_LPCWSTR(I18N(L"Source path")));

#ifdef _DEBUG
				bool bMockReadFile = false;
				parser.AddOption(L"--use-mockread", 0, &bMockReadFile,
					ArgEncodingFlags::ArgEncodingFlags_Default,
					L"Use MockFile for test");
#endif
				parser.Parse();

#ifdef _DEBUG
				CDebug::IsMockReadFile = bMockReadFile;
#endif
				bTestShowReadErrorDialog_ = bTestShowReadErrorDialog;
				bStart_ = bStart;
				IsCloseOnFinish = bIsCloseOnFinish;

				if (bShowHelp)
				{
					CppUtils::Info(gcnew String(parser.getHelpMessage({ pOpStringTestShowReadErrorDlg }).c_str()));
					bCloseNow_ = true;
					return;
				}
				if (bShowGitRev)
				{
					MessageBox::Show(
						gcnew String(GITREV::GetHashMessage().c_str()),
						L"Gitrev - " + Application::ProductName,
						MessageBoxButtons::OK,
						MessageBoxIcon::Information);
					bCloseNow_ = true;
					return;
				}

				// check input
				if (!parser.getUnknowOptionStrings().empty())
				{
					StringBuilder message;
					message.AppendLine(I18N(L"Unknown option(s):"));
					message.AppendLine(gcnew String(parser.getUnknowOptionStrings().c_str()));
					MessageBox::Show(message.ToString(),
						Application::ProductName,
						MessageBoxButtons::OK,
						MessageBoxIcon::Exclamation);
				}
				//if (!mainArg.empty())
				//{
				//	StringBuilder message;
				//	message.AppendLine(I18N(L"Unknown argument:"));
				//	message.AppendLine(gcnew String(mainArg.c_str()));
				//	MessageBox::Show(message.ToString(),
				//		Application::ProductName,
				//		MessageBoxButtons::OK,
				//		MessageBoxIcon::Exclamation);
				//}

				SetSourceText(mainArgs);

				txtDestination->Text = gcnew String(dest.c_str());

				if (!retryCount.empty())
				{
					if (!Int32::TryParse(gcnew String(retryCount.c_str()), intval))
					{
						CppUtils::Alert(I18N(L"-r value must be int"));
					}
					else
					{
						if (!(MINRETRYCOUNT <= intval && intval <= MAXRETRYCOUNT))
						{
							CppUtils::Alert(String::Format(I18N(L"-r value must be between {0} and {1}."),
								MINRETRYCOUNT, MAXRETRYCOUNT));
						}
						else
						{
							udRetry->Value = intval;
							ThreadTransitory::UserRetry = intval;
						}
					}
				}
				if (!bufferSize.empty())
				{
					if (!Int32::TryParse(gcnew String(bufferSize.c_str()), intval))
					{
						CppUtils::Alert(I18N(L"-b value must be int"));
					}
					else
					{
						if (!(MINREADBUFFERSIZE <= intval && intval <= MAXREADBUFFERSIZE))
						{
							CppUtils::Alert(String::Format(I18N(L"-b value must be between {0} and {1}."),
								MINREADBUFFERSIZE, MAXREADBUFFERSIZE));
						}
						else
						{
							udBuffer->Value = intval;
							ThreadTransitory::UserBuffer = intval;
						}
					}
				}
				if (!overwrite.empty())
				{
					OverwriteInfo::SetComboItemFromCL(cmbOverwrite, overwrite.c_str());
				}
				if (!operation.empty())
				{
					OperationInfo::SetComboItemFromCL(cmbOperation, operation.c_str());
				}
			}
		}
		System::Void FormMain::FormMain_Load(System::Object^ sender, System::EventArgs^ e)
		{
			if (bCloseNow_)
			{
				Close();
				return;
			}

			if (bTestShowReadErrorDialog_)
			{
				ReadFileFailedGetUserAction(
					ThreadTransitory::ThreadNumber,
					Application::ExecutablePath,
					0,
					1000,
					3,
					1,
					3,
					4096);
				Close();
				return;
			}
			if (bStart_)
				btnStart->PerformClick();
		}
		void FormMain::DoAppendLog(String^ message)
		{
			DASSERT(logForm_);
			String^ line = String::Format(L"{0}\t{1}", // + Environment::NewLine,
				DateTime::Now.ToString("yyyy-MM-dd HH.mm.ss"),
				message);
			sbLogBuffer_.AppendLine(line);
		}
		void FormMain::AppendLog(String^ message, bool bNow)
		{
			DoAppendLog(message);
			if (bNow)
				timerUpdate_Tick(nullptr, nullptr);
		}
		void FormMain::AppendLog(List<String^>^ messages, bool bNow)
		{
			for each (String ^ message in messages)
				DoAppendLog(message);
			if (bNow)
				timerUpdate_Tick(nullptr, nullptr);
		}
		System::Void FormMain::btnNavSource_Click(System::Object^ sender, System::EventArgs^ e)
		{
			System::Drawing::Point pos(
				btnNavSource->Location.X + btnNavSource->Size.Width,
				btnNavSource->Location.Y);
			ctxNavigate->Tag = btnNavSource;
			ctxNavigate->Show(this, pos.X, pos.Y);
		}
		System::Void FormMain::btnAddSource_Click(System::Object^ sender, System::EventArgs^ e)
		{
			System::Drawing::Point pos(
				btnNavSource->Location.X + btnNavSource->Size.Width,
				btnNavSource->Location.Y);
			ctxNavigate->Tag = btnAddSource;
			ctxNavigate->Show(this, pos.X, pos.Y);
		}

		System::Void FormMain::tsmiFile_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (ctxNavigate->Tag == btnNavSource)
			{
				cli::array<String^>^ files =
					RetrycopyMisc::GetMultipleFilesFromUser(I18N(L"Select source files"));
				if (!files)
					return;
				SetSourceText(files);
			}
			else if (ctxNavigate->Tag == btnAddSource)
			{
				cli::array<String^>^ files =
					RetrycopyMisc::GetMultipleFilesFromUser(I18N(L"Select source files (Add)"));
				if (!files)
					return;
				AddSourceText(files);
			}
			else if (ctxNavigate->Tag == btnNavDestination)
			{
				String^ destination = AmbLib::GetSaveFileDialog(I18N(L"Select destination file"));
				if (String::IsNullOrEmpty(destination))
					return;
				txtDestination->Text = destination;
			}
			else
				DASSERT(false);
		}

		System::Void FormMain::tsmiDirectory_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (ctxNavigate->Tag == btnNavSource)
			{
				cli::array<String^>^ dirs = RetrycopyMisc::GetMultipleFoldersFromUser(I18N(L"Select source directory"));
				if (!dirs)
					return;
				SetSourceText(dirs);
			}
			if (ctxNavigate->Tag == btnAddSource)
			{
				cli::array<String^>^ dirs = RetrycopyMisc::GetMultipleFoldersFromUser(I18N(L"Select source directory"));
				if (!dirs)
					return;
				AddSourceText(dirs);
			}
			else if (ctxNavigate->Tag == btnNavDestination)
			{
				String^ destination;
				if (!browseFolder(this, I18N(L"Select destination file"), destination))
					return;
				if (String::IsNullOrEmpty(destination))
					return;
				txtDestination->Text = destination;
			}
			else
				DASSERT(false);
		}

		System::Void FormMain::btnNavDestination_Click(System::Object^ sender, System::EventArgs^ e)
		{
			System::Drawing::Point pos(
				btnNavDestination->Location.X + btnNavDestination->Size.Width,
				btnNavDestination->Location.Y);
			ctxNavigate->Tag = btnNavDestination;
			ctxNavigate->Show(this, pos.X, pos.Y);
		}

		System::Void FormMain::cmbOverwrite_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
		{
			ThreadTransitory::UserOverWrite = (OVERWRITE_TYPE)cmbOverwrite->SelectedIndex;
		}
		System::Void FormMain::cmbOperation_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
		{
			ThreadTransitory::UserOperation = (OPERATION)cmbOperation->SelectedIndex;
		}

		System::Void FormMain::btnCopy_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (ThreadState != ThreadStateType::NONE)
			{
				if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(this,
					I18N(L"Operation is still active. Do you want to cancel it?"),
					MessageBoxDefaultButton::Button2))
				{
					return;
				}
				ThreadState = ThreadStateType::NONE;
			}
			DASSERT(ThreadState == ThreadStateType::NONE);

			ThreadDataMaster^ thData = gcnew ThreadDataMaster(
				ThreadTransitory::IncrementThreadNumber(),
				txtSource->Text, txtDestination->Text);

			theThread_ = gcnew Thread(
				gcnew ParameterizedThreadStart(this, &FormMain::StartOfThreadMaster));
			theThread_->Start(thData);
		}
		System::Void FormMain::timerUpdate_Tick(System::Object^ sender, System::EventArgs^ e)
		{
			txtTotalCount->Text = ThreadTransitory::TotalCount.ToString();
			txtProcessedCount->Text = ThreadTransitory::ProcessedTotalCount.ToString();

			txtTotalSize->Text = SizeToUser(ThreadTransitory::TotalSize);
			txtTotalProcessed->Text = SizeToUser(ThreadTransitory::ProcessedTotalSize);

			txtCurrentSize->Text = SizeToUser(ThreadTransitory::CurSize);
			txtCurrentProcessed->Text = SizeToUser(ThreadTransitory::ProcessedCurSize);

			txtProgress->Text = ThreadTransitory::ProgressDisp;
			txtLastError->Text = ThreadTransitory::LastErrorDisp;

			progressMain->Value = ThreadTransitory::TotalPercent;

			logForm_->txtLog->AppendText(sbLogBuffer_.ToString());
			sbLogBuffer_.Clear();

			//tbiigcnew->ProgressState = System::Windows::Shell::TaskbarItemProgressState::Normal;
			//tbiigcnew->ProgressValue = ThreadTransitory::TotalPercentAsDouble;

			//Microsoft::WindowsAPICodePack::Taskbar::TaskbarManager::Instance.SetProgressState(Microsoft.WindowsAPICodePack.Taskbar.TaskbarProgressBarState.NoProgress);
			//Microsoft.WindowsAPICodePack.Taskbar.TaskbarManager.Instance.SetProgressValue(ActualValue, MaxValue));

			Ambiesoft::RetrycopyMisc::SetTaskProgress(ThreadTransitory::TotalPercent);
			UpdateTitle();
		}

		void FormMain::OnRetryCountChanged(System::Object^ sender, System::EventArgs^ e)
		{
			ThreadTransitory::UserRetry = Decimal::ToInt32(udRetry->Value);
		}
		void FormMain::OnBufferSizeChanged(System::Object^ sender, System::EventArgs^ e)
		{
			ThreadTransitory::UserBuffer = Decimal::ToInt32(udBuffer->Value);
		}

		String^ FormMain::IniPath::get()
		{
			return Path::Combine(
				Path::GetDirectoryName(Application::ExecutablePath),
				Path::GetFileNameWithoutExtension(Application::ExecutablePath) + L".ini");
		}
		System::Void FormMain::FormMain_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
		{
			if (ThreadState != ThreadStateType::NONE)
			{
				ThreadStateType prevState = ThreadState;
				ThreadState = ThreadStateType::PAUSED;
				if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(this,
					I18N(L"Operation is still active. Are you sure to quit?"),
					MessageBoxDefaultButton::Button2))
				{
					ThreadState = prevState;
					e->Cancel = true;
					return;
				}
			}

			HashIni^ ini = Profile::ReadAll(IniPath);

			Profile::WriteInt(SECTION_OPTION, KEY_RETRY_COUNT,
				Decimal::ToInt32(udRetry->Value), ini);

			Profile::WriteInt(SECTION_OPTION, KEY_BUFFER_SIZE,
				Decimal::ToInt32(udBuffer->Value), ini);

			Profile::WriteInt(SECTION_OPTION, KEY_OVERWRITE, cmbOverwrite->SelectedIndex, ini);
			Profile::WriteInt(SECTION_OPTION, KEY_OPERATION, cmbOperation->SelectedIndex, ini);

			if (!Profile::WriteAll(ini, IniPath))
			{
				CppUtils::Alert(this, I18N(L"Failed to save ini."));
			}

			CWaitCursor wc;
			ThreadState = ThreadStateType::NONE;
		}
		System::Void FormMain::btnSuspend_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (threadState_ == ThreadStateType::RUNNING)
			{
				ThreadState = ThreadStateType::PAUSED;
			}
			else if (threadState_ == ThreadStateType::PAUSED)
			{
				ThreadState = ThreadStateType::RUNNING;
			}
		}

		ref class EnableBacker
		{
			Form^ f_;
			static int count_ = 0;
		public:
			EnableBacker(Form^ f) {
				++count_;
				if (count_ == 1) {
					f_ = f;
					f_->Enabled = false;
				}
			}
			~EnableBacker() {
				--count_;
				if (f_)
					f_->Enabled = true;
				GC::SuppressFinalize(this);
			}
			!EnableBacker() {
				DASSERT(false);
			}
		};
		void FormMain::ThreadState::set(ThreadStateType ts)
		{
			if (threadState_ == ts)
				return;
			EnableBacker enableBacker(this);
			switch (ts)
			{
			case ThreadStateType::NONE:
				if (threadState_ == ThreadStateType::PAUSED)
					ThreadState = ThreadStateType::RUNNING;
				if (theThread_)
				{
					ThreadTransitory::IncrementThreadNumber();
					theThread_->Abort();
					theThread_->Join();
					delete theThread_;
					theThread_ = nullptr;
					AppendLog(I18N(L"Task Ended"), true);
				}
				txtSource->ReadOnly = false;
				btnNavSource->Enabled = true;
				txtDestination->ReadOnly = false;
				btnNavDestination->Enabled = true;
				cmbOperation->Enabled = true;
				btnStart->Enabled = true;
				btnSuspend->Enabled = false;
				timerUpdate->Enabled = false;
				RetrycopyMisc::SetTaskbarStateNone();
				break;
			case ThreadStateType::RUNNING:
				DASSERT(theThread_);
				if (threadState_ == ThreadStateType::NONE)
				{
					ThreadTransitory::init();
					txtSource->ReadOnly = true;
					btnNavSource->Enabled = false;
					txtDestination->ReadOnly = true;
					btnNavDestination->Enabled = false;
					cmbOperation->Enabled = false;
					btnStart->Enabled = false;
					btnSuspend->Enabled = true;
					AppendLog(I18N(L"Task Started"), true);
				}
				else if (threadState_ == ThreadStateType::PAUSED)
				{
					theThread_->Resume();
				}
				timerUpdate->Enabled = true;
				btnSuspend->Text = I18N(L"Pause");
				RetrycopyMisc::SetTaskbarStateRunning();
				break;
			case ThreadStateType::PAUSED:
				DASSERT(theThread_);
				DASSERT(threadState_ != ThreadStateType::NONE);
				theThread_->Suspend();
				timerUpdate->Enabled = false;
				btnSuspend->Text = I18N(L"Resume");
				RetrycopyMisc::SetTaskbarStatePaused();
				break;
			}

			threadState_ = ts;
			UpdateTitle();
		}
		void FormMain::UpdateTitle()
		{
			StringBuilder title;
			switch (ThreadState)
			{
			case ThreadStateType::NONE:
				title.Append(Application::ProductName);
				break;
			case ThreadStateType::RUNNING:
			case ThreadStateType::PAUSED:
				if (ThreadState == ThreadStateType::RUNNING)
				{
					title.Append(OperationInfo::GetMainTitle(this->cmbOperation));
				}
				else
				{
					title.Append(I18N(L"Paused"));
				}
				title.Append(L" ");
				title.Append(ThreadTransitory::TotalPercent);
				title.Append(L"%");

				title.Append(L" - ");
				title.Append(Application::ProductName);
				break;
			}
			Text = title.ToString();
		}

		System::Void FormMain::ctxAbout_Opening(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e)
		{
			tsmiShowLog->Checked = logForm_ ? logForm_->Visible : false;
			tsmiAboutThisApplication->Checked = aboutForm_ ? aboutForm_->Visible : false;
		}
		System::Void FormMain::btnAbout_Click(System::Object^ sender, System::EventArgs^ e)
		{
			System::Drawing::Point pos(btnAbout->Location.X, btnAbout->Location.Y + btnAbout->Size.Height);
			ctxAbout->Show(this, pos.X, pos.Y);
		}
		System::Void FormMain::showLogToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
			if (!logForm_->Visible) {
				logForm_->Show(this);
			}
			else {
				logForm_->Visible = false;
			}
		}
		System::Void FormMain::tsmiAboutThisApplication_Click(System::Object^ sender, System::EventArgs^ e) {
			if (!aboutForm_) {
				aboutForm_ = gcnew FormAbout();
			}
			if (!aboutForm_->Visible) {
				aboutForm_->Show(this);
			}
			else {
				aboutForm_->Visible = false;
			}
		}

		List<String^>^ FormMain::GetSourceText()
		{
			List<String^>^ ret = gcnew List<String^>();
			if (String::IsNullOrEmpty(txtSource->Text))
				return ret;

			ret->AddRange(S2A(txtSource->Text));
			return ret;
		}
		void FormMain::SetSourceText(const std::vector<std::wstring>& args)
		{
			txtSource->Text = gcnew String(V2S(args));
		}
		System::Void FormMain::txtSource_TextChanged(System::Object^ sender, System::EventArgs^ e)
		{
			size_t count = 0;
			if (!String::IsNullOrEmpty(txtSource->Text))
			{
				CCommandLineString cmd(TO_LPCWSTR(txtSource->Text));
				count = cmd.getCount();
			}
			lblSource->Text = LblSrouceOrig + L" " +
				String::Format(I18N(L"{0} file(s)"), count);
		}
		void FormMain::SetSourceText(cli::array<String^>^ files)
		{
			vector<wstring> vs;
			for each (String ^ s in files)
				vs.push_back(TO_LPCWSTR(s));
			SetSourceText(vs);
		}
		void FormMain::AddSourceText(cli::array<String^>^ files)
		{
			List<String^>^ current = GetSourceText();
			current->AddRange(files);
			SetSourceText(current->ToArray());
		}
	}

}