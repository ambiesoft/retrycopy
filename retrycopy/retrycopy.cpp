
#include "stdafx.h"

#include "FormMain.h"

using namespace System::Windows::Forms;
using namespace Ambiesoft;
using namespace Ambiesoft::retrycopy;
using namespace std;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	InitHighDPISupport();
	
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	//FormMain f;
	//f.ShowDialog();
	Application::Run(gcnew FormMain());
	return 0;
}
