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
			initonly int initialBufferSize_ = 0;
			initonly String^ drive_;
		public:
			ReadErrorDialog(String^ message, String^ drive, int bufferSize, int retryCount)
			{
				InitializeComponent();

				txtMessage->Text = message;
				drive_ = drive;
				bufferSize_ = bufferSize;
				initialBufferSize_ = bufferSize;
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

		private: System::Windows::Forms::Button^ btnCancel;
		private: System::Windows::Forms::Button^ btnGiveUpAndWZOmode;
		private: System::Windows::Forms::SplitContainer^ spMain;
		private: System::Windows::Forms::TextBox^ txtInfo;
		private: System::Windows::Forms::Button^ btnShowDriveInfo;

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
				this->btnCancel = (gcnew System::Windows::Forms::Button());
				this->btnGiveUpAndWZOmode = (gcnew System::Windows::Forms::Button());
				this->spMain = (gcnew System::Windows::Forms::SplitContainer());
				this->txtInfo = (gcnew System::Windows::Forms::TextBox());
				this->btnShowDriveInfo = (gcnew System::Windows::Forms::Button());
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
				this->txtMessage->Size = System::Drawing::Size(581, 128);
				this->txtMessage->TabIndex = 100;
				// 
				// btnRetry
				// 
				this->btnRetry->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
				this->btnRetry->Location = System::Drawing::Point(413, 276);
				this->btnRetry->Name = L"btnRetry";
				this->btnRetry->Size = System::Drawing::Size(94, 67);
				this->btnRetry->TabIndex = 700;
				this->btnRetry->Text = L"&Retry";
				this->btnRetry->UseVisualStyleBackColor = true;
				this->btnRetry->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnRetry_Click);
				this->btnRetry->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnRetry_Enter);
				this->btnRetry->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnRetry_MouseMove);
				// 
				// btnChangeBufferSize
				// 
				this->btnChangeBufferSize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
				this->btnChangeBufferSize->Location = System::Drawing::Point(12, 275);
				this->btnChangeBufferSize->Name = L"btnChangeBufferSize";
				this->btnChangeBufferSize->Size = System::Drawing::Size(94, 67);
				this->btnChangeBufferSize->TabIndex = 300;
				this->btnChangeBufferSize->Text = L"Change Buffer size";
				this->btnChangeBufferSize->UseVisualStyleBackColor = true;
				this->btnChangeBufferSize->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnChangeBufferSize_Click);
				// 
				// btnGiveupAndWriteZero
				// 
				this->btnGiveupAndWriteZero->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
				this->btnGiveupAndWriteZero->Location = System::Drawing::Point(112, 276);
				this->btnGiveupAndWriteZero->Name = L"btnGiveupAndWriteZero";
				this->btnGiveupAndWriteZero->Size = System::Drawing::Size(94, 67);
				this->btnGiveupAndWriteZero->TabIndex = 400;
				this->btnGiveupAndWriteZero->Text = L"Give up to read and write zero";
				this->btnGiveupAndWriteZero->UseVisualStyleBackColor = true;
				this->btnGiveupAndWriteZero->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnGiveupAndWriteZero_Click);
				// 
				// btnCancel
				// 
				this->btnCancel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
				this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->btnCancel->Location = System::Drawing::Point(513, 276);
				this->btnCancel->Name = L"btnCancel";
				this->btnCancel->Size = System::Drawing::Size(80, 67);
				this->btnCancel->TabIndex = 800;
				this->btnCancel->Text = L"Cancel";
				this->btnCancel->UseVisualStyleBackColor = true;
				// 
				// btnGiveUpAndWZOmode
				// 
				this->btnGiveUpAndWZOmode->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
				this->btnGiveUpAndWZOmode->Location = System::Drawing::Point(212, 276);
				this->btnGiveUpAndWZOmode->Name = L"btnGiveUpAndWZOmode";
				this->btnGiveUpAndWZOmode->Size = System::Drawing::Size(94, 67);
				this->btnGiveUpAndWZOmode->TabIndex = 500;
				this->btnGiveUpAndWZOmode->Text = L"Give up to read and write zero until it succeed";
				this->btnGiveUpAndWZOmode->UseVisualStyleBackColor = true;
				this->btnGiveUpAndWZOmode->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnGiveUpAndWZOmode_Click);
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
				this->spMain->Size = System::Drawing::Size(581, 257);
				this->spMain->SplitterDistance = 128;
				this->spMain->TabIndex = 5;
				// 
				// txtInfo
				// 
				this->txtInfo->Dock = System::Windows::Forms::DockStyle::Fill;
				this->txtInfo->Location = System::Drawing::Point(0, 0);
				this->txtInfo->Multiline = true;
				this->txtInfo->Name = L"txtInfo";
				this->txtInfo->ReadOnly = true;
				this->txtInfo->Size = System::Drawing::Size(581, 125);
				this->txtInfo->TabIndex = 200;
				// 
				// btnShowDriveInfo
				// 
				this->btnShowDriveInfo->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
				this->btnShowDriveInfo->Location = System::Drawing::Point(312, 276);
				this->btnShowDriveInfo->Name = L"btnShowDriveInfo";
				this->btnShowDriveInfo->Size = System::Drawing::Size(94, 67);
				this->btnShowDriveInfo->TabIndex = 600;
				this->btnShowDriveInfo->Text = L"&Show Drive Info";
				this->btnShowDriveInfo->UseVisualStyleBackColor = true;
				this->btnShowDriveInfo->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Click);
				// 
				// ReadErrorDialog
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->CancelButton = this->btnCancel;
				this->ClientSize = System::Drawing::Size(605, 355);
				this->Controls->Add(this->btnShowDriveInfo);
				this->Controls->Add(this->spMain);
				this->Controls->Add(this->btnCancel);
				this->Controls->Add(this->btnGiveUpAndWZOmode);
				this->Controls->Add(this->btnGiveupAndWriteZero);
				this->Controls->Add(this->btnChangeBufferSize);
				this->Controls->Add(this->btnRetry);
				this->MaximizeBox = false;
				this->MinimizeBox = false;
				this->MinimumSize = System::Drawing::Size(621, 394);
				this->Name = L"ReadErrorDialog";
				this->ShowIcon = false;
				this->ShowInTaskbar = false;
				this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
				this->Text = L"ReadErrorDialog";
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
			}
			property int RetryCount
			{
				int get() { return retryCount_; }
			}

			READERROR_RESPONSE ShowDialogAndGetResponce(System::Windows::Forms::IWin32Window^ parent);
		private:
			System::Void btnRetry_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnChangeBufferSize_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnGiveupAndWriteZero_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnGiveUpAndWZOmode_Click(System::Object^ sender, System::EventArgs^ e);

		private: System::Void btnRetry_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
			txtInfo->Text = "1";
		}
		private: System::Void btnRetry_Enter(System::Object^ sender, System::EventArgs^ e) {
			txtInfo->Text = "2";
		}
			   System::Void btnShowDriveInfo_Click(System::Object^ sender, System::EventArgs^ e);

		};
	}
}