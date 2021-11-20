#pragma once


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
	public:
		FormMain(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

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

	protected:


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
			this->lblSource = (gcnew System::Windows::Forms::Label());
			this->lblDest = (gcnew System::Windows::Forms::Label());
			this->txtSource = (gcnew System::Windows::Forms::TextBox());
			this->txtDestination = (gcnew System::Windows::Forms::TextBox());
			this->btnNavSource = (gcnew System::Windows::Forms::Button());
			this->btnNavDestination = (gcnew System::Windows::Forms::Button());
			this->btnCopy = (gcnew System::Windows::Forms::Button());
			this->txtLog = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// lblSource
			// 
			this->lblSource->AutoSize = true;
			this->lblSource->Location = System::Drawing::Point(12, 9);
			this->lblSource->Name = L"lblSource";
			this->lblSource->Size = System::Drawing::Size(44, 13);
			this->lblSource->TabIndex = 1;
			this->lblSource->Text = L"&Source:";
			// 
			// lblDest
			// 
			this->lblDest->AutoSize = true;
			this->lblDest->Location = System::Drawing::Point(12, 48);
			this->lblDest->Name = L"lblDest";
			this->lblDest->Size = System::Drawing::Size(63, 13);
			this->lblDest->TabIndex = 2;
			this->lblDest->Text = L"&Destination:";
			// 
			// txtSource
			// 
			this->txtSource->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtSource->Location = System::Drawing::Point(15, 25);
			this->txtSource->Name = L"txtSource";
			this->txtSource->Size = System::Drawing::Size(488, 20);
			this->txtSource->TabIndex = 3;
			// 
			// txtDestination
			// 
			this->txtDestination->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtDestination->Location = System::Drawing::Point(12, 64);
			this->txtDestination->Name = L"txtDestination";
			this->txtDestination->Size = System::Drawing::Size(491, 20);
			this->txtDestination->TabIndex = 4;
			// 
			// btnNavSource
			// 
			this->btnNavSource->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnNavSource->Location = System::Drawing::Point(509, 23);
			this->btnNavSource->Name = L"btnNavSource";
			this->btnNavSource->Size = System::Drawing::Size(29, 23);
			this->btnNavSource->TabIndex = 5;
			this->btnNavSource->Text = L"...";
			this->btnNavSource->UseVisualStyleBackColor = true;
			this->btnNavSource->Click += gcnew System::EventHandler(this, &FormMain::btnNavSource_Click);
			// 
			// btnNavDestination
			// 
			this->btnNavDestination->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnNavDestination->Location = System::Drawing::Point(509, 62);
			this->btnNavDestination->Name = L"btnNavDestination";
			this->btnNavDestination->Size = System::Drawing::Size(29, 23);
			this->btnNavDestination->TabIndex = 5;
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
			this->btnCopy->Size = System::Drawing::Size(526, 23);
			this->btnCopy->TabIndex = 6;
			this->btnCopy->Text = L"&Copy";
			this->btnCopy->UseVisualStyleBackColor = true;
			this->btnCopy->Click += gcnew System::EventHandler(this, &FormMain::btnCopy_Click);
			// 
			// txtLog
			// 
			this->txtLog->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtLog->Location = System::Drawing::Point(15, 119);
			this->txtLog->Multiline = true;
			this->txtLog->Name = L"txtLog";
			this->txtLog->Size = System::Drawing::Size(523, 175);
			this->txtLog->TabIndex = 7;
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(550, 306);
			this->Controls->Add(this->txtLog);
			this->Controls->Add(this->btnCopy);
			this->Controls->Add(this->btnNavDestination);
			this->Controls->Add(this->btnNavSource);
			this->Controls->Add(this->txtDestination);
			this->Controls->Add(this->txtSource);
			this->Controls->Add(this->lblDest);
			this->Controls->Add(this->lblSource);
			this->Name = L"FormMain";
			this->Text = L"retrycopy";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		System::Threading::Thread^ theThread_;
		HANDLE hSource_;
		HANDLE hDestination_;
		void ClearThread();
		void StartOfThread(Object^ obj);
	private:
		System::Void btnNavSource_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnNavDestination_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnCopy_Click(System::Object^ sender, System::EventArgs^ e);
	
};
};


