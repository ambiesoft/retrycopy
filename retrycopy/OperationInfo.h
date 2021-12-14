#pragma once

using namespace System;
namespace Ambiesoft {
	namespace retrycopy {

		enum class OPERATION
		{
			COPY,
			MOVE,
			MOVERECYCLE,
			ASK,
			OPERATION_COUNT,
		};
		ref class OperationItem {
			initonly OPERATION kind_;
			initonly String^ text_;
			initonly String^ btnText_;
			initonly String^ mainTitle_;
			initonly String^ clValue_;
		public:
			OperationItem(OPERATION kind, String^ text, String^ btnText, String^ mainTitle, String^ clValue) :
				kind_(kind), text_(text), btnText_(btnText), mainTitle_(mainTitle), clValue_(clValue) {}
			property OPERATION Kind {
				OPERATION get() { return kind_; }
			}
			property String^ CLValue {
				String^ get() { return clValue_; }
			}
			property String^ ButtonText {
				String^ get() { return btnText_; }
			}
			property String^ MainTitle {
				String^ get() { return mainTitle_; }
			}
			String^ ToString() override {
				return text_;
			}
		};

		ref class OperationInfo sealed abstract
		{
		public:
			initonly static String^ ONEOFOPERATION = I18N(L"'copy', 'move', 'moverecycle', or 'ask'");
			initonly static cli::array<OperationItem^>^ itemInfos_ = {
				gcnew OperationItem(
					OPERATION::COPY, 
					I18N(L"Copy"), 
					I18N(L"&Copy"), 
					I18N("Copying"), 
					"copy"),
				gcnew OperationItem(
					OPERATION::MOVE,
					I18N(L"Move"),
					I18N(L"&Move"),
					I18N("Moving"),
					"move"),
				gcnew OperationItem(
					OPERATION::MOVERECYCLE, 
					I18N(L"Move (Recycle)"),
					I18N("Moving (recycle)"),
					I18N(L"Move (&Recyle)"),
					"moverecycle"),
				gcnew OperationItem(
					OPERATION::ASK,
					I18N("Ask"), 
					I18N(L"&Ask"), 
					I18N("Moving (ask)"),
					"ask"),
			};
			initonly static OPERATION DefaultItem = OPERATION::COPY;
			static OperationInfo() {
				DASSERT((int)OPERATION::OPERATION_COUNT == itemInfos_->Length);
				for (int i = 0; i < itemInfos_->Length; ++i)
				{
					DASSERT(itemInfos_[i]->Kind == (OPERATION)i);
				}
			}
			static property OperationItem^ CopyOpItem
			{
				OperationItem^ get() {
					for each (OperationItem ^ oi in itemInfos_) {
						if (oi->Kind == OPERATION::COPY)
							return oi;
					}
					return nullptr;
				}
			}
			static void AddComboItem(System::Windows::Forms::ComboBox^ cmb);
			static void SetComboItemFromCL(System::Windows::Forms::ComboBox^ cmb, LPCWSTR pCLValue);
			static String^ GetButtonTitle(System::Windows::Forms::ComboBox^ cmb);
			static String^ GetMainTitle(System::Windows::Forms::ComboBox^ cmb);
		};
	}
}