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
	eSuccess = 0,
	eBadArgs = 1,
	eTimedOut = 2,
	eClickedBallon = 3,
	eClosedBallon = 4,
	eUnknown = 255
};

//int _tmain(int argc, const TCHAR* argv[])
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

