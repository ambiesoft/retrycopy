#include "stdafx.h"

#include "../../lsMisc/BrowseFolder.h"

#include "threadData.h"
#include "helper.h"
#include "FormLog.h"
#include "FormAbout.h"
#include "OverwriteInfo.h"
#include "RemoveInfo.h"
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
		udBuffer->Minimum = MINREADBUFFERSIZE;
		udBuffer->Maximum = MAXREADBUFFERSIZE;
		udBuffer->TextChanged += gcnew System::EventHandler(this, &FormMain::OnBufferSizeChanged);

		udRetry->Minimum = MINRETRYCOUNT;
		udRetry->Maximum = MAXRETRYCOUNT;
		udRetry->TextChanged += gcnew System::EventHandler(this, &FormMain::OnRetryCountChanged);

		int intval;
		Ambiesoft::AmbLib::MakeTripleClickTextBox(txtSource);
		Ambiesoft::AmbLib::MakeTripleClickTextBox(txtDestination);
		txtSource->Text = source_;
		txtDestination->Text = destination_;

		HashIni^ ini = Profile::ReadAll(IniPath);
		Profile::GetInt(SECTION_OPTION, KEY_RETRY_COUNT,
			ThreadTransitory::UserRetry, intval, ini);
		udRetry->Value = std::clamp(intval, MINRETRYCOUNT, MAXRETRYCOUNT);

		Profile::GetInt(SECTION_OPTION, KEY_BUFFER_SIZE,
			ThreadTransitory::UserBuffer, intval, ini);
		udBuffer->Value = std::clamp(intval, MINREADBUFFERSIZE, MAXREADBUFFERSIZE);

		OverwriteItem::AddComboItem(cmbOverwrite);
		Profile::GetInt(SECTION_OPTION, KEY_OVERWRITE,
			(int)OverwriteItem::DefaultItem, intval, ini);
		if (0 <= intval && intval < cmbOverwrite->Items->Count)
			cmbOverwrite->SelectedIndex = intval;


		RemoveInfo::AddComboItem(cmbRemove);
		Profile::GetInt(SECTION_OPTION, KEY_REMOVE,
			(int)RemoveInfo::DefaultItem, intval, ini);
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
		System::Drawing::Point pos(
			btnNavSource->Location.X + btnNavSource->Size.Width,
			btnNavSource->Location.Y);
		ctxNavigate->Tag = btnNavSource;
		ctxNavigate->Show(this, pos.X, pos.Y);
	}
	System::Void FormMain::tsmiFile_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (ctxNavigate->Tag == btnNavSource)
		{
			String^ source = AmbLib::GetOpenFileDialog(I18N(L"Select source file"));
			if (String::IsNullOrEmpty(source))
				return;
			txtSource->Text = source;
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
			String^ source;
			if (!browseFolder(this, I18N(L"Select source directory"), source))
				return;
			if (String::IsNullOrEmpty(source))
				return;
			txtSource->Text = source;
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
				I18N(L"Operation is in progress. Are you sure to quit?"),
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
		Profile::WriteInt(SECTION_OPTION, KEY_REMOVE, cmbRemove->SelectedIndex, ini);

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

}

