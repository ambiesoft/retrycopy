#pragma once


namespace retrycopy {

	enum class OVERWRITE_TYPE
	{
		OVERWRITE_YES,
		OVERWRITE_NO,
		OVERWRITE_ASK,

		OVERWRITE_COUNT,
	};
	ref class OverwriteItemInfo {
		OVERWRITE_TYPE kind_;
		System::String^ text_;
	public:
		OverwriteItemInfo(OVERWRITE_TYPE kind, System::String^ text):
			kind_(kind), text_(text){}
		property OVERWRITE_TYPE Kind {
			OVERWRITE_TYPE get() { return kind_; }
		}
		System::String^ ToString() override {
			return text_;
		}
	};

	ref class OverwriteItem sealed abstract
	{
	public:
		initonly static cli::array<OverwriteItemInfo^>^ itemInfos_ = {
			gcnew OverwriteItemInfo(OVERWRITE_TYPE::OVERWRITE_YES, I18N("Yes")),
			gcnew OverwriteItemInfo(OVERWRITE_TYPE::OVERWRITE_NO, I18N("No")),
			gcnew OverwriteItemInfo(OVERWRITE_TYPE::OVERWRITE_ASK, I18N("Ask")),
		};
		initonly static OVERWRITE_TYPE DefaultItem = OVERWRITE_TYPE::OVERWRITE_NO;
		static OverwriteItem() {
			DASSERT((int)OVERWRITE_TYPE::OVERWRITE_COUNT == itemInfos_->Length);
			for (int i = 0; i < itemInfos_->Length; ++i)
			{
				DASSERT(itemInfos_[i]->Kind == (OVERWRITE_TYPE)i);
			}
		}
		static void AddComboItem(System::Windows::Forms::ComboBox^ cmb);
	};
}
