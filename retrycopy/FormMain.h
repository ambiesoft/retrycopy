#pragma once

#include "Ref.h"
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
		static initonly String^ KEY_REMOVE = "Remove";
		static property String^ IniPath
		{
			String^ get();
		}
	private:

		initonly FormLog^ logForm_;
		FormAbout^ aboutForm_;
		System::Text::StringBuilder sbLogBuffer_;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ lblBuffer;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ txtCurSrc;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ txtCurDst;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::ComboBox^ cmbRemove;
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
	private: System::Windows::Forms::Button^ btnCopy;
	private: System::Windows::Forms::TextBox^ txtLog;
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
			this->lblSource = (gcnew System::Windows::Forms::Label());
			this->lblDest = (gcnew System::Windows::Forms::Label());
			this->txtSource = (gcnew System::Windows::Forms::TextBox());
			this->txtDestination = (gcnew System::Windows::Forms::TextBox());
			this->btnNavSource = (gcnew System::Windows::Forms::Button());
			this->btnNavDestination = (gcnew System::Windows::Forms::Button());
			this->btnCopy = (gcnew System::Windows::Forms::Button());
			this->txtLog = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->lblBuffer = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txtCurSrc = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->txtCurDst = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->cmbOverwrite = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->cmbRemove = (gcnew System::Windows::Forms::ComboBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->txtTotalSize = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->txtTotalProcessed = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->txtCurrentSize = (gcnew System::Windows::Forms::TextBox());
			this->txtCurrentProcessed = (gcnew System::Windows::Forms::TextBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->txtTotalCount = (gcnew System::Windows::Forms::TextBox());
			this->txtProcessedCount = (gcnew System::Windows::Forms::TextBox());
			this->timerUpdate = (gcnew System::Windows::Forms::Timer(this->components));
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->btnSuspend = (gcnew System::Windows::Forms::Button());
			this->spTransitory = (gcnew System::Windows::Forms::SplitContainer());
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
			// lblSource
			// 
			this->lblSource->AutoSize = true;
			this->lblSource->Location = System::Drawing::Point(12, 9);
			this->lblSource->Name = L"lblSource";
			this->lblSource->Size = System::Drawing::Size(44, 13);
			this->lblSource->TabIndex = 100;
			this->lblSource->Text = L"&Source:";
			// 
			// lblDest
			// 
			this->lblDest->AutoSize = true;
			this->lblDest->Location = System::Drawing::Point(12, 48);
			this->lblDest->Name = L"lblDest";
			this->lblDest->Size = System::Drawing::Size(63, 13);
			this->lblDest->TabIndex = 400;
			this->lblDest->Text = L"&Destination:";
			// 
			// txtSource
			// 
			this->txtSource->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtSource->Location = System::Drawing::Point(12, 25);
			this->txtSource->Name = L"txtSource";
			this->txtSource->Size = System::Drawing::Size(468, 20);
			this->txtSource->TabIndex = 200;
			// 
			// txtDestination
			// 
			this->txtDestination->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtDestination->Location = System::Drawing::Point(12, 64);
			this->txtDestination->Name = L"txtDestination";
			this->txtDestination->Size = System::Drawing::Size(468, 20);
			this->txtDestination->TabIndex = 500;
			// 
			// btnNavSource
			// 
			this->btnNavSource->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnNavSource->Location = System::Drawing::Point(486, 25);
			this->btnNavSource->Name = L"btnNavSource";
			this->btnNavSource->Size = System::Drawing::Size(29, 21);
			this->btnNavSource->TabIndex = 300;
			this->btnNavSource->Text = L"...";
			this->btnNavSource->UseVisualStyleBackColor = true;
			this->btnNavSource->Click += gcnew System::EventHandler(this, &FormMain::btnNavSource_Click);
			// 
			// btnNavDestination
			// 
			this->btnNavDestination->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnNavDestination->Location = System::Drawing::Point(486, 64);
			this->btnNavDestination->Name = L"btnNavDestination";
			this->btnNavDestination->Size = System::Drawing::Size(29, 21);
			this->btnNavDestination->TabIndex = 600;
			this->btnNavDestination->Text = L"...";
			this->btnNavDestination->UseVisualStyleBackColor = true;
			this->btnNavDestination->Click += gcnew System::EventHandler(this, &FormMain::btnNavDestination_Click);
			// 
			// btnCopy
			// 
			this->btnCopy->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnCopy->Location = System::Drawing::Point(12, 90);
			this->btnCopy->Name = L"btnCopy";
			this->btnCopy->Size = System::Drawing::Size(503, 23);
			this->btnCopy->TabIndex = 700;
			this->btnCopy->Text = L"&Copy";
			this->btnCopy->UseVisualStyleBackColor = true;
			this->btnCopy->Click += gcnew System::EventHandler(this, &FormMain::btnCopy_Click);
			// 
			// txtLog
			// 
			this->txtLog->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtLog->Location = System::Drawing::Point(81, 301);
			this->txtLog->Name = L"txtLog";
			this->txtLog->ReadOnly = true;
			this->txtLog->Size = System::Drawing::Size(431, 20);
			this->txtLog->TabIndex = 2500;
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(43, 368);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
			this->label1->TabIndex = 2700;
			this->label1->Text = L"Retry:";
			// 
			// lblBuffer
			// 
			this->lblBuffer->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->lblBuffer->AutoSize = true;
			this->lblBuffer->Location = System::Drawing::Point(135, 368);
			this->lblBuffer->Name = L"lblBuffer";
			this->lblBuffer->Size = System::Drawing::Size(38, 13);
			this->lblBuffer->TabIndex = 2900;
			this->lblBuffer->Text = L"Buffer:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 129);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(81, 13);
			this->label2->TabIndex = 800;
			this->label2->Text = L"&Current Source:";
			// 
			// txtCurSrc
			// 
			this->txtCurSrc->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtCurSrc->Location = System::Drawing::Point(12, 145);
			this->txtCurSrc->Name = L"txtCurSrc";
			this->txtCurSrc->ReadOnly = true;
			this->txtCurSrc->Size = System::Drawing::Size(503, 20);
			this->txtCurSrc->TabIndex = 900;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 168);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(100, 13);
			this->label3->TabIndex = 1000;
			this->label3->Text = L"Current Destination:";
			// 
			// txtCurDst
			// 
			this->txtCurDst->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtCurDst->Location = System::Drawing::Point(12, 184);
			this->txtCurDst->Name = L"txtCurDst";
			this->txtCurDst->ReadOnly = true;
			this->txtCurDst->Size = System::Drawing::Size(503, 20);
			this->txtCurDst->TabIndex = 1100;
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(226, 369);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(55, 13);
			this->label4->TabIndex = 3100;
			this->label4->Text = L"&Overwrite:";
			// 
			// cmbOverwrite
			// 
			this->cmbOverwrite->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->cmbOverwrite->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbOverwrite->FormattingEnabled = true;
			this->cmbOverwrite->Location = System::Drawing::Point(229, 385);
			this->cmbOverwrite->Name = L"cmbOverwrite";
			this->cmbOverwrite->Size = System::Drawing::Size(99, 21);
			this->cmbOverwrite->TabIndex = 3200;
			this->cmbOverwrite->SelectedIndexChanged += gcnew System::EventHandler(this, &FormMain::cmbOverwrite_SelectedIndexChanged);
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(334, 369);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(50, 13);
			this->label5->TabIndex = 3300;
			this->label5->Text = L"&Remove:";
			// 
			// cmbRemove
			// 
			this->cmbRemove->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->cmbRemove->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbRemove->FormattingEnabled = true;
			this->cmbRemove->Location = System::Drawing::Point(334, 385);
			this->cmbRemove->Name = L"cmbRemove";
			this->cmbRemove->Size = System::Drawing::Size(99, 21);
			this->cmbRemove->TabIndex = 3400;
			this->cmbRemove->SelectedIndexChanged += gcnew System::EventHandler(this, &FormMain::cmbRemove_SelectedIndexChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(3, 32);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(57, 13);
			this->label6->TabIndex = 1600;
			this->label6->Text = L"Total Size:";
			// 
			// txtTotalSize
			// 
			this->txtTotalSize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtTotalSize->Location = System::Drawing::Point(110, 29);
			this->txtTotalSize->Name = L"txtTotalSize";
			this->txtTotalSize->ReadOnly = true;
			this->txtTotalSize->Size = System::Drawing::Size(131, 20);
			this->txtTotalSize->TabIndex = 1700;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(3, 32);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(87, 13);
			this->label7->TabIndex = 1800;
			this->label7->Text = L"Total Processed:";
			// 
			// txtTotalProcessed
			// 
			this->txtTotalProcessed->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtTotalProcessed->Location = System::Drawing::Point(111, 29);
			this->txtTotalProcessed->Name = L"txtTotalProcessed";
			this->txtTotalProcessed->ReadOnly = true;
			this->txtTotalProcessed->Size = System::Drawing::Size(131, 20);
			this->txtTotalProcessed->TabIndex = 1900;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(3, 60);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(44, 13);
			this->label8->TabIndex = 2000;
			this->label8->Text = L"Current:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(3, 59);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(97, 13);
			this->label9->TabIndex = 2200;
			this->label9->Text = L"Current Processed:";
			// 
			// txtCurrentSize
			// 
			this->txtCurrentSize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtCurrentSize->Location = System::Drawing::Point(110, 55);
			this->txtCurrentSize->Name = L"txtCurrentSize";
			this->txtCurrentSize->ReadOnly = true;
			this->txtCurrentSize->Size = System::Drawing::Size(131, 20);
			this->txtCurrentSize->TabIndex = 2100;
			// 
			// txtCurrentProcessed
			// 
			this->txtCurrentProcessed->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtCurrentProcessed->Location = System::Drawing::Point(111, 55);
			this->txtCurrentProcessed->Name = L"txtCurrentProcessed";
			this->txtCurrentProcessed->ReadOnly = true;
			this->txtCurrentProcessed->Size = System::Drawing::Size(131, 20);
			this->txtCurrentProcessed->TabIndex = 2300;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(3, 6);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(65, 13);
			this->label10->TabIndex = 1200;
			this->label10->Text = L"Total Count:";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(3, 6);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(91, 13);
			this->label11->TabIndex = 1400;
			this->label11->Text = L"Count Processed:";
			// 
			// txtTotalCount
			// 
			this->txtTotalCount->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtTotalCount->Location = System::Drawing::Point(110, 3);
			this->txtTotalCount->Name = L"txtTotalCount";
			this->txtTotalCount->ReadOnly = true;
			this->txtTotalCount->Size = System::Drawing::Size(131, 20);
			this->txtTotalCount->TabIndex = 1300;
			// 
			// txtProcessedCount
			// 
			this->txtProcessedCount->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtProcessedCount->Location = System::Drawing::Point(111, 3);
			this->txtProcessedCount->Name = L"txtProcessedCount";
			this->txtProcessedCount->ReadOnly = true;
			this->txtProcessedCount->Size = System::Drawing::Size(131, 20);
			this->txtProcessedCount->TabIndex = 1500;
			// 
			// timerUpdate
			// 
			this->timerUpdate->Interval = 500;
			this->timerUpdate->Tick += gcnew System::EventHandler(this, &FormMain::timerUpdate_Tick);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(12, 304);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(55, 13);
			this->label12->TabIndex = 2400;
			this->label12->Text = L"Last Error:";
			// 
			// btnSuspend
			// 
			this->btnSuspend->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnSuspend->Enabled = false;
			this->btnSuspend->Location = System::Drawing::Point(12, 337);
			this->btnSuspend->Name = L"btnSuspend";
			this->btnSuspend->Size = System::Drawing::Size(503, 23);
			this->btnSuspend->TabIndex = 2600;
			this->btnSuspend->Text = L"&Suspend";
			this->btnSuspend->UseVisualStyleBackColor = true;
			this->btnSuspend->Click += gcnew System::EventHandler(this, &FormMain::btnSuspend_Click);
			// 
			// spTransitory
			// 
			this->spTransitory->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->spTransitory->IsSplitterFixed = true;
			this->spTransitory->Location = System::Drawing::Point(12, 210);
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
			this->spTransitory->Size = System::Drawing::Size(503, 80);
			this->spTransitory->SplitterDistance = 251;
			this->spTransitory->TabIndex = 3501;
			// 
			// progressMain
			// 
			this->progressMain->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->progressMain->Location = System::Drawing::Point(12, 116);
			this->progressMain->Name = L"progressMain";
			this->progressMain->Size = System::Drawing::Size(502, 10);
			this->progressMain->TabIndex = 750;
			// 
			// btnAbout
			// 
			this->btnAbout->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->btnAbout->Location = System::Drawing::Point(12, 385);
			this->btnAbout->Name = L"btnAbout";
			this->btnAbout->Size = System::Drawing::Size(27, 22);
			this->btnAbout->TabIndex = 2650;
			this->btnAbout->Text = L"...";
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
			this->ctxAbout->Size = System::Drawing::Size(190, 70);
			this->ctxAbout->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &FormMain::ctxAbout_Opening);
			// 
			// tsmiShowLog
			// 
			this->tsmiShowLog->Name = L"tsmiShowLog";
			this->tsmiShowLog->Size = System::Drawing::Size(189, 22);
			this->tsmiShowLog->Text = L"&Show Log";
			this->tsmiShowLog->Click += gcnew System::EventHandler(this, &FormMain::showLogToolStripMenuItem_Click);
			// 
			// tsmiAboutThisApplication
			// 
			this->tsmiAboutThisApplication->Name = L"tsmiAboutThisApplication";
			this->tsmiAboutThisApplication->Size = System::Drawing::Size(189, 22);
			this->tsmiAboutThisApplication->Text = L"&About this application...";
			this->tsmiAboutThisApplication->Click += gcnew System::EventHandler(this, &FormMain::tsmiAboutThisApplication_Click);
			// 
			// tsmiHelp
			// 
			this->tsmiHelp->Name = L"tsmiHelp";
			this->tsmiHelp->Size = System::Drawing::Size(189, 22);
			this->tsmiHelp->Text = L"&Help";
			// 
			// btnClose
			// 
			this->btnClose->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->btnClose->Location = System::Drawing::Point(440, 385);
			this->btnClose->Name = L"btnClose";
			this->btnClose->Size = System::Drawing::Size(75, 22);
			this->btnClose->TabIndex = 3502;
			this->btnClose->Text = L"&Close";
			this->btnClose->UseVisualStyleBackColor = true;
			this->btnClose->Click += gcnew System::EventHandler(this, &FormMain::btnClose_Click);
			// 
			// ctxNavigate
			// 
			this->ctxNavigate->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->tsmiFile, this->tsmiDirectory });
			this->ctxNavigate->Name = L"ctxNavigate";
			this->ctxNavigate->Size = System::Drawing::Size(119, 48);
			// 
			// tsmiFile
			// 
			this->tsmiFile->Name = L"tsmiFile";
			this->tsmiFile->Size = System::Drawing::Size(118, 22);
			this->tsmiFile->Text = L"&File";
			this->tsmiFile->Click += gcnew System::EventHandler(this, &FormMain::tsmiFile_Click);
			// 
			// tsmiDirectory
			// 
			this->tsmiDirectory->Name = L"tsmiDirectory";
			this->tsmiDirectory->Size = System::Drawing::Size(118, 22);
			this->tsmiDirectory->Text = L"&Directory";
			this->tsmiDirectory->Click += gcnew System::EventHandler(this, &FormMain::tsmiDirectory_Click);
			// 
			// udBuffer
			// 
			this->udBuffer->Location = System::Drawing::Point(138, 386);
			this->udBuffer->Name = L"udBuffer";
			this->udBuffer->Size = System::Drawing::Size(85, 20);
			this->udBuffer->TabIndex = 3000;
			// 
			// udRetry
			// 
			this->udRetry->Location = System::Drawing::Point(45, 386);
			this->udRetry->Name = L"udRetry";
			this->udRetry->Size = System::Drawing::Size(87, 20);
			this->udRetry->TabIndex = 2800;
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(527, 417);
			this->Controls->Add(this->udRetry);
			this->Controls->Add(this->udBuffer);
			this->Controls->Add(this->btnClose);
			this->Controls->Add(this->btnAbout);
			this->Controls->Add(this->progressMain);
			this->Controls->Add(this->spTransitory);
			this->Controls->Add(this->btnSuspend);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->cmbRemove);
			this->Controls->Add(this->cmbOverwrite);
			this->Controls->Add(this->lblBuffer);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->txtLog);
			this->Controls->Add(this->btnCopy);
			this->Controls->Add(this->btnNavDestination);
			this->Controls->Add(this->btnNavSource);
			this->Controls->Add(this->txtCurDst);
			this->Controls->Add(this->txtDestination);
			this->Controls->Add(this->txtCurSrc);
			this->Controls->Add(this->txtSource);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->lblDest);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->lblSource);
			this->Name = L"FormMain";
			this->Text = L"retrycopy";
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
			int bufferSize);
		UserResponceOfFail^ SFPFailedGetUserAction(int tn, String^ file, LONGLONG pos, LONGLONG allSize, DWORD le, int retried);
		void ProgressWriteWithZero(int tn, LONGLONG pos, int bufferSize);
		void ThreadTaskFinished(ThreadDataMaster^ thData);
		void ThreadFinished(int tn);
		void ThreadLog(String^ logMessage) {
			AppendLog(logMessage);
		}
		void AppendLog(String^ message);
		void AppendLogNow(String^ message);

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
	internal:
		bool AskOverwrite(int tn, String^ fileTobeOverwritten);
		Object^ EndInvokeWithTN(int tn, IAsyncResult^);
		Object^ EndInvoke(IAsyncResult^) new {
			DASSERT(false);
			return nullptr;
		}

	private:
		System::Void btnNavSource_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnNavDestination_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnCopy_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void cmbOverwrite_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void cmbRemove_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void timerUpdate_Tick(System::Object^ sender, System::EventArgs^ e);

		System::Void FormMain_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);

		System::Void btnSuspend_Click(System::Object^ sender, System::EventArgs^ e);

		System::Void showLogToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnClose_Click(System::Object^ sender, System::EventArgs^ e) {
			Close();
		}
		System::Void btnAbout_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ctxAbout_Opening(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e);

		System::Void tsmiAboutThisApplication_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void tsmiFile_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void tsmiDirectory_Click(System::Object^ sender, System::EventArgs^ e);

		void OnBufferSizeChanged(System::Object^ sender, System::EventArgs^ e);
		void OnRetryCountChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void FormMain_Load(System::Object^ sender, System::EventArgs^ e) {
			if (bCloseNow_)
				Close();
		}

}; // FormMain

	enum class USERACTION {
		UA_NONE,
		UA_RETRY,
		UA_CANCEL,
		UA_IGNORE,
		// UA_IGNOREALL,
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
		//property bool IsIgnoreAll
		//{
		//	bool get() { return action_ == USERACTION::UA_IGNOREALL; }
		//}
		property int BufferSize
		{
			int get() { return bufferSize_; }
		}
	};
	delegate UserResponceOfFail^ RISDLLLLDwIDelegate(int, String^, LONGLONG, LONGLONG, DWORD, int);
	delegate UserResponceOfFail^ RISDLLLLDwIIDelegate(int, String^, LONGLONG, LONGLONG, DWORD, int, int);
} // namespace


