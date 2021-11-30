#include "stdafx.h"

#include "threadData.h"
#include "helper.h"
#include "FormLog.h"
#include "FormMain.h"
using namespace System::Collections::Generic;
using namespace System::Text;

using namespace System::IO;
using namespace System::Threading;
using namespace Ambiesoft;
namespace retrycopy {

	FormMain::FormMain(String^ src, String^ dest) :
		source_(src), destination_(dest)
	{
		InitializeComponent();

		theForm_ = this;
		logForm_ = gcnew FormLog();

		int intval;
		Ambiesoft::AmbLib::MakeTripleClickTextBox(txtSource);
		Ambiesoft::AmbLib::MakeTripleClickTextBox(txtDestination);
		txtSource->Text = source_;
		txtDestination->Text = destination_;

		HashIni^ ini = Profile::ReadAll(IniPath);
		Profile::GetInt(SECTION_OPTION, KEY_RETRY_COUNT,
			ThreadTransitory::UserRetry, intval, ini);
		txtRetryCount->Text = intval.ToString();

		Profile::GetInt(SECTION_OPTION, KEY_BUFFER_SIZE,
			ThreadTransitory::UserBuffer, intval, ini);
		txtBuffer->Text = intval.ToString();

		cmbOverwrite->Items->Add(I18N(L"Yes"));
		cmbOverwrite->Items->Add(I18N(L"No"));
		cmbOverwrite->Items->Add(I18N(L"Ask"));
		Profile::GetInt(SECTION_OPTION, KEY_OVERWRITE,
			0, intval, ini);
		if (0 <= intval && intval < cmbOverwrite->Items->Count)
			cmbOverwrite->SelectedIndex = intval;



		cmbRemove->Items->Add(I18N(L"No"));
		cmbRemove->Items->Add(I18N(L"Yes Recycle"));
		cmbRemove->Items->Add(I18N(L"Yes Delete"));
		cmbRemove->Items->Add(I18N(L"Ask"));
		Profile::GetInt(SECTION_OPTION, KEY_REMOVE,
			1, intval, ini);
		if (0 <= intval && intval < cmbRemove->Items->Count)
			cmbRemove->SelectedIndex = intval;


	}
	void FormMain::AppendLog(String^ message)
	{
		DASSERT(logForm_);
		String^ line = String::Format(L"{0}\t{1}", // + Environment::NewLine,
			DateTime::Now.ToString("yyyy-MM-dd HH.mm.ss"),
			message);
		sbLogBuffer_.AppendLine(line);
	}
	void FormMain::AppendLogNow(String^ message)
	{
		AppendLog(message);
		timerUpdate_Tick(nullptr, nullptr);
	}
	System::Void FormMain::btnNavSource_Click(System::Object^ sender, System::EventArgs^ e)
	{
		String^ source = AmbLib::GetOpenFileDialog(I18N(L"Select source file"));
		if (String::IsNullOrEmpty(source))
			return;
		txtSource->Text = source;
	}
	System::Void FormMain::btnNavDestination_Click(System::Object^ sender, System::EventArgs^ e)
	{
		String^ destination = AmbLib::GetSaveFileDialog(I18N(L"Select destination file"));
		if (String::IsNullOrEmpty(destination))
			return;
		txtDestination->Text = destination;
	}

	System::Void FormMain::cmbOverwrite_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
	{
		ThreadTransitory::UserOverWrite = (OVERWRITE_TYPE)cmbOverwrite->SelectedIndex;
	}
	System::Void FormMain::cmbRemove_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
	{
		ThreadTransitory::UserRemove = (REMOVE_TYPE)cmbRemove->SelectedIndex;
	}

