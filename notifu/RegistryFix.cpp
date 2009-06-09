#include "StdAfx.h"
#include "RegistryFix.h"


//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced:EnableBalloonTips=1
LONG EnableBalloonTips(bool enable)
{
	LONG result = 0;
	HKEY reg;

	result = RegOpenKey(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", &reg);

	if(result == ERROR_SUCCESS)
	{
		DWORD enable_balloon_tips = enable ? 1 : 0;

		result = RegSetValueEx(reg, L"EnableBalloonTips", 0, REG_DWORD, (const BYTE*)&enable_balloon_tips, sizeof enable_balloon_tips);
	}

	return result;
}
