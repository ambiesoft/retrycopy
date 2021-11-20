
#include "stdafx.h"
#include "FormMain.h"

using namespace retrycopy;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	FormMain f;
	f.ShowDialog();
	return 0;
}
