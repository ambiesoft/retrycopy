#include "stdafx.h"

#include "OverwriteInfo.h"

using namespace System;
namespace retrycopy {

	void OverwriteItem::AddComboItem(System::Windows::Forms::ComboBox^ cmb)
	{
		DASSERT(cmb);
		DASSERT(cmb->Items->Count == 0);
		for each (OverwriteItemInfo ^ ii in itemInfos_)
			cmb->Items->Add(ii);
	}
}

