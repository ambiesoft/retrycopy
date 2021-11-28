#pragma once

namespace retrycopy {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FormLog
	/// </summary>
	public ref class FormLog : public System::Windows::Forms::Form
	{
	public:
		FormLog(void)
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
		~FormLog()
		{
			if (components)
			{
				delete components;
			}
		}
	internal: System::Windows::Forms::TextBox^ txtLog;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->txtLog = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// txtLog
			// 
			this->txtLog->Dock = System::Windows::Forms::DockStyle::Fill;
			this->txtLog->Location = System::Drawing::Point(0, 0);
			this->txtLog->Multiline = true;
			this->txtLog->Name = L"txtLog";
			this->txtLog->ReadOnly = true;
			this->txtLog->Size = System::Drawing::Size(532, 219);
			this->txtLog->TabIndex = 0;
			// 
			// FormLog
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(532, 219);
			this->Controls->Add(this->txtLog);
			this->Name = L"FormLog";
			this->Text = L"FormLog";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
