#pragma once
namespace Ambiesoft {
	namespace retrycopy {

		using namespace System;
		using namespace System::ComponentModel;
		using namespace System::Collections;
		using namespace System::Windows::Forms;
		using namespace System::Data;
		using namespace System::Drawing;


		/// <summary>
		/// Summary for ReadErrorDialog
		/// </summary>
		public ref class ReadErrorDialog : public System::Windows::Forms::Form
		{
			initonly String^ drive_;
		public:
			ReadErrorDialog(String^ message, String^ drive, int bufferSize, int retryCount)
			{
				InitializeComponent();

				txtMessage->Text = message;
				drive_ = drive;
				bufferSize_ = bufferSize;
				retryCount_ = retryCount;
			}
		private:
			property String^ Drive
			{
				String^ get() { return drive_; }
			}
		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~ReadErrorDialog()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::TextBox^ txtMessage;
		protected:
		private: System::Windows::Forms::Button^ btnRetry;
		private: System::Windows::Forms::Button^ btnChangeBufferSize;
		private: System::Windows::Forms::Button^ btnGiveupAndWriteZero;


		private: System::Windows::Forms::Button^ btnGiveUpAndWZOmode;
		private: System::Windows::Forms::SplitContainer^ spMain;
		private: System::Windows::Forms::TextBox^ txtInfo;
		private: System::Windows::Forms::Button^ btnShowDriveInfo;
		private: System::Windows::Forms::Button^ btnChangeRetryCount;

		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>
			System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
			/// <summary>
			/// Required method for Designer support - do not modify
			/// the contents of this method with the code editor.
			/// </summary>
			void InitializeComponent(void)
			{
				this->txtMessage = (gcnew System::Windows::Forms::TextBox());
				this->btnRetry = (gcnew System::Windows::Forms::Button());
				this->btnChangeBufferSize = (gcnew System::Windows::Forms::Button());
				this->btnGiveupAndWriteZero = (gcnew System::Windows::Forms::Button());
				this->btnGiveUpAndWZOmode = (gcnew System::Windows::Forms::Button());
				this->spMain = (gcnew System::Windows::Forms::SplitContainer());
				this->txtInfo = (gcnew System::Windows::Forms::TextBox());
				this->btnShowDriveInfo = (gcnew System::Windows::Forms::Button());
				this->btnChangeRetryCount = (gcnew System::Windows::Forms::Button());
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->spMain))->BeginInit();
				this->spMain->Panel1->SuspendLayout();
				this->spMain->Panel2->SuspendLayout();
				this->spMain->SuspendLayout();
				this->SuspendLayout();
				// 
				// txtMessage
				// 
				this->txtMessage->Dock = System::Windows::Forms::DockStyle::Fill;
				this->txtMessage->Location = System::Drawing::Point(0, 0);
				this->txtMessage->Multiline = true;
				this->txtMessage->Name = L"txtMessage";
				this->txtMessage->ReadOnly = true;
				this->txtMessage->ScrollBars = System::Windows::Forms::ScrollBars::Both;
				this->txtMessage->Size = System::Drawing::Size(592, 145);
				this->txtMessage->TabIndex = 100;
				// 
				// btnRetry
				// 
				this->btnRetry->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
				this->btnRetry->Location = System::Drawing::Point(512, 311);
				this->btnRetry->Name = L"btnRetry";
				this->btnRetry->Size = System::Drawing::Size(94, 67);
				this->btnRetry->TabIndex = 700;
				this->btnRetry->Text = L"&Retry";
				this->btnRetry->UseVisualStyleBackColor = true;
				this->btnRetry->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnRetry_Click);
				this->btnRetry->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnRetry->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// btnChangeBufferSize
				// 
				this->btnChangeBufferSize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
				this->btnChangeBufferSize->Location = System::Drawing::Point(12, 310);
				this->btnChangeBufferSize->Name = L"btnChangeBufferSize";
				this->btnChangeBufferSize->Size = System::Drawing::Size(94, 67);
				this->btnChangeBufferSize->TabIndex = 300;
				this->btnChangeBufferSize->Text = L"Change Buffer size";
				this->btnChangeBufferSize->UseVisualStyleBackColor = true;
				this->btnChangeBufferSize->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnChangeBufferSize_Click);
				this->btnChangeBufferSize->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnChangeBufferSize->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// btnGiveupAndWriteZero
				// 
				this->btnGiveupAndWriteZero->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
				this->btnGiveupAndWriteZero->Location = System::Drawing::Point(212, 311);
				this->btnGiveupAndWriteZero->Name = L"btnGiveupAndWriteZero";
				this->btnGiveupAndWriteZero->Size = System::Drawing::Size(94, 67);
				this->btnGiveupAndWriteZero->TabIndex = 400;
				this->btnGiveupAndWriteZero->Text = L"Give up to read and write zero";
				this->btnGiveupAndWriteZero->UseVisualStyleBackColor = true;
				this->btnGiveupAndWriteZero->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnGiveupAndWriteZero_Click);
				this->btnGiveupAndWriteZero->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnGiveupAndWriteZero->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// btnGiveUpAndWZOmode
				// 
				this->btnGiveUpAndWZOmode->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
				this->btnGiveUpAndWZOmode->Location = System::Drawing::Point(312, 311);
				this->btnGiveUpAndWZOmode->Name = L"btnGiveUpAndWZOmode";
				this->btnGiveUpAndWZOmode->Size = System::Drawing::Size(94, 67);
				this->btnGiveUpAndWZOmode->TabIndex = 500;
				this->btnGiveUpAndWZOmode->Text = L"Give up to read and write zero until it succeed";
				this->btnGiveUpAndWZOmode->UseVisualStyleBackColor = true;
				this->btnGiveUpAndWZOmode->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnGiveUpAndWZOmode_Click);
				this->btnGiveUpAndWZOmode->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnGiveUpAndWZOmode->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// spMain
				// 
				this->spMain->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
					| System::Windows::Forms::AnchorStyles::Left)
					| System::Windows::Forms::AnchorStyles::Right));
				this->spMain->Location = System::Drawing::Point(12, 12);
				this->spMain->Name = L"spMain";
				this->spMain->Orientation = System::Windows::Forms::Orientation::Horizontal;
				// 
				// spMain.Panel1
				// 
				this->spMain->Panel1->Controls->Add(this->txtMessage);
				// 
				// spMain.Panel2
				// 
				this->spMain->Panel2->Controls->Add(this->txtInfo);
				this->spMain->Size = System::Drawing::Size(592, 292);
				this->spMain->SplitterDistance = 145;
				this->spMain->TabIndex = 5;
				// 
				// txtInfo
				// 
				this->txtInfo->Dock = System::Windows::Forms::DockStyle::Fill;
				this->txtInfo->Location = System::Drawing::Point(0, 0);
				this->txtInfo->Multiline = true;
				this->txtInfo->Name = L"txtInfo";
				this->txtInfo->ReadOnly = true;
				this->txtInfo->Size = System::Drawing::Size(592, 143);
				this->txtInfo->TabIndex = 200;
				// 
				// btnShowDriveInfo
				// 
				this->btnShowDriveInfo->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
				this->btnShowDriveInfo->Location = System::Drawing::Point(412, 311);
				this->btnShowDriveInfo->Name = L"btnShowDriveInfo";
				this->btnShowDriveInfo->Size = System::Drawing::Size(94, 67);
				this->btnShowDriveInfo->TabIndex = 600;
				this->btnShowDriveInfo->Text = L"&Show Drive Info";
				this->btnShowDriveInfo->UseVisualStyleBackColor = true;
				this->btnShowDriveInfo->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Click);
				this->btnShowDriveInfo->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnShowDriveInfo->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// btnChangeRetryCount
				// 
				this->btnChangeRetryCount->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
				this->btnChangeRetryCount->Location = System::Drawing::Point(112, 310);
				this->btnChangeRetryCount->Name = L"btnChangeRetryCount";
				this->btnChangeRetryCount->Size = System::Drawing::Size(94, 67);
				this->btnChangeRetryCount->TabIndex = 350;
				this->btnChangeRetryCount->Text = L"Change Retry Count";
				this->btnChangeRetryCount->UseVisualStyleBackColor = true;
				this->btnChangeRetryCount->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnChangeRetryCount_Click);
				this->btnChangeRetryCount->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnChangeRetryCount->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// ReadErrorDialog
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(616, 390);
				this->Controls->Add(this->btnShowDriveInfo);
				this->Controls->Add(this->spMain);
				this->Controls->Add(this->btnGiveUpAndWZOmode);
				this->Controls->Add(this->btnGiveupAndWriteZero);
				this->Controls->Add(this->btnChangeRetryCount);
				this->Controls->Add(this->btnChangeBufferSize);
				this->Controls->Add(this->btnRetry);
				this->MaximizeBox = false;
				this->MinimizeBox = false;
				this->MinimumSize = System::Drawing::Size(632, 429);
				this->Name = L"ReadErrorDialog";
				this->ShowIcon = false;
				this->ShowInTaskbar = false;
				this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
				this->Text = L"ReadErrorDialog";
				this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ReadErrorDialog::ReadErrorDialog_FormClosing);
				this->spMain->Panel1->ResumeLayout(false);
				this->spMain->Panel1->PerformLayout();
				this->spMain->Panel2->ResumeLayout(false);
				this->spMain->Panel2->PerformLayout();
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->spMain))->EndInit();
				this->spMain->ResumeLayout(false);
				this->ResumeLayout(false);

			}
