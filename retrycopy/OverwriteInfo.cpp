#include "stdafx.h"

#include "OverwriteInfo.h"

using namespace System;

using namespace Ambiesoft;
namespace retrycopy {

	void OverwriteInfo::AddComboItem(System::Windows::Forms::ComboBox^ cmb)
	{
		DASSERT(cmb);
		DASSERT(cmb->Items->Count == 0);
		for each (OverwriteItem ^ ii in itemInfos_)
			cmb->Items->Add(ii);
	}
	void OverwriteInfo::SetComboItemFromCL(System::Windows::Forms::ComboBox^ cmb, LPCWSTR pCLValue)
	{
		for (int i = 0; i < itemInfos_->Length; ++i)
		{
			if (0 == String::Compare(itemInfos_[i]->CLValue, gcnew String(pCLValue), true))
			{
				cmb->SelectedIndex = i;
				return;
			}
		}
		CppUtils::Alert(I18N(L"-ov must be one of 'Yes', 'No' or 'Ask'"));
	}
}

