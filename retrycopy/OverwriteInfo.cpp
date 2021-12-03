#include "stdafx.h"

#include "OverwriteInfo.h"

using namespace System;

using namespace Ambiesoft;
namespace retrycopy {

	void OverwriteItem::AddComboItem(System::Windows::Forms::ComboBox^ cmb)
	{
		DASSERT(cmb);
		DASSERT(cmb->Items->Count == 0);
		for each (OverwriteItemInfo ^ ii in itemInfos_)
			cmb->Items->Add(ii);
	}
	void OverwriteItem::SetComboItemFromCL(System::Windows::Forms::ComboBox^ cmb, LPCWSTR pCLValue)
	{
		for (int i = 0; i < itemInfos_->Length; ++i)
		{
			if (itemInfos_[i]->CLValue == gcnew String(pCLValue))
			{
				cmb->SelectedIndex = i;
				return;
			}
		}
		CppUtils::Alert(I18N(L"-ov must be one of 'Yes', 'No' or 'Ask'"));
	}
}

