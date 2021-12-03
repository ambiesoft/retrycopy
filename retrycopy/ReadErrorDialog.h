#pragma once

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
	public:
		ReadErrorDialog(String^ message, int bufferSize, int retryCount)
		{
			InitializeComponent();

			txtMessage->Text = message;
			bufferSize_ = bufferSize;
			initialBufferSize_ = bufferSize;
			retryCount_ = retryCount;
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
			this->SuspendLayout();
			// 
			// txtMessage
			// 
			this->txtMessage->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtMessage->Location = System::Drawing::Point(12, 12);
			this->txtMessage->Multiline = true;
			this->txtMessage->Name = L"txtMessage";
			this->txtMessage->ReadOnly = true;
			this->txtMessage->Size = System::Drawing::Size(582, 101);
			this->txtMessage->TabIndex = 0;
			// 
			// btnRetry
			// 
			this->btnRetry->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->btnRetry->Location = System::Drawing::Point(12, 167);
			this->btnRetry->Name = L"btnRetry";
			this->btnRetry->Size = System::Drawing::Size(113, 67);
			this->btnRetry->TabIndex = 1;
			this->btnRetry->Text = L"&Retry";
			this->btnRetry->UseVisualStyleBackColor = true;
			this->btnRetry->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnRetry_Click);
			// 
			// btnChangeBufferSize
			// 
			this->btnChangeBufferSize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->btnChangeBufferSize->Location = System::Drawing::Point(131, 167);
			this->btnChangeBufferSize->Name = L"btnChangeBufferSize";
			this->btnChangeBufferSize->Size = System::Drawing::Size(113, 67);
			this->btnChangeBufferSize->TabIndex = 2;
			this->btnChangeBufferSize->Text = L"Change Buffer size";
			this->btnChangeBufferSize->UseVisualStyleBackColor = true;
			this->btnChangeBufferSize->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnChangeBufferSize_Click);
			// 
			// btnGiveupAndWriteZero
			// 
			this->btnGiveupAndWriteZero->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->btnGiveupAndWriteZero->Location = System::Drawing::Point(250, 167);
			this->btnGiveupAndWriteZero->Name = L"btnGiveupAndWriteZero";
			this->btnGiveupAndWriteZero->Size = System::Drawing::Size(113, 67);
			this->btnGiveupAndWriteZero->TabIndex = 3;
			this->btnGiveupAndWriteZero->Text = L"Give up to read and write zero";
			this->btnGiveupAndWriteZero->UseVisualStyleBackColor = true;
			this->btnGiveupAndWriteZero->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnGiveupAndWriteZero_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnCancel->Location = System::Drawing::Point(495, 191);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(99, 43);
			this->btnCancel->TabIndex = 4;
			this->btnCancel->Text = L"Cancel";
			this->btnCancel->UseVisualStyleBackColor = true;
			// 
			// btnGiveUpAndWZOmode
			// 
			this->btnGiveUpAndWZOmode->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->btnGiveUpAndWZOmode->Location = System::Drawing::Point(369, 167);
			this->btnGiveUpAndWZOmode->Name = L"btnGiveUpAndWZOmode";
			this->btnGiveUpAndWZOmode->Size = System::Drawing::Size(113, 67);
			this->btnGiveUpAndWZOmode->TabIndex = 3;
			this->btnGiveUpAndWZOmode->Text = L"Give up to read 1 byte and write zero until it succeed";
			this->btnGiveUpAndWZOmode->UseVisualStyleBackColor = true;
			this->btnGiveUpAndWZOmode->Click += gcnew System::EventHandler(this, &ReadErrorDialog::btnGiveUpAndWZOmode_Click);
			// 
			// ReadErrorDialog
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->btnCancel;
			this->ClientSize = System::Drawing::Size(606, 246);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->btnGiveUpAndWZOmode);
			this->Controls->Add(this->btnGiveupAndWriteZero);
			this->Controls->Add(this->btnChangeBufferSize);
			this->Controls->Add(this->btnRetry);
			this->Controls->Add(this->txtMessage);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"ReadErrorDialog";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"ReadErrorDialog";
			this->ResumeLayout(false);
			this->PerformLayout();

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

};
}
