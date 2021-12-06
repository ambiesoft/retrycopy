#pragma once

using namespace System;

namespace retrycopy {

	enum class OVERWRITE_TYPE
	{
		OVERWRITE_YES,
		OVERWRITE_NO,
		OVERWRITE_ASK,

		OVERWRITE_COUNT,
	};
	ref class OverwriteItem {
		initonly OVERWRITE_TYPE kind_;
		initonly String^ text_;
		initonly String^ clValue_;
	public:
		OverwriteItem(OVERWRITE_TYPE kind, String^ text, String^ clValue):
			kind_(kind), text_(text), clValue_(clValue) {}
		property OVERWRITE_TYPE Kind {
			OVERWRITE_TYPE get() { return kind_; }
		}
		property String^ CLValue {
			String^ get() { return clValue_; }
		}
		String^ ToString() override {
			return text_;
		}
	};

	ref class OverwriteInfo sealed abstract
	{
	public:
		initonly static String^ ONEOFOPERATION = I18N("'yes', 'no', or 'ask'");
		initonly static cli::array<OverwriteItem^>^ itemInfos_ = {
			gcnew OverwriteItem(OVERWRITE_TYPE::OVERWRITE_YES, I18N("Yes"), "yes"),
			gcnew OverwriteItem(OVERWRITE_TYPE::OVERWRITE_NO, I18N("No"), "no"),
			gcnew OverwriteItem(OVERWRITE_TYPE::OVERWRITE_ASK, I18N("Ask"), "ask"),
		};
		initonly static OVERWRITE_TYPE DefaultItem = OVERWRITE_TYPE::OVERWRITE_NO;
		static OverwriteInfo() {
			DASSERT((int)OVERWRITE_TYPE::OVERWRITE_COUNT == itemInfos_->Length);
			for (int i = 0; i < itemInfos_->Length; ++i)
			{
				DASSERT(itemInfos_[i]->Kind == (OVERWRITE_TYPE)i);
			}
		}
		static void AddComboItem(System::Windows::Forms::ComboBox^ cmb);
		static void SetComboItemFromCL(System::Windows::Forms::ComboBox^ cmb, LPCWSTR pCLValue);
	};
}
