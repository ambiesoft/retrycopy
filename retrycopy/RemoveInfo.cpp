#include "stdafx.h"

#include "RemoveInfo.h"

using namespace System;
using namespace Ambiesoft;
namespace retrycopy {

	void RemoveInfo::AddComboItem(System::Windows::Forms::ComboBox^ cmb)
	{
		DASSERT(cmb);
		DASSERT(cmb->Items->Count == 0);
		for each (RemoveItemInfo ^ ii in itemInfos_)
			cmb->Items->Add(ii);
	}
	void RemoveInfo::SetComboItemFromCL(System::Windows::Forms::ComboBox^ cmb, LPCWSTR pCLValue)
	{
		for (int i = 0; i < itemInfos_->Length; ++i)
		{
			if (itemInfos_[i]->CLValue == gcnew String(pCLValue))
			{
				cmb->SelectedIndex = i;
				return;
			}
		}
		CppUtils::Alert(I18N(L"-rm must be one of 'YesRecycle', 'YesDelete', 'No' or 'Ask'"));
	}
}

