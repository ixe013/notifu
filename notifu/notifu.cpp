// notifu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CommandLine.h"
#include "NotifyUser.h"
#include "RegistryFix.h"
#include "Serialize.h"

CAppModule _Module;

enum
{
	//I know the compiler numbers enums, but I like source that
	//is its own documentation...
	eSuccess = 0, /// The command line was understood and executed properly
	eBadArgs = 1, /// Some parameters were bad or missing.
	eTimedOut = 2, /// Implies eSuccess, but a timeout was requested and the ballon was dismissed because of that
	eClickedBallon = 3, /// Implies eSuccess, but the user closed the ballon to close it, ending the program
	eClosedBallon = 4, /// Implies eSuccess, but the user closed the upper right corner of the ballon to close it, ending the program
	eUnknown = 255
};

/* -------------------------------------------------------------------------- */
/** @brief The entry point of the program (it is command line).
* 
* @param hInstance This module's instance
* @param hPrevInstance Legacy parameter, always 0
* @param lpCmdLine The command line parameters. They will be sent to the command
*                  line parser object
* @param nCmdShow  Recommemded way of showing a window that does not exist in
*                  this program. It is not used.
* 
* @return One of the error codes documeneted in the enum above.
*/
/* ---------------------------------------------------------------------------- */
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	int errorlevel = eUnknown;
			    
	gCommandLine.Setup();

	gCommandLine.Parse(lpCmdLine);

	if(gCommandLine.Fix)
	{
		if(EnableBalloonTips() == ERROR_SUCCESS)
		{
			if(gCommandLine.HasErrors())
			{
				//just wanted to fix the registry, that's ok
				errorlevel = eSuccess;
				gCommandLine.Show(_T("Balloon tips were enabled in the registry.\n\nThe key EnableBalloonTips=1 was added under HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced"));
				goto end;
			}
		}
		else
		{
			gCommandLine.Show(_T("Unable to enable balloon tips in the registry.\n\nPlease add EnableBalloonTips=1 under HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced"));
		}
	}

	if(gCommandLine.Continue())
	{
		HRESULT result;
		CoInitialize(0);

		NOTIFU_PARAM params;

		gCommandLine.CopyCommandLineToParams(params);

		SerializeEnter();

		result = NotifyUser(params);

		SerializeLeave();

		DestroyIcon(params.mIcon);

		switch(result)
		{
		case 0x800704C7 : errorlevel = eClosedBallon; break;
		case S_OK :	errorlevel = eClickedBallon; break;
		case S_FALSE :	errorlevel = eTimedOut; break;
		}

		CoUninitialize();
	}
	else
	{
		errorlevel = eBadArgs;
	}

end:

	return errorlevel;
}

