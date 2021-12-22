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
		/// FormAbout の概要
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
			/// 使用中のリソースをすべてクリーンアップします。
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
		private: System::Windows::Forms::LinkLabel^ linkHomepage;
		private: System::Windows::Forms::LinkLabel^ linkDonation;
		protected:

		private:
			/// <summary>
			/// 必要なデザイナー変数です。
			/// </summary>
			System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
			/// <summary>
			/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
			/// コード エディターで変更しないでください。
			/// </summary>
			void InitializeComponent(void)
			{
				System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(FormAbout::typeid));
				this->lblAppAndVersion = (gcnew System::Windows::Forms::Label());
				this->btnOK = (gcnew System::Windows::Forms::Button());
				this->linkHomepage = (gcnew System::Windows::Forms::LinkLabel());
				this->linkDonation = (gcnew System::Windows::Forms::LinkLabel());
				this->SuspendLayout();
				// 
				// lblAppAndVersion
				// 
				resources->ApplyResources(this->lblAppAndVersion, L"lblAppAndVersion");
				this->lblAppAndVersion->Name = L"lblAppAndVersion";
				// 
				// btnOK
				// 
				resources->ApplyResources(this->btnOK, L"btnOK");
				this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->btnOK->Name = L"btnOK";
				this->btnOK->UseVisualStyleBackColor = true;
				this->btnOK->Click += gcnew System::EventHandler(this, &FormAbout::btnOK_Click);
				// 
				// linkHomepage
				// 
				resources->ApplyResources(this->linkHomepage, L"linkHomepage");
				this->linkHomepage->Name = L"linkHomepage";
				this->linkHomepage->TabStop = true;
				this->linkHomepage->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &FormAbout::linkHomepage_LinkClicked);
				// 
				// linkDonation
				// 
				resources->ApplyResources(this->linkDonation, L"linkDonation");
				this->linkDonation->Name = L"linkDonation";
				this->linkDonation->TabStop = true;
				this->linkDonation->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &FormAbout::linkDonation_LinkClicked);
				// 
				// FormAbout
				// 
				this->AcceptButton = this->btnOK;
				resources->ApplyResources(this, L"$this");
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->Controls->Add(this->linkDonation);
				this->Controls->Add(this->linkHomepage);
				this->Controls->Add(this->btnOK);
				this->Controls->Add(this->lblAppAndVersion);
				this->MaximizeBox = false;
				this->MinimizeBox = false;
				this->Name = L"FormAbout";
				this->ShowIcon = false;
				this->ShowInTaskbar = false;
				this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormAbout::FormAbout_FormClosing);
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
		private:
			System::Void btnOK_Click(System::Object^ sender, System::EventArgs^ e) {
				Close();
			}
			System::Void FormAbout_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
			System::Void linkHomepage_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e);
			System::Void linkDonation_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e);
		
};
	}
}