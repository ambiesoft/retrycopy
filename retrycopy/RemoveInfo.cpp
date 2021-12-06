#include "stdafx.h"

#include "RemoveInfo.h"

using namespace System;
using namespace Ambiesoft;
namespace retrycopy {

	void OperationInfo::AddComboItem(System::Windows::Forms::ComboBox^ cmb)
	{
		DASSERT(cmb);
		DASSERT(cmb->Items->Count == 0);
		for each (OperationItem ^ ii in itemInfos_)
			cmb->Items->Add(ii);
	}
	void OperationInfo::SetComboItemFromCL(System::Windows::Forms::ComboBox^ cmb, LPCWSTR pCLValue)
	{
		String^ cl = gcnew String(pCLValue);

		// default is copy
		if (String::IsNullOrEmpty(cl))
			cl = CopyOpItem->CLValue;

		for (int i = 0; i < itemInfos_->Length; ++i)
		{
			if (0 == String::Compare(itemInfos_[i]->CLValue, cl, true))
			{
				cmb->SelectedIndex = i;
				return;
			}
		}
		CppUtils::Alert(String::Format(I18N(L"-op must be one of {0}"),
			ONEOFOPERATION));
	}
	String^ OperationInfo::GetButtonTitle(System::Windows::Forms::ComboBox^ cmb)
	{
		for (int i = 0; i < itemInfos_->Length; ++i)
		{
			if (i == cmb->SelectedIndex)
			{
				return itemInfos_[i]->ButtonText;
			}
		}
		DASSERT(false);
		return nullptr;
	}
}

