
#include "stdafx.h"
#include "../../lsMisc/CommandLineParser.h"
#include "gitrev.h"
#include "FormMain.h"

using namespace retrycopy;
using namespace Ambiesoft;
using namespace std;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	CCommandLineParser parser;

	wstring source;
	parser.AddOption(L"-s", ArgCount::ArgCount_One, &source,
		ArgEncodingFlags::ArgEncodingFlags_Default,
		I18N(L"Source"));

	wstring dest;
	parser.AddOption(L"-d", ArgCount::ArgCount_One, &dest,
		ArgEncodingFlags::ArgEncodingFlags_Default,
		I18N(L"Destination"));

	bool bShowGitRev = false;
	parser.AddOption(L"--show-gitrev", 0, &bShowGitRev,
		ArgEncodingFlags::ArgEncodingFlags_Default,
		I18N(L"Show Gitrev"));

	parser.Parse();

	if (bShowGitRev)
	{
		MessageBox::Show(
			gcnew String(GITREV::GetHashMessage().c_str()),
			L"Gitrev - " + Application::ProductName,
			MessageBoxButtons::OK,
			MessageBoxIcon::Information);
		return 0;
	}
	// Create the main window and run it
	FormMain f(gcnew String(source.c_str()), gcnew String(dest.c_str()));
	f.ShowDialog();
	return 0;
}
