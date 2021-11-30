#pragma once

#include "threadData.h"
#include "FormLog.h"
#include "FormAbout.h"
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
		initonly String^ source_;
		initonly FormLog^ logForm_;
		FormAbout^ aboutForm_;
		System::Text::StringBuilder sbLogBuffer_;
	private: System::Windows::Forms::TextBox^ txtRetryCount;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ lblBuffer;
	private: System::Windows::Forms::TextBox^ txtBuffer;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ txtCurSrc;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ txtCurDst;
		   initonly String^ destination_;

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
	private: System::Windows::Forms::ComboBox^ cmbOverwrite;


	public:
		FormMain(String^ src, String^ dest);
		static FormMain^ theForm_;
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
			this->txtRetryCount = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->lblBuffer = (gcnew System::Windows::Forms::Label());
			this->txtBuffer = (gcnew System::Windows::Forms::TextBox());
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->spTransitory))->BeginInit();
			this->spTransitory->Panel1->SuspendLayout();
			this->spTransitory->Panel2->SuspendLayout();
			this->spTransitory->SuspendLayout();
			this->ctxAbout->SuspendLayout();
			this->SuspendLayout();
			// 
			// lblSource
			// 
			this->lblSource->AutoSize = true;
			this->lblSource->Location = System::Drawing::Point(12, 8);
			this->lblSource->Name = L"lblSource";
			this->lblSource->Size = System::Drawing::Size(42, 12);
			this->lblSource->TabIndex = 100;
			this->lblSource->Text = L"&Source:";
			// 
			// lblDest
			// 
			this->lblDest->AutoSize = true;
			this->lblDest->Location = System::Drawing::Point(12, 44);
			this->lblDest->Name = L"lblDest";
			this->lblDest->Size = System::Drawing::Size(65, 12);
			this->lblDest->TabIndex = 400;
			this->lblDest->Text = L"&Destination:";
			// 
			// txtSource
			// 
			this->txtSource->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtSource->Location = System::Drawing::Point(12, 23);
			this->txtSource->Name = L"txtSource";
			this->txtSource->Size = System::Drawing::Size(468, 19);
			this->txtSource->TabIndex = 200;
			// 
			// txtDestination
			// 
			this->txtDestination->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtDestination->Location = System::Drawing::Point(12, 59);
			this->txtDestination->Name = L"txtDestination";
			this->txtDestination->Size = System::Drawing::Size(468, 19);
			this->txtDestination->TabIndex = 500;
			// 
			// btnNavSource
			// 
			this->btnNavSource->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnNavSource->Location = System::Drawing::Point(486, 21);
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
			this->btnNavDestination->Location = System::Drawing::Point(486, 57);
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
			this->btnCopy->Location = System::Drawing::Point(12, 83);
			this->btnCopy->Name = L"btnCopy";
			this->btnCopy->Size = System::Drawing::Size(503, 21);
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
			this->txtLog->Location = System::Drawing::Point(81, 278);
			this->txtLog->Name = L"txtLog";
			this->txtLog->ReadOnly = true;
			this->txtLog->Size = System::Drawing::Size(431, 19);
			this->txtLog->TabIndex = 2500;
			// 
			// txtRetryCount
			// 
			this->txtRetryCount->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->txtRetryCount->Location = System::Drawing::Point(45, 355);
			this->txtRetryCount->Name = L"txtRetryCount";
			this->txtRetryCount->Size = System::Drawing::Size(86, 19);
			this->txtRetryCount->TabIndex = 2800;
			this->txtRetryCount->TextChanged += gcnew System::EventHandler(this, &FormMain::txtRetryCount_TextChanged);
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(43, 340);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 12);
			this->label1->TabIndex = 2700;
			this->label1->Text = L"Retry:";
			// 
			// lblBuffer
			// 
			this->lblBuffer->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->lblBuffer->AutoSize = true;
			this->lblBuffer->Location = System::Drawing::Point(135, 340);
			this->lblBuffer->Name = L"lblBuffer";
			this->lblBuffer->Size = System::Drawing::Size(39, 12);
			this->lblBuffer->TabIndex = 2900;
			this->lblBuffer->Text = L"Buffer:";
			// 
			// txtBuffer
			// 
			this->txtBuffer->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->txtBuffer->Location = System::Drawing::Point(137, 355);
			this->txtBuffer->Name = L"txtBuffer";
			this->txtBuffer->Size = System::Drawing::Size(86, 19);
			this->txtBuffer->TabIndex = 3000;
			this->txtBuffer->TextChanged += gcnew System::EventHandler(this, &FormMain::txtBuffer_TextChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 119);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(84, 12);
			this->label2->TabIndex = 800;
			this->label2->Text = L"&Current Source:";
			// 
			// txtCurSrc
			// 
			this->txtCurSrc->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtCurSrc->Location = System::Drawing::Point(12, 134);
			this->txtCurSrc->Name = L"txtCurSrc";
			this->txtCurSrc->ReadOnly = true;
			this->txtCurSrc->Size = System::Drawing::Size(503, 19);
			this->txtCurSrc->TabIndex = 900;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 155);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(107, 12);
			this->label3->TabIndex = 1000;
			this->label3->Text = L"Current Destination:";
			// 
			// txtCurDst
			// 
			this->txtCurDst->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtCurDst->Location = System::Drawing::Point(12, 170);
			this->txtCurDst->Name = L"txtCurDst";
			this->txtCurDst->ReadOnly = true;
			this->txtCurDst->Size = System::Drawing::Size(503, 19);
			this->txtCurDst->TabIndex = 1100;
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(226, 341);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(56, 12);
			this->label4->TabIndex = 3100;
			this->label4->Text = L"&Overwrite:";
			// 
			// cmbOverwrite
			// 
			this->cmbOverwrite->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->cmbOverwrite->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbOverwrite->FormattingEnabled = true;
			this->cmbOverwrite->Location = System::Drawing::Point(229, 355);
			this->cmbOverwrite->Name = L"cmbOverwrite";
			this->cmbOverwrite->Size = System::Drawing::Size(99, 20);
			this->cmbOverwrite->TabIndex = 3200;
			this->cmbOverwrite->SelectedIndexChanged += gcnew System::EventHandler(this, &FormMain::cmbOverwrite_SelectedIndexChanged);
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(334, 341);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(48, 12);
			this->label5->TabIndex = 3300;
			this->label5->Text = L"&Remove:";
			// 
			// cmbRemove
			// 
			this->cmbRemove->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->cmbRemove->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbRemove->FormattingEnabled = true;
			this->cmbRemove->Location = System::Drawing::Point(334, 355);
			this->cmbRemove->Name = L"cmbRemove";
			this->cmbRemove->Size = System::Drawing::Size(99, 20);
			this->cmbRemove->TabIndex = 3400;
			this->cmbRemove->SelectedIndexChanged += gcnew System::EventHandler(this, &FormMain::cmbRemove_SelectedIndexChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(3, 30);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(58, 12);
			this->label6->TabIndex = 1600;
			this->label6->Text = L"Total Size:";
			// 
			// txtTotalSize
			// 
			this->txtTotalSize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtTotalSize->Location = System::Drawing::Point(102, 27);
			this->txtTotalSize->Name = L"txtTotalSize";
			this->txtTotalSize->ReadOnly = true;
			this->txtTotalSize->Size = System::Drawing::Size(139, 19);
			this->txtTotalSize->TabIndex = 1700;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(3, 30);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(90, 12);
			this->label7->TabIndex = 1800;
			this->label7->Text = L"Total Processed:";
			// 
			// txtTotalProcessed
			// 
			this->txtTotalProcessed->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtTotalProcessed->Location = System::Drawing::Point(102, 27);
			this->txtTotalProcessed->Name = L"txtTotalProcessed";
			this->txtTotalProcessed->ReadOnly = true;
			this->txtTotalProcessed->Size = System::Drawing::Size(140, 19);
			this->txtTotalProcessed->TabIndex = 1900;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(3, 55);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(45, 12);
			this->label8->TabIndex = 2000;
			this->label8->Text = L"Current:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(3, 54);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(102, 12);
			this->label9->TabIndex = 2200;
			this->label9->Text = L"Current Processed:";
			// 
			// txtCurrentSize
			// 
			this->txtCurrentSize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtCurrentSize->Location = System::Drawing::Point(102, 51);
			this->txtCurrentSize->Name = L"txtCurrentSize";
			this->txtCurrentSize->ReadOnly = true;
			this->txtCurrentSize->Size = System::Drawing::Size(139, 19);
			this->txtCurrentSize->TabIndex = 2100;
			// 
			// txtCurrentProcessed
			// 
			this->txtCurrentProcessed->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtCurrentProcessed->Location = System::Drawing::Point(102, 51);
			this->txtCurrentProcessed->Name = L"txtCurrentProcessed";
			this->txtCurrentProcessed->ReadOnly = true;
			this->txtCurrentProcessed->Size = System::Drawing::Size(140, 19);
			this->txtCurrentProcessed->TabIndex = 2300;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(3, 6);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(67, 12);
			this->label10->TabIndex = 1200;
			this->label10->Text = L"Total Count:";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(3, 6);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(94, 12);
			this->label11->TabIndex = 1400;
			this->label11->Text = L"Count Processed:";
			// 
			// txtTotalCount
			// 
			this->txtTotalCount->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtTotalCount->Location = System::Drawing::Point(102, 3);
			this->txtTotalCount->Name = L"txtTotalCount";
			this->txtTotalCount->ReadOnly = true;
			this->txtTotalCount->Size = System::Drawing::Size(139, 19);
			this->txtTotalCount->TabIndex = 1300;
			// 
			// txtProcessedCount
			// 
			this->txtProcessedCount->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtProcessedCount->Location = System::Drawing::Point(102, 3);
			this->txtProcessedCount->Name = L"txtProcessedCount";
			this->txtProcessedCount->ReadOnly = true;
			this->txtProcessedCount->Size = System::Drawing::Size(140, 19);
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
			this->label12->Location = System::Drawing::Point(12, 281);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(58, 12);
			this->label12->TabIndex = 2400;
			this->label12->Text = L"Last Error:";
			// 
			// btnSuspend
			// 
			this->btnSuspend->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnSuspend->Enabled = false;
			this->btnSuspend->Location = System::Drawing::Point(12, 311);
			this->btnSuspend->Name = L"btnSuspend";
			this->btnSuspend->Size = System::Drawing::Size(503, 21);
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
			this->spTransitory->Location = System::Drawing::Point(12, 194);
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
			this->spTransitory->Size = System::Drawing::Size(503, 74);
			this->spTransitory->SplitterDistance = 251;
			this->spTransitory->TabIndex = 3501;
			// 
			// progressMain
			// 
			this->progressMain->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->progressMain->Location = System::Drawing::Point(12, 107);
			this->progressMain->Name = L"progressMain";
			this->progressMain->Size = System::Drawing::Size(502, 9);
			this->progressMain->TabIndex = 750;
			// 
			// btnAbout
			// 
			this->btnAbout->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->btnAbout->Location = System::Drawing::Point(12, 353);
			this->btnAbout->Name = L"btnAbout";
			this->btnAbout->Size = System::Drawing::Size(27, 23);
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
			this->ctxAbout->Size = System::Drawing::Size(212, 70);
			this->ctxAbout->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &FormMain::ctxAbout_Opening);
			// 
			// tsmiShowLog
			// 
			this->tsmiShowLog->Name = L"tsmiShowLog";
			this->tsmiShowLog->Size = System::Drawing::Size(211, 22);
			this->tsmiShowLog->Text = L"&Show Log";
			this->tsmiShowLog->Click += gcnew System::EventHandler(this, &FormMain::showLogToolStripMenuItem_Click);
			// 
			// tsmiAboutThisApplication
			// 
			this->tsmiAboutThisApplication->Name = L"tsmiAboutThisApplication";
			this->tsmiAboutThisApplication->Size = System::Drawing::Size(211, 22);
			this->tsmiAboutThisApplication->Text = L"&About this application...";
			this->tsmiAboutThisApplication->Click += gcnew System::EventHandler(this, &FormMain::tsmiAboutThisApplication_Click);
			// 
			// tsmiHelp
			// 
			this->tsmiHelp->Name = L"tsmiHelp";
			this->tsmiHelp->Size = System::Drawing::Size(211, 22);
			this->tsmiHelp->Text = L"&Help";
			// 
			// btnClose
			// 
			this->btnClose->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->btnClose->Location = System::Drawing::Point(440, 353);
			this->btnClose->Name = L"btnClose";
			this->btnClose->Size = System::Drawing::Size(75, 23);
			this->btnClose->TabIndex = 3502;
			this->btnClose->Text = L"&Close";
			this->btnClose->UseVisualStyleBackColor = true;
			this->btnClose->Click += gcnew System::EventHandler(this, &FormMain::btnClose_Click);
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(527, 385);
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
			this->Controls->Add(this->txtBuffer);
			this->Controls->Add(this->txtRetryCount);
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
			this->spTransitory->Panel1->ResumeLayout(false);
			this->spTransitory->Panel1->PerformLayout();
			this->spTransitory->Panel2->ResumeLayout(false);
			this->spTransitory->Panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->spTransitory))->EndInit();
			this->spTransitory->ResumeLayout(false);
			this->ctxAbout->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		System::Threading::Thread^ theThread_;
		//HANDLE hSource_;
		//HANDLE hDestination_;
		
		void StartOfThreadMaster(Object^ obj);
		void StartOfThreadFile(ThreadDataFile^ thData);
		void ThreadStarted();
		void ThreadFileStarted(ThreadDataFile^ thData);
		void ThreadFileEnded(ThreadDataFile^ thData);
		bool OpenFileFailedGetUserAction(DWORD le);
		void ReadFileFailed_obsolete(LONGLONG pos, LONGLONG allSize, DWORD le, int retried);
		Object^ ReadFileFailedGetUserAction(LONGLONG pos, LONGLONG allSize, DWORD le, int retried);
		void ProcessProgressed_obsolete(LONGLONG pos);
		void ProgressWriteWithZero(LONGLONG pos, int bufferSize);
		void ThreadFinished(ThreadDataMaster^ thData);

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
		bool AskOverwrite(String^ fileTobeOverwritten);
		
	private:
		System::Void btnNavSource_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnNavDestination_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnCopy_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void cmbOverwrite_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void cmbRemove_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void timerUpdate_Tick(System::Object^ sender, System::EventArgs^ e);

		System::Void txtRetryCount_TextChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void txtBuffer_TextChanged(System::Object^ sender, System::EventArgs^ e);

		System::Void FormMain_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);

		System::Void btnSuspend_Click(System::Object^ sender, System::EventArgs^ e);

		System::Void showLogToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
			if (!logForm_->Visible) {
				logForm_->Show(this);
			}
			else {
				logForm_->Visible = false;
			}
		}

		System::Void btnClose_Click(System::Object^ sender, System::EventArgs^ e) {
			Close();
		}
		System::Void btnAbout_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ctxAbout_Opening(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e);

		System::Void tsmiAboutThisApplication_Click(System::Object^ sender, System::EventArgs^ e) {
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

}; // FormMain

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
	ReadFailData(ACTION action) : action_(action) {}
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

} // namespace


