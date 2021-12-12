#pragma once

#include "Ref.h"
namespace Ambiesoft {
	namespace retrycopy {

		using namespace System;
		using namespace System::ComponentModel;
		using namespace System::Collections;
		using namespace System::Windows::Forms;
		using namespace System::Data;
		using namespace System::Drawing;



		/// <summary>
		/// Summary for FormMain
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class FormMain : public System::Windows::Forms::Form
		{
		private:
			static initonly String^ SECTION_OPTION = "Option";
			static initonly String^ KEY_RETRY_COUNT = "RetryCount";
			static initonly String^ KEY_BUFFER_SIZE = "BufferSize";
			static initonly String^ KEY_OVERWRITE = "Overwrite";
			static initonly String^ KEY_OPERATION = "Operation";
			static property String^ IniPath
			{
				String^ get();
			}
		private:

			initonly FormLog^ logForm_;
			FormAbout^ aboutForm_;
			System::Text::StringBuilder sbLogBuffer_;
			property bool IsCloseOnFinish;
			initonly String^ lblSourceOrig_;
			property String^ LblSrouceOrig
			{
				String^ get() { return lblSourceOrig_; }
			}
		private: System::Windows::Forms::Label^ label1;
		private: System::Windows::Forms::Label^ lblBuffer;
		private: System::Windows::Forms::Label^ label2;
		private: System::Windows::Forms::TextBox^ txtCurSrc;
		private: System::Windows::Forms::Label^ label3;
		private: System::Windows::Forms::TextBox^ txtCurDst;
		private: System::Windows::Forms::Label^ label4;


		private: System::Windows::Forms::Label^ label6;
		private: System::Windows::Forms::TextBox^ txtTotalSize;
		private: System::Windows::Forms::Label^ label7;
		private: System::Windows::Forms::TextBox^ txtTotalProcessed;
		private: System::Windows::Forms::Label^ label8;
		private: System::Windows::Forms::Label^ label9;
		private: System::Windows::Forms::TextBox^ txtCurrentSize;
		private: System::Windows::Forms::TextBox^ txtCurrentProcessed;
		private: System::Windows::Forms::Label^ label10;
		private: System::Windows::Forms::Label^ label11;
		private: System::Windows::Forms::TextBox^ txtTotalCount;
		private: System::Windows::Forms::TextBox^ txtProcessedCount;
		private: System::Windows::Forms::Timer^ timerUpdate;
		private: System::Windows::Forms::Label^ label12;

		private: System::Windows::Forms::Button^ btnSuspend;
		private: System::Windows::Forms::SplitContainer^ spTransitory;
		private: System::Windows::Forms::ProgressBar^ progressMain;
		private: System::Windows::Forms::Button^ btnAbout;
		private: System::Windows::Forms::ContextMenuStrip^ ctxAbout;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiShowLog;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiAboutThisApplication;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiHelp;





		private: System::Windows::Forms::Button^ btnClose;
		private: System::Windows::Forms::ContextMenuStrip^ ctxNavigate;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiFile;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiDirectory;
		private: System::Windows::Forms::NumericUpDown^ udBuffer;
		private: System::Windows::Forms::NumericUpDown^ udRetry;
		private: System::Windows::Forms::TextBox^ txtLastError;

		private: System::Windows::Forms::Label^ label13;
		private: System::Windows::Forms::ComboBox^ cmbOperation;
		private: System::Windows::Forms::Button^ btnAddSource;
		private: System::Windows::Forms::ComboBox^ cmbOverwrite;


		public:
			FormMain();
			static FormMain^ theForm_;
			bool bCloseNow_ = false;

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~FormMain()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::Label^ lblSource;
		protected:
		private: System::Windows::Forms::Label^ lblDest;
		private: System::Windows::Forms::TextBox^ txtSource;
		private: System::Windows::Forms::TextBox^ txtDestination;
		private: System::Windows::Forms::Button^ btnNavSource;
		private: System::Windows::Forms::Button^ btnNavDestination;
		private: System::Windows::Forms::Button^ btnStart;

		private: System::Windows::Forms::TextBox^ txtProgress;

		private: System::ComponentModel::IContainer^ components;

		protected:


		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
			void InitializeComponent(void)
			{
				this->components = (gcnew System::ComponentModel::Container());
				System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(FormMain::typeid));
				this->spTransitory = (gcnew System::Windows::Forms::SplitContainer());
				this->label10 = (gcnew System::Windows::Forms::Label());
				this->label6 = (gcnew System::Windows::Forms::Label());
				this->label8 = (gcnew System::Windows::Forms::Label());
				this->txtTotalSize = (gcnew System::Windows::Forms::TextBox());
				this->txtCurrentSize = (gcnew System::Windows::Forms::TextBox());
				this->txtTotalCount = (gcnew System::Windows::Forms::TextBox());
				this->label11 = (gcnew System::Windows::Forms::Label());
				this->label7 = (gcnew System::Windows::Forms::Label());
				this->label9 = (gcnew System::Windows::Forms::Label());
				this->txtCurrentProcessed = (gcnew System::Windows::Forms::TextBox());
				this->txtTotalProcessed = (gcnew System::Windows::Forms::TextBox());
				this->txtProcessedCount = (gcnew System::Windows::Forms::TextBox());
				this->lblSource = (gcnew System::Windows::Forms::Label());
				this->lblDest = (gcnew System::Windows::Forms::Label());
				this->txtSource = (gcnew System::Windows::Forms::TextBox());
				this->txtDestination = (gcnew System::Windows::Forms::TextBox());
				this->btnNavSource = (gcnew System::Windows::Forms::Button());
				this->btnNavDestination = (gcnew System::Windows::Forms::Button());
				this->btnStart = (gcnew System::Windows::Forms::Button());
				this->txtProgress = (gcnew System::Windows::Forms::TextBox());
				this->label1 = (gcnew System::Windows::Forms::Label());
				this->lblBuffer = (gcnew System::Windows::Forms::Label());
				this->label2 = (gcnew System::Windows::Forms::Label());
				this->txtCurSrc = (gcnew System::Windows::Forms::TextBox());
				this->label3 = (gcnew System::Windows::Forms::Label());
				this->txtCurDst = (gcnew System::Windows::Forms::TextBox());
				this->label4 = (gcnew System::Windows::Forms::Label());
				this->cmbOverwrite = (gcnew System::Windows::Forms::ComboBox());
				this->timerUpdate = (gcnew System::Windows::Forms::Timer(this->components));
				this->label12 = (gcnew System::Windows::Forms::Label());
				this->btnSuspend = (gcnew System::Windows::Forms::Button());
				this->progressMain = (gcnew System::Windows::Forms::ProgressBar());
				this->btnAbout = (gcnew System::Windows::Forms::Button());
				this->ctxAbout = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
				this->tsmiShowLog = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->tsmiAboutThisApplication = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->tsmiHelp = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->btnClose = (gcnew System::Windows::Forms::Button());
				this->ctxNavigate = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
				this->tsmiFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->tsmiDirectory = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->udBuffer = (gcnew System::Windows::Forms::NumericUpDown());
				this->udRetry = (gcnew System::Windows::Forms::NumericUpDown());
				this->txtLastError = (gcnew System::Windows::Forms::TextBox());
				this->label13 = (gcnew System::Windows::Forms::Label());
				this->cmbOperation = (gcnew System::Windows::Forms::ComboBox());
				this->btnAddSource = (gcnew System::Windows::Forms::Button());
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->spTransitory))->BeginInit();
				this->spTransitory->Panel1->SuspendLayout();
				this->spTransitory->Panel2->SuspendLayout();
				this->spTransitory->SuspendLayout();
				this->ctxAbout->SuspendLayout();
				this->ctxNavigate->SuspendLayout();
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->udBuffer))->BeginInit();
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->udRetry))->BeginInit();
				this->SuspendLayout();
				// 
				// spTransitory
				// 
				resources->ApplyResources(this->spTransitory, L"spTransitory");
				this->spTransitory->Name = L"spTransitory";
				// 
				// spTransitory.Panel1
				// 
				this->spTransitory->Panel1->Controls->Add(this->label10);
				this->spTransitory->Panel1->Controls->Add(this->label6);
				this->spTransitory->Panel1->Controls->Add(this->label8);
				this->spTransitory->Panel1->Controls->Add(this->txtTotalSize);
				this->spTransitory->Panel1->Controls->Add(this->txtCurrentSize);
				this->spTransitory->Panel1->Controls->Add(this->txtTotalCount);
				// 
				// spTransitory.Panel2
				// 
				this->spTransitory->Panel2->Controls->Add(this->label11);
				this->spTransitory->Panel2->Controls->Add(this->label7);
				this->spTransitory->Panel2->Controls->Add(this->label9);
				this->spTransitory->Panel2->Controls->Add(this->txtCurrentProcessed);
				this->spTransitory->Panel2->Controls->Add(this->txtTotalProcessed);
				this->spTransitory->Panel2->Controls->Add(this->txtProcessedCount);
				// 
				// label10
				// 
				resources->ApplyResources(this->label10, L"label10");
				this->label10->Name = L"label10";
				// 
				// label6
				// 
				resources->ApplyResources(this->label6, L"label6");
				this->label6->Name = L"label6";
				// 
				// label8
				// 
				resources->ApplyResources(this->label8, L"label8");
				this->label8->Name = L"label8";
				// 
				// txtTotalSize
				// 
				resources->ApplyResources(this->txtTotalSize, L"txtTotalSize");
				this->txtTotalSize->Name = L"txtTotalSize";
				this->txtTotalSize->ReadOnly = true;
				// 
				// txtCurrentSize
				// 
				resources->ApplyResources(this->txtCurrentSize, L"txtCurrentSize");
				this->txtCurrentSize->Name = L"txtCurrentSize";
				this->txtCurrentSize->ReadOnly = true;
				// 
				// txtTotalCount
				// 
				resources->ApplyResources(this->txtTotalCount, L"txtTotalCount");
				this->txtTotalCount->Name = L"txtTotalCount";
				this->txtTotalCount->ReadOnly = true;
				// 
				// label11
				// 
				resources->ApplyResources(this->label11, L"label11");
				this->label11->Name = L"label11";
				// 
				// label7
				// 
				resources->ApplyResources(this->label7, L"label7");
				this->label7->Name = L"label7";
				// 
				// label9
				// 
				resources->ApplyResources(this->label9, L"label9");
				this->label9->Name = L"label9";
				// 
				// txtCurrentProcessed
				// 
				resources->ApplyResources(this->txtCurrentProcessed, L"txtCurrentProcessed");
				this->txtCurrentProcessed->Name = L"txtCurrentProcessed";
				this->txtCurrentProcessed->ReadOnly = true;
				// 
				// txtTotalProcessed
				// 
				resources->ApplyResources(this->txtTotalProcessed, L"txtTotalProcessed");
				this->txtTotalProcessed->Name = L"txtTotalProcessed";
				this->txtTotalProcessed->ReadOnly = true;
				// 
				// txtProcessedCount
				// 
				resources->ApplyResources(this->txtProcessedCount, L"txtProcessedCount");
				this->txtProcessedCount->Name = L"txtProcessedCount";
				this->txtProcessedCount->ReadOnly = true;
				// 
				// lblSource
				// 
				resources->ApplyResources(this->lblSource, L"lblSource");
				this->lblSource->Name = L"lblSource";
				// 
				// lblDest
				// 
				resources->ApplyResources(this->lblDest, L"lblDest");
				this->lblDest->Name = L"lblDest";
				// 
				// txtSource
				// 
				resources->ApplyResources(this->txtSource, L"txtSource");
				this->txtSource->Name = L"txtSource";
				this->txtSource->TextChanged += gcnew System::EventHandler(this, &FormMain::txtSource_TextChanged);
				// 
				// txtDestination
				// 
				resources->ApplyResources(this->txtDestination, L"txtDestination");
				this->txtDestination->Name = L"txtDestination";
				// 
				// btnNavSource
				// 
				resources->ApplyResources(this->btnNavSource, L"btnNavSource");
				this->btnNavSource->Name = L"btnNavSource";
				this->btnNavSource->UseVisualStyleBackColor = true;
				this->btnNavSource->Click += gcnew System::EventHandler(this, &FormMain::btnNavSource_Click);
				// 
				// btnNavDestination
				// 
				resources->ApplyResources(this->btnNavDestination, L"btnNavDestination");
				this->btnNavDestination->Name = L"btnNavDestination";
				this->btnNavDestination->UseVisualStyleBackColor = true;
				this->btnNavDestination->Click += gcnew System::EventHandler(this, &FormMain::btnNavDestination_Click);
				// 
				// btnStart
				// 
				resources->ApplyResources(this->btnStart, L"btnStart");
				this->btnStart->Name = L"btnStart";
				this->btnStart->UseVisualStyleBackColor = true;
				this->btnStart->Click += gcnew System::EventHandler(this, &FormMain::btnCopy_Click);
				// 
				// txtProgress
				// 
				resources->ApplyResources(this->txtProgress, L"txtProgress");
				this->txtProgress->Name = L"txtProgress";
				this->txtProgress->ReadOnly = true;
				// 
				// label1
				// 
				resources->ApplyResources(this->label1, L"label1");
				this->label1->Name = L"label1";
				// 
				// lblBuffer
				// 
				resources->ApplyResources(this->lblBuffer, L"lblBuffer");
				this->lblBuffer->Name = L"lblBuffer";
				// 
				// label2
				// 
				resources->ApplyResources(this->label2, L"label2");
				this->label2->Name = L"label2";
				// 
				// txtCurSrc
				// 
				resources->ApplyResources(this->txtCurSrc, L"txtCurSrc");
				this->txtCurSrc->Name = L"txtCurSrc";
				this->txtCurSrc->ReadOnly = true;
				// 
				// label3
				// 
				resources->ApplyResources(this->label3, L"label3");
				this->label3->Name = L"label3";
				// 
				// txtCurDst
				// 
				resources->ApplyResources(this->txtCurDst, L"txtCurDst");
				this->txtCurDst->Name = L"txtCurDst";
				this->txtCurDst->ReadOnly = true;
				// 
				// label4
				// 
				resources->ApplyResources(this->label4, L"label4");
				this->label4->Name = L"label4";
				// 
				// cmbOverwrite
				// 
				resources->ApplyResources(this->cmbOverwrite, L"cmbOverwrite");
				this->cmbOverwrite->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
				this->cmbOverwrite->FormattingEnabled = true;
				this->cmbOverwrite->Name = L"cmbOverwrite";
				this->cmbOverwrite->SelectedIndexChanged += gcnew System::EventHandler(this, &FormMain::cmbOverwrite_SelectedIndexChanged);
				// 
				// timerUpdate
				// 
				this->timerUpdate->Interval = 500;
				this->timerUpdate->Tick += gcnew System::EventHandler(this, &FormMain::timerUpdate_Tick);
				// 
				// label12
				// 
				resources->ApplyResources(this->label12, L"label12");
				this->label12->Name = L"label12";
				// 
				// btnSuspend
				// 
				resources->ApplyResources(this->btnSuspend, L"btnSuspend");
				this->btnSuspend->Name = L"btnSuspend";
				this->btnSuspend->UseVisualStyleBackColor = true;
				this->btnSuspend->Click += gcnew System::EventHandler(this, &FormMain::btnSuspend_Click);
				// 
				// progressMain
				// 
				resources->ApplyResources(this->progressMain, L"progressMain");
				this->progressMain->Name = L"progressMain";
				// 
				// btnAbout
				// 
				resources->ApplyResources(this->btnAbout, L"btnAbout");
				this->btnAbout->Name = L"btnAbout";
				this->btnAbout->UseVisualStyleBackColor = true;
				this->btnAbout->Click += gcnew System::EventHandler(this, &FormMain::btnAbout_Click);
				// 
				// ctxAbout
				// 
				this->ctxAbout->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
					this->tsmiShowLog, this->tsmiAboutThisApplication,
						this->tsmiHelp
				});
				this->ctxAbout->Name = L"ctxAbout";
				resources->ApplyResources(this->ctxAbout, L"ctxAbout");
				this->ctxAbout->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &FormMain::ctxAbout_Opening);
				// 
				// tsmiShowLog
				// 
				this->tsmiShowLog->Name = L"tsmiShowLog";
				resources->ApplyResources(this->tsmiShowLog, L"tsmiShowLog");
				this->tsmiShowLog->Click += gcnew System::EventHandler(this, &FormMain::showLogToolStripMenuItem_Click);
				// 
				// tsmiAboutThisApplication
				// 
				this->tsmiAboutThisApplication->Name = L"tsmiAboutThisApplication";
				resources->ApplyResources(this->tsmiAboutThisApplication, L"tsmiAboutThisApplication");
				this->tsmiAboutThisApplication->Click += gcnew System::EventHandler(this, &FormMain::tsmiAboutThisApplication_Click);
				// 
				// tsmiHelp
				// 
				this->tsmiHelp->Name = L"tsmiHelp";
				resources->ApplyResources(this->tsmiHelp, L"tsmiHelp");
				// 
				// btnClose
				// 
				resources->ApplyResources(this->btnClose, L"btnClose");
				this->btnClose->Name = L"btnClose";
				this->btnClose->UseVisualStyleBackColor = true;
				this->btnClose->Click += gcnew System::EventHandler(this, &FormMain::btnClose_Click);
				// 
				// ctxNavigate
				// 
				this->ctxNavigate->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->tsmiFile, this->tsmiDirectory });
				this->ctxNavigate->Name = L"ctxNavigate";
				resources->ApplyResources(this->ctxNavigate, L"ctxNavigate");
				// 
				// tsmiFile
				// 
				this->tsmiFile->Name = L"tsmiFile";
				resources->ApplyResources(this->tsmiFile, L"tsmiFile");
				this->tsmiFile->Click += gcnew System::EventHandler(this, &FormMain::tsmiFile_Click);
				// 
				// tsmiDirectory
				// 
				this->tsmiDirectory->Name = L"tsmiDirectory";
				resources->ApplyResources(this->tsmiDirectory, L"tsmiDirectory");
				this->tsmiDirectory->Click += gcnew System::EventHandler(this, &FormMain::tsmiDirectory_Click);
				// 
				// udBuffer
				// 
				resources->ApplyResources(this->udBuffer, L"udBuffer");
				this->udBuffer->Name = L"udBuffer";
				// 
				// udRetry
				// 
				resources->ApplyResources(this->udRetry, L"udRetry");
				this->udRetry->Name = L"udRetry";
				// 
				// txtLastError
				// 
				resources->ApplyResources(this->txtLastError, L"txtLastError");
				this->txtLastError->Name = L"txtLastError";
				this->txtLastError->ReadOnly = true;
				// 
				// label13
				// 
				resources->ApplyResources(this->label13, L"label13");
				this->label13->Name = L"label13";
				// 
				// cmbOperation
				// 
				this->cmbOperation->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
				this->cmbOperation->FormattingEnabled = true;
				resources->ApplyResources(this->cmbOperation, L"cmbOperation");
				this->cmbOperation->Name = L"cmbOperation";
				this->cmbOperation->SelectedIndexChanged += gcnew System::EventHandler(this, &FormMain::cmbOperation_SelectedIndexChanged);
				// 
				// btnAddSource
				// 
				resources->ApplyResources(this->btnAddSource, L"btnAddSource");
				this->btnAddSource->Name = L"btnAddSource";
				this->btnAddSource->UseVisualStyleBackColor = true;
				this->btnAddSource->Click += gcnew System::EventHandler(this, &FormMain::btnAddSource_Click);
				// 
				// FormMain
				// 
				resources->ApplyResources(this, L"$this");
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->Controls->Add(this->btnAddSource);
				this->Controls->Add(this->udRetry);
				this->Controls->Add(this->udBuffer);
				this->Controls->Add(this->btnClose);
				this->Controls->Add(this->btnAbout);
				this->Controls->Add(this->progressMain);
				this->Controls->Add(this->spTransitory);
				this->Controls->Add(this->btnSuspend);
				this->Controls->Add(this->label13);
				this->Controls->Add(this->label12);
				this->Controls->Add(this->cmbOperation);
				this->Controls->Add(this->cmbOverwrite);
				this->Controls->Add(this->lblBuffer);
				this->Controls->Add(this->txtLastError);
				this->Controls->Add(this->label1);
				this->Controls->Add(this->txtProgress);
				this->Controls->Add(this->btnStart);
				this->Controls->Add(this->btnNavDestination);
				this->Controls->Add(this->btnNavSource);
				this->Controls->Add(this->txtCurDst);
				this->Controls->Add(this->txtDestination);
				this->Controls->Add(this->txtCurSrc);
				this->Controls->Add(this->txtSource);
				this->Controls->Add(this->label4);
				this->Controls->Add(this->label3);
				this->Controls->Add(this->lblDest);
				this->Controls->Add(this->label2);
				this->Controls->Add(this->lblSource);
				this->Name = L"FormMain";
				this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Show;
				this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormMain::FormMain_FormClosing);
				this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
				this->spTransitory->Panel1->ResumeLayout(false);
				this->spTransitory->Panel1->PerformLayout();
				this->spTransitory->Panel2->ResumeLayout(false);
				this->spTransitory->Panel2->PerformLayout();
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->spTransitory))->EndInit();
				this->spTransitory->ResumeLayout(false);
				this->ctxAbout->ResumeLayout(false);
				this->ctxNavigate->ResumeLayout(false);
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->udBuffer))->EndInit();
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->udRetry))->EndInit();
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
			System::Threading::Thread^ theThread_;
			//HANDLE hSource_;
			//HANDLE hDestination_;
			void StartOfThreadMaster(Object^ obj);
			void StartOfThreadMaster2(ThreadDataMaster^);
			void StartOfThreadFile(ThreadDataFile^ thData);
			void ThreadStarted(int tn);
			bool OnThreadYesNo(int tn, String^ question);
			void OnThreadError(int tn, String^ error);
			void ThreadFileStarted(ThreadDataFile^ thData);
			void ThreadFileEnded(ThreadDataFile^ thData);
			bool OpenFileFailedGetUserAction(int tn, DWORD le);
			UserResponceOfFail^ ReadFileFailedGetUserAction(
				int tn,
				String^ file,
				LONGLONG pos,
				LONGLONG allSize,
				DWORD le,
				int retried,
				int retryCount,
				int bufferSize);
			UserResponceOfFail^ SFPFailedGetUserAction(int tn, String^ file, LONGLONG pos, LONGLONG allSize, DWORD le, int retried);
			void ProgressWriteWithZero(int tn, LONGLONG pos, int bufferSize);
			void ThreadFinished(ThreadDataMaster^ thData);
			void ThreadPathFinished(ThreadDataPath^ thPath);
			void ThreadLog(String^ logMessage) {
				AppendLog(logMessage);
			}
			void AppendLog(String^ message, bool bNow);
			void AppendLog(String^ message) {
				AppendLog(message, false);
			}
			void AppendLog(System::Collections::Generic::List<String^>^ messages, bool bNow);
			void AppendLog(System::Collections::Generic::List<String^>^ messages) {
				AppendLog(messages, false);
			}
			void DoAppendLog(String^ message);

			enum class ThreadStateType {
				NONE,
				RUNNING,
				PAUSED,
			} threadState_;
			property ThreadStateType ThreadState
			{
				ThreadStateType get() { return threadState_; }
				void set(ThreadStateType ts);
			}
			void UpdateTitle();

			static bool appClosing_ = false;
			initonly bool bTestShowReadErrorDialog_ = false;
			initonly bool bStart_ = false;

			System::Collections::Generic::List<String^>^ GetSourceText();
			void SetSourceText(const std::vector<std::wstring>& args);
			void SetSourceText(cli::array<String^>^ files);
			void SetSourceText(String^ file) {
				SetSourceText(gcnew cli::array<String^>{file});
			}
			void AddSourceText(cli::array<String^>^ files);
			void AddSourceText(String^ file) {
				AddSourceText(gcnew cli::array<String^>{file});
			}
		internal:
			static property bool AppClosing
			{
				bool get() { return appClosing_; }
			}

			bool AskOverwrite(int tn, String^ fileTobeOverwritten);
			Object^ EndInvokeWithTN(int tn, IAsyncResult^);
			Object^ EndInvoke(IAsyncResult^) new {
				DASSERT(false);
				return nullptr;
			}

		private:
			System::Void btnNavSource_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnAddSource_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnNavDestination_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnCopy_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void cmbOverwrite_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
			System::Void cmbOperation_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
			System::Void timerUpdate_Tick(System::Object^ sender, System::EventArgs^ e);

			System::Void FormMain_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);

			System::Void btnSuspend_Click(System::Object^ sender, System::EventArgs^ e);

			System::Void showLogToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnClose_Click(System::Object^ sender, System::EventArgs^ e) {
				appClosing_ = true;
				Close();
			}
			System::Void btnAbout_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void ctxAbout_Opening(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e);

			System::Void tsmiAboutThisApplication_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiFile_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiDirectory_Click(System::Object^ sender, System::EventArgs^ e);

			void OnBufferSizeChanged(System::Object^ sender, System::EventArgs^ e);
			void OnRetryCountChanged(System::Object^ sender, System::EventArgs^ e);
			System::Void FormMain_Load(System::Object^ sender, System::EventArgs^ e);


			System::Void txtSource_TextChanged(System::Object^ sender, System::EventArgs^ e);

		}; // FormMain

		enum class USERACTION {
			UA_NONE,
			UA_RETRY,
			UA_CANCEL,
			UA_IGNORE,
			UA_WZOMODE,
		};
		ref class UserResponceOfFail
		{
		private:
			USERACTION action_;
			int bufferSize_ = -1;
		public:
			UserResponceOfFail(USERACTION action) : action_(action) {}
			UserResponceOfFail(USERACTION action, int bs) :
				action_(action), bufferSize_(bs) {}
			property bool IsRetry
			{
				bool get() { return action_ == USERACTION::UA_RETRY; }
			}
			property bool IsCancel
			{
				bool get() { return action_ == USERACTION::UA_CANCEL; }
			}
			property bool IsIgnore
			{
				bool get() { return action_ == USERACTION::UA_IGNORE; }
			}
			property bool IsWZOMode
			{
				bool get() { return action_ == USERACTION::UA_WZOMODE; }
			}
			property int BufferSize
			{
				int get() { return bufferSize_; }
			}
		};
		delegate UserResponceOfFail^ RISDLLLLDwIDelegate(int, String^, LONGLONG, LONGLONG, DWORD, int);
		delegate UserResponceOfFail^ RISDLLLLDwIIIDelegate(int, String^, LONGLONG, LONGLONG, DWORD, int, int, int);
	} // namespace
}