#include "stdafx.h"

#include "FormMain.h"

#pragma comment(lib, "User32.lib")

using namespace System::Text;

using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
namespace retrycopy {

	ref class ReadFailData
	{
	public:
		enum class ACTION {
			NONE,
			RETRY,
			CANCEL,
			IGNOREALL,
		};
	private:
		ACTION action_;
	public:
		ReadFailData(ACTION action) : action_(action){}
		property bool IsRetry
		{
			bool get() { return action_ == ACTION::RETRY; }
		}
		property bool IsCancel
		{
			bool get() { return action_ == ACTION::CANCEL; }
		}
		property bool IsIgnoreAll
		{
			bool get() { return action_ == ACTION::IGNOREALL; }
		}
	};
	void FormMain::StartOfThread(Object^ obj)
	{
		ThreadData^ thData = (ThreadData^)obj;
		EndInvoke(BeginInvoke(gcnew VVDelegate(this, &FormMain::ThreadStarted)));
		StartOfThread2(thData);
		BeginInvoke(gcnew VDataDelegate(this, &FormMain::ThreadFinished), thData);
	}
	void FormMain::StartOfThread2(ThreadData^ thData)
	{
		thData->allProcessed_ = 0;
		BYTE b;
		bool ignoreAllFail = false;
		do {
			LARGE_INTEGER li;
			li.QuadPart = thData->allProcessed_;
			if (0 == SetFilePointerEx(thData->HSrc, li, NULL, FILE_BEGIN))
			{
				CppUtils::Alert("fp");
				return;
			}

			DWORD dwRead;
			if (!ReadFile(thData->HSrc,
				&b,
				1,
				&dwRead,
				NULL))
			{
				DWORD le = GetLastError();
				if (!ignoreAllFail)
				{
					// fail
					ReadFailData^ rfd = (ReadFailData^)
						EndInvoke(BeginInvoke(gcnew RDLLDelegate(this, &FormMain::ReadFileFailed), 
							thData->allProcessed_));
					if (rfd->IsCancel)
						return;
					if (rfd->IsRetry)
						continue;
					if (rfd->IsIgnoreAll)
						ignoreAllFail = true;
				}
				EndInvoke(BeginInvoke(gcnew VLLDelegate(this, &FormMain::ProgressFailed),
					thData->allProcessed_));
				b = 0;
				dwRead = 1;
			}
			if (dwRead == 0)
			{
				thData->SetOK();
				// finished
				return;
			}

			DWORD dwWritten;
			if (!(WriteFile(thData->HDst,
				&b,
				1,
				&dwWritten,
				NULL) && dwWritten == 1))
			{
				CppUtils::Alert("w");
				continue;
			}
			++thData->allProcessed_;

			if ((thData->allProcessed_ % 10000) == 0)
			{
				BeginInvoke(gcnew VLLDelegate(this, &FormMain::ProcessProgressed), 
					thData->allProcessed_);
			}
		} while (true);
	}
	void FormMain::ClearThread()
	{
		if (hSource_ != NULL && hSource_ != INVALID_HANDLE_VALUE)
			CloseHandle(hSource_);
		if (hDestination_ != NULL && hDestination_ != INVALID_HANDLE_VALUE)
			CloseHandle(hDestination_);

		if (theThread_)
		{
			theThread_->Abort();
			theThread_->Join();
		}

	}
#define TO_LPCWSTR(s) (getStdWstring(s).c_str())
	std::wstring getStdWstring(String^ s)
	{
		std::wstring ret;
		if (s == nullptr)
			return ret;

		pin_ptr<const wchar_t> p = PtrToStringChars(s);
		ret = p;
		return ret;
	}
	System::Void FormMain::btnCopy_Click(System::Object^ sender, System::EventArgs^ e)
	{
		ClearThread();
		if (String::IsNullOrEmpty(txtSource->Text))
		{
			CppUtils::Alert(I18N(L"Source file is empty."));
			return;
		}
		if (!File::Exists(txtSource->Text))
		{
			CppUtils::Alert(I18N(L"Source file does not exist."));
			return;
		}

		hSource_ = CreateFile(
			TO_LPCWSTR(txtSource->Text),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | 
			// FILE_FLAG_NO_BUFFERING|
			FILE_FLAG_RANDOM_ACCESS |
			0,
			NULL);
		if (hSource_ == INVALID_HANDLE_VALUE)
		{
			CppUtils::Alert(I18N(L"Failed to open source file."));
			return;
		}
		hDestination_ = CreateFile(
			TO_LPCWSTR(txtDestination->Text),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hDestination_ == INVALID_HANDLE_VALUE)
		{
			CppUtils::Alert(I18N(L"Failed to open destination file."));
			return;
		}
		LARGE_INTEGER li;
		if (!GetFileSizeEx(hSource_, &li))
		{
			CppUtils::Alert(I18N(L"Failed get file size of source file."));
			return;
		}
		ThreadData^ thData = gcnew ThreadData(hSource_, hDestination_, li.QuadPart);
		theThread_ = gcnew Thread(
			gcnew ParameterizedThreadStart(this, &FormMain::StartOfThread));
		theThread_->Start(thData);
	}
	void FormMain::ThreadStarted()
	{
		//processSuspeded_ = false;
		//processTerminatedDuetoAppClose_ = false;

		//CurrentTaskState = TaskState::Encoding;

		//dwBackPriority_ = GetPriorityClass(GetCurrentProcess());
		//if (tsmiPriorityBackground->Checked)
		//	SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);
	}
	void FormMain::ThreadFinished(Object^ obj)
	{
		ThreadData^ thData = (ThreadData^)obj;
		ClearThread();
		if (thData->IsOK)
		{
			CppUtils::Info(this, String::Format(
				I18N(L"success srcSize={0}, Written size={1}"),
				thData->SrcSize, thData->WrittenSize));
		}
		else
		{
			CppUtils::Fatal("failed");
		}
	}

	void FormMain::ProcessProgressed(LONGLONG pos)
	{
		txtLog->AppendText(String::Format(I18N(L"{0} bytes done."), pos));
		txtLog->AppendText(Environment::NewLine);
	}
	void FormMain::ProgressFailed(LONGLONG pos)
	{
		txtLog->AppendText(String::Format(I18N(L"Write 0 at {0}."), pos));
		txtLog->AppendText(Environment::NewLine);
	}
	Object^ FormMain::ReadFileFailed(LONGLONG pos)
	{
		txtLog->AppendText(String::Format(I18N(L"Failed to ReadFile at {0}"), pos));
		txtLog->AppendText(Environment::NewLine);

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
}