#pragma once

namespace retrycopy {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// FormAbout �̊T�v
	/// </summary>
	public ref class FormAbout : public System::Windows::Forms::Form
	{
	public:
		FormAbout(void)
		{
			InitializeComponent();

			this->Text = String::Format(I18N(L"About {0}"), Application::ProductName);
			lblAppAndVersion->Text = String::Format(L"{0} v{1}",
				Application::ProductName,
				Ambiesoft::AmbLib::getAssemblyVersion(System::Reflection::Assembly::GetExecutingAssembly(), 3));
		}

	protected:
		/// <summary>
		/// �g�p���̃��\�[�X�����ׂăN���[���A�b�v���܂��B
		/// </summary>
		~FormAbout()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ lblAppAndVersion;
	private: System::Windows::Forms::Button^ btnOK;
	protected:

	private:
		/// <summary>
		/// �K�v�ȃf�U�C�i�[�ϐ��ł��B
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �f�U�C�i�[ �T�|�[�g�ɕK�v�ȃ��\�b�h�ł��B���̃��\�b�h�̓��e��
		/// �R�[�h �G�f�B�^�[�ŕύX���Ȃ��ł��������B
		/// </summary>
		void InitializeComponent(void)
		{
			this->lblAppAndVersion = (gcnew System::Windows::Forms::Label());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// lblAppAndVersion
			// 
			this->lblAppAndVersion->AutoSize = true;
			this->lblAppAndVersion->Location = System::Drawing::Point(12, 22);
			this->lblAppAndVersion->Name = L"lblAppAndVersion";
			this->lblAppAndVersion->Size = System::Drawing::Size(96, 12);
			this->lblAppAndVersion->TabIndex = 0;
			this->lblAppAndVersion->Text = L"lblAppAndVersion";
			// 
			// btnOK
			// 
			this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnOK->Location = System::Drawing::Point(305, 104);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 1;
			this->btnOK->Text = L"OK";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &FormAbout::btnOK_Click);
			// 
			// FormAbout
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(392, 139);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->lblAppAndVersion);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"FormAbout";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->Text = L"FormAbout";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormAbout::FormAbout_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: 
		System::Void btnOK_Click(System::Object^ sender, System::EventArgs^ e) {
			Close();
		}
		System::Void FormAbout_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
			e->Cancel = true;
			Visible = false;
		}
	};
}
