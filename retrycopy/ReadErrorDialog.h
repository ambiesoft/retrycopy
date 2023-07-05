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
		private: System::Windows::Forms::Button^ btnGiveUpAndAllWZOmode;


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
				System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(ReadErrorDialog::typeid));
				this->spMain = (gcnew System::Windows::Forms::SplitContainer());
				this->txtMessage = (gcnew System::Windows::Forms::TextBox());
				this->txtInfo = (gcnew System::Windows::Forms::TextBox());
				this->btnRetry = (gcnew System::Windows::Forms::Button());
				this->btnChangeBufferSize = (gcnew System::Windows::Forms::Button());
				this->btnGiveupAndWriteZero = (gcnew System::Windows::Forms::Button());
				this->btnGiveUpAndWZOmode = (gcnew System::Windows::Forms::Button());
				this->btnShowDriveInfo = (gcnew System::Windows::Forms::Button());
				this->btnChangeRetryCount = (gcnew System::Windows::Forms::Button());
				this->btnGiveUpAndAllWZOmode = (gcnew System::Windows::Forms::Button());
				(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->spMain))->BeginInit();
				this->spMain->Panel1->SuspendLayout();
				this->spMain->Panel2->SuspendLayout();
				this->spMain->SuspendLayout();
				this->SuspendLayout();
				// 
				// spMain
				// 
				resources->ApplyResources(this->spMain, L"spMain");
				this->spMain->Name = L"spMain";
				// 
				// spMain.Panel1
				// 
				resources->ApplyResources(this->spMain->Panel1, L"spMain.Panel1");
				this->spMain->Panel1->Controls->Add(this->txtMessage);
				// 
				// spMain.Panel2
				// 
				resources->ApplyResources(this->spMain->Panel2, L"spMain.Panel2");
				this->spMain->Panel2->Controls->Add(this->txtInfo);
				// 
				// txtMessage
				// 
				resources->ApplyResources(this->txtMessage, L"txtMessage");
				this->txtMessage->Name = L"txtMessage";
				this->txtMessage->ReadOnly = true;
				// 
				// txtInfo
				// 
				resources->ApplyResources(this->txtInfo, L"txtInfo");
				this->txtInfo->Name = L"txtInfo";
				this->txtInfo->ReadOnly = true;
				// 
				// btnRetry
				// 
				resources->ApplyResources(this->btnRetry, L"btnRetry");
				this->btnRetry->Name = L"btnRetry";
				this->btnRetry->UseVisualStyleBackColor = true;
				this->btnRetry->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnRetry_Click);
				this->btnRetry->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnRetry->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// btnChangeBufferSize
				// 
				resources->ApplyResources(this->btnChangeBufferSize, L"btnChangeBufferSize");
				this->btnChangeBufferSize->Name = L"btnChangeBufferSize";
				this->btnChangeBufferSize->UseVisualStyleBackColor = true;
				this->btnChangeBufferSize->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnChangeBufferSize_Click);
				this->btnChangeBufferSize->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnChangeBufferSize->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// btnGiveupAndWriteZero
				// 
				resources->ApplyResources(this->btnGiveupAndWriteZero, L"btnGiveupAndWriteZero");
				this->btnGiveupAndWriteZero->Name = L"btnGiveupAndWriteZero";
				this->btnGiveupAndWriteZero->UseVisualStyleBackColor = true;
				this->btnGiveupAndWriteZero->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnGiveupAndWriteZero_Click);
				this->btnGiveupAndWriteZero->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnGiveupAndWriteZero->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// btnGiveUpAndWZOmode
				// 
				resources->ApplyResources(this->btnGiveUpAndWZOmode, L"btnGiveUpAndWZOmode");
				this->btnGiveUpAndWZOmode->Name = L"btnGiveUpAndWZOmode";
				this->btnGiveUpAndWZOmode->UseVisualStyleBackColor = true;
				this->btnGiveUpAndWZOmode->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnGiveUpAndWZOmode_Click);
				this->btnGiveUpAndWZOmode->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnGiveUpAndWZOmode->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// btnShowDriveInfo
				// 
				resources->ApplyResources(this->btnShowDriveInfo, L"btnShowDriveInfo");
				this->btnShowDriveInfo->Name = L"btnShowDriveInfo";
				this->btnShowDriveInfo->UseVisualStyleBackColor = true;
				this->btnShowDriveInfo->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Click);
				this->btnShowDriveInfo->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnShowDriveInfo->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// btnChangeRetryCount
				// 
				resources->ApplyResources(this->btnChangeRetryCount, L"btnChangeRetryCount");
				this->btnChangeRetryCount->Name = L"btnChangeRetryCount";
				this->btnChangeRetryCount->UseVisualStyleBackColor = true;
				this->btnChangeRetryCount->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnChangeRetryCount_Click);
				this->btnChangeRetryCount->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnChangeRetryCount->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// btnGiveUpAndAllWZOmode
				// 
				resources->ApplyResources(this->btnGiveUpAndAllWZOmode, L"btnGiveUpAndAllWZOmode");
				this->btnGiveUpAndAllWZOmode->Name = L"btnGiveUpAndAllWZOmode";
				this->btnGiveUpAndAllWZOmode->UseVisualStyleBackColor = true;
				this->btnGiveUpAndAllWZOmode->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnGiveUpAndAllWZOmode_Click);
				this->btnGiveUpAndAllWZOmode->Enter += gcnew System::EventHandler(this, &ReadErrorDialog::btnShowDriveInfo_Enter);
				this->btnGiveUpAndAllWZOmode->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ReadErrorDialog::btnShowDriveInfo_MouseMove);
				// 
				// ReadErrorDialog
				// 
				resources->ApplyResources(this, L"$this");
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->Controls->Add(this->btnGiveUpAndAllWZOmode);
				this->Controls->Add(this->btnShowDriveInfo);
				this->Controls->Add(this->spMain);
				this->Controls->Add(this->btnGiveUpAndWZOmode);
				this->Controls->Add(this->btnGiveupAndWriteZero);
				this->Controls->Add(this->btnChangeRetryCount);
				this->Controls->Add(this->btnChangeBufferSize);
				this->Controls->Add(this->btnRetry);
				this->MaximizeBox = false;
				this->MinimizeBox = false;
				this->Name = L"ReadErrorDialog";
				this->ShowIcon = false;
				this->ShowInTaskbar = false;
				this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ReadErrorDialog::ReadErrorDialog_FormClosing);
				this->Load += gcnew System::EventHandler(this, &ReadErrorDialog::ReadErrorDialog_Load);
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

			void onClickWZOmodeCommon(const bool bAll);
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
			System::Void btnGiveUpAndAllWZOmode_Click(System::Object^ sender, System::EventArgs^ e);

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
			System::Void ReadErrorDialog_Load(System::Object^ sender, System::EventArgs^ e);
		};
	}
}