#pragma endregion

		private:
			READERROR_RESPONSE responce_ = READERROR_RESPONSE::RR_NONE;
			int bufferSize_ = 0;
			int retryCount_ = 0;
		public:
			property int BufferSize
			{
				int get() { return bufferSize_; }
			private:
				void set(int v) {
					bufferSize_ = v;
					if (btnChangeBufferSize->Focused) {
						SetInfoText(btnChangeBufferSize);
					}
				}
			}
			property int RetryCount
			{
				int get() { return retryCount_; }
			private:
				void set(int v) {
					retryCount_ = v;
					if (btnChangeRetryCount->Focused) {
						SetInfoText(btnChangeRetryCount);
					}
				}
			}

			READERROR_RESPONSE ShowDialogAndGetResponce(System::Windows::Forms::IWin32Window^ parent);
		private:
			void SetInfoText(Object^ sender);
			System::Void btnRetry_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnChangeBufferSize_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnChangeRetryCount_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnGiveupAndWriteZero_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnGiveUpAndWZOmode_Click(System::Object^ sender, System::EventArgs^ e);

			System::Void btnShowDriveInfo_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void ReadErrorDialog_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
			System::Void btnShowDriveInfo_Enter(System::Object^ sender, System::EventArgs^ e)
			{
				SetInfoText(sender);
			}
			System::Void btnShowDriveInfo_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
			{
				DASSERT_IS_CLASS(sender, System::Windows::Forms::Button);
				SetInfoText(sender);
				((Control^)sender)->Focus();
			}
		};
	}
}