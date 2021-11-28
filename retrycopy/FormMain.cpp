#include "stdafx.h"

#include "threadData.h"
#include "helper.h"
#include "FormLog.h"
#include "FormMain.h"

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
		String^ line = String::Format(L"{0}\t{1}" + Environment::NewLine,
			DateTime::Now.ToString("yyyy-MM-dd HH.mm.ss"),
			message);
		logForm_->txtLog->AppendText(line);
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
		if (HasThread)
		{
			if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(this,
				I18N(L"Copying is still active. Do you want to cancel it?"),
				MessageBoxDefaultButton::Button2))
			{
				return;
			}
			ThreadState = ThreadStateType::NONE;
		}
		DASSERT(!HasThread);

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

		//String^ fileToWrite = txtDestination->Text;
		//if (Directory::Exists(fileToWrite))
		//{
		//	fileToWrite = Path::Combine(fileToWrite,
		//		Path::GetFileName(txtSource->Text));
		//	if (File::Exists(fileToWrite))
		//	{
		//		if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(this,
		//			String::Format(I18N(L"'{0}' is already exists. Do you want to overwrite?"),fileToWrite),
		//			MessageBoxDefaultButton::Button2))
		//		{
		//			return;
		//		}
		//	}
		//}

		KVS^ sds = Ambiesoft::AmbLib::GetSourceAndDestFiles(txtSource->Text, txtDestination->Text);
		ThreadDataMaster^ thData = gcnew ThreadDataMaster(sds);
		theThread_ = gcnew Thread(
			gcnew ParameterizedThreadStart(this, &FormMain::StartOfThreadMaster));
		theThread_->Start(thData);
		ThreadState = ThreadStateType::RUNNING;
		timerUpdate->Enabled = true;
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
		if (HasThread)
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

		if (!Profile::WriteAll(ini,IniPath))
		{
			CppUtils::Alert(this, I18N(L"Failed to save ini."));
		}

		//CWaitCursor wc;
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
	void FormMain::ThreadState::set(ThreadStateType ts)
	{
		if (threadState_ == ts)
			return;

		switch (ts)
		{
		case ThreadStateType::NONE:
			timerUpdate->Enabled = false;
			if (threadState_ == ThreadStateType::PAUSED)
				ThreadState = ThreadStateType::RUNNING;
			if (theThread_)
			{
				theThread_->Abort();
				theThread_->Join();
				delete theThread_;
				theThread_ = nullptr;
			}
			txtSource->Enabled = true;
			btnNavSource->Enabled = true;
			txtDestination->Enabled = true;
			btnNavDestination->Enabled = true;
			btnCopy->Enabled = true;
			btnSuspend->Enabled = false;
			break;
		case ThreadStateType::RUNNING:
			DASSERT(theThread_);
			if (threadState_ == ThreadStateType::NONE)
			{
				ThreadTransitory::init();
				txtSource->Enabled = false;
				btnNavSource->Enabled = false;
				txtDestination->Enabled = false;
				btnNavDestination->Enabled = false;
				btnCopy->Enabled = false;
				btnSuspend->Enabled = true;
			}
			else if (threadState_ == ThreadStateType::PAUSED)
			{
				theThread_->Resume();
			}
			btnSuspend->Text = I18N(L"Pause");
			break;
		case ThreadStateType::PAUSED:
			DASSERT(theThread_);
			DASSERT(threadState_ != ThreadStateType::NONE);
			theThread_->Suspend();
			btnSuspend->Text = I18N(L"Resume");
			break;
		}
		
		threadState_ = ts;
	}
}