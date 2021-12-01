#pragma once


namespace retrycopy {

	enum class REMOVE_TYPE
	{
		REMOVE_YES_RECYCLE,
		REMOVE_YES_DELETE,
		REMOVE_NO,
		REMOVE_ASK,

		REMOVE_COUNT,
	};
	ref class RemoveItemInfo {
		REMOVE_TYPE kind_;
		System::String^ text_;
	public:
		RemoveItemInfo(REMOVE_TYPE kind, System::String^ text):
			kind_(kind), text_(text){}
		property REMOVE_TYPE Kind {
			REMOVE_TYPE get() { return kind_; }
		}
		System::String^ ToString() override {
			return text_;
		}
	};

	ref class RemoveInfo sealed abstract
	{
	public:
		initonly static cli::array<RemoveItemInfo^>^ itemInfos_ = {
			gcnew RemoveItemInfo(REMOVE_TYPE::REMOVE_YES_RECYCLE, I18N("Yes (Recycle)")),
			gcnew RemoveItemInfo(REMOVE_TYPE::REMOVE_YES_DELETE, I18N("Yes (Delete)")),
			gcnew RemoveItemInfo(REMOVE_TYPE::REMOVE_NO, I18N("No")),
			gcnew RemoveItemInfo(REMOVE_TYPE::REMOVE_ASK, I18N("Ask")),
		};
		initonly static REMOVE_TYPE DefaultItem = REMOVE_TYPE::REMOVE_NO;
		static RemoveInfo() {
			DASSERT((int)REMOVE_TYPE::REMOVE_COUNT == itemInfos_->Length);
			for (int i = 0; i < itemInfos_->Length; ++i)
			{
				DASSERT(itemInfos_[i]->Kind == (REMOVE_TYPE)i);
			}
		}
		static void AddComboItem(System::Windows::Forms::ComboBox^ cmb);
	};
}