	System::Void FormMain::btnCopy_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (ThreadState != ThreadStateType::NONE)
		{
			if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(this,
				I18N(L"Copying is still active. Do you want to cancel it?"),
				MessageBoxDefaultButton::Button2))
			{
				return;
			}
			ThreadState = ThreadStateType::NONE;
		}
		DASSERT(ThreadState == ThreadStateType::NONE);

		if (String::IsNullOrEmpty(txtSource->Text))
		{
			CppUtils::Alert(I18N(L"Source file is empty."));
			return;
		}
		if (!File::Exists(txtSource->Text) && !Directory::Exists(txtSource->Text))
		{
			CppUtils::Alert(I18N(L"Source file or directory does not exist."));
			return;
		}

		if (String::IsNullOrEmpty(txtDestination->Text))
		{
			CppUtils::Alert(I18N(L"Destination is empty."));
			return;
		}

		if (Directory::Exists(txtSource->Text))
		{
			if (File::Exists(txtDestination->Text))
			{
				CppUtils::Alert(I18N(L"Source is directory but destination is a file."));
				return;
			}
			if (!Directory::Exists(txtDestination->Text))
			{
				if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(this, 
					String::Format(
						I18N(L"'{0}' does not exist. Do you want to create directory?"),
						txtDestination->Text),
					MessageBoxDefaultButton::Button2))
				{
					return;
				}

				Directory::CreateDirectory(txtDestination->Text);
				if (!Directory::Exists(txtDestination->Text))
				{
					CppUtils::Alert(I18N(L"Failed to create a directory."));
					return;
				}
			}
		}

		List<String^>^ dstDirs;
		KVS^ sds;
		try
		{
			sds = AmbLib::GetSourceAndDestFiles(txtSource->Text, txtDestination->Text, dstDirs);
		}
		catch (Exception^ ex)
		{
			CppUtils::Alert(this, ex);
			return;
		}

		ThreadDataMaster^ thData = gcnew ThreadDataMaster(
			Directory::Exists(txtSource->Text) ? txtSource->Text : nullptr,
			sds, dstDirs);
		theThread_ = gcnew Thread(
			gcnew ParameterizedThreadStart(this, &FormMain::StartOfThreadMaster));
		theThread_->Start(thData);
		ThreadState = ThreadStateType::RUNNING;
	}
	System::Void FormMain::timerUpdate_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		txtTotalCount->Text = ThreadTransitory::TotalCount.ToString();
		txtProcessedCount->Text = ThreadTransitory::ProcessedTotalCount.ToString();

		txtTotalSize->Text = SizeToUser(ThreadTransitory::TotalSize);
		txtTotalProcessed->Text = SizeToUser(ThreadTransitory::ProcessedTotalSize);

		txtCurrentSize->Text = SizeToUser(ThreadTransitory::CurSize);
		txtCurrentProcessed->Text = SizeToUser(ThreadTransitory::ProcessedCurSize);

		txtLog->Text = ThreadTransitory::FileLastError;

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
	System::Void FormMain::txtRetryCount_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		int v;
		if (!Int32::TryParse(txtRetryCount->Text, v))
		{
			txtRetryCount->Text = ThreadTransitory::UserRetry.ToString();
			return;
		}
		if (v < 0 && v != -1)
		{
			txtRetryCount->Text = L"-1";
			return;
		}
		ThreadTransitory::UserRetry = v;
	}
	System::Void FormMain::txtBuffer_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		int v;
		if (!Int32::TryParse(txtBuffer->Text, v) || v < 1)
		{
			txtBuffer->Text = ThreadTransitory::UserBuffer.ToString();
			return;
		}
		ThreadTransitory::UserBuffer = v;
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
			if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(this,
				I18N(L"Operation is in progress. Are you sure to quit?"),
				MessageBoxDefaultButton::Button2))
			{
				e->Cancel = true;
				return;
			}
		}

		HashIni^ ini = Profile::ReadAll(IniPath);
		int v;
		if (Int32::TryParse(txtRetryCount->Text, v))
		{
			Profile::WriteInt(SECTION_OPTION, KEY_RETRY_COUNT, v, ini);
		}
		if (Int32::TryParse(txtBuffer->Text, v))
		{
			Profile::WriteInt(SECTION_OPTION, KEY_BUFFER_SIZE, v, ini);
		}
		Profile::WriteInt(SECTION_OPTION, KEY_OVERWRITE, cmbOverwrite->SelectedIndex, ini);
		Profile::WriteInt(SECTION_OPTION, KEY_REMOVE, cmbRemove->SelectedIndex, ini);

		if (!Profile::WriteAll(ini, IniPath))
		{
			CppUtils::Alert(this, I18N(L"Failed to save ini."));
		}

		// CWaitCursor wc;
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
				theThread_->Abort();
				theThread_->Join();
				delete theThread_;
				theThread_ = nullptr;
			}
			txtSource->ReadOnly = false;
			btnNavSource->Enabled = true;
			txtDestination->ReadOnly = false;
			btnNavDestination->Enabled = true;
			btnCopy->Enabled = true;
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
				btnCopy->Enabled = false;
				btnSuspend->Enabled = true;
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
				title.Append(I18N(L"Copying"));
			else
				title.Append(I18N(L"Paused"));
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
}