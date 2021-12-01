#include "stdafx.h"

#include "RemoveInfo.h"

using namespace System;
namespace retrycopy {

	void RemoveInfo::AddComboItem(System::Windows::Forms::ComboBox^ cmb)
	{
		DASSERT(cmb);
		DASSERT(cmb->Items->Count == 0);
		for each (RemoveItemInfo ^ ii in itemInfos_)
			cmb->Items->Add(ii);
	}
}

