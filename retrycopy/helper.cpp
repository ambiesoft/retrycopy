#include "stdafx.h"

#include "helper.h"

using namespace System;
using namespace System::Text;
using namespace System::Threading;
using namespace System::IO;
using namespace Ambiesoft;
namespace retrycopy {

	String^ SizeToUser(LONGLONG size)
	{
		return String::Format(I18N(L"{0} ({1})"),
			size,
			AmbLib::FormatSize(size));
	}

} // namespace