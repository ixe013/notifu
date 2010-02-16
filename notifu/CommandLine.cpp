// CommandLine.cpp: implementation of the CCommandLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandLine.h"
#include "resource.h"
#include "Icon.h"

#ifndef NIIF_RESPECT_QUIET_TIME 
#define NIIF_RESPECT_QUIET_TIME 0x00000080
#endif

CCommandLine gCommandLine;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandLine::CCommandLine()
: Type(L"t", L"The type of message to display")
, Delay(L"d", L"The number of milliseconds to display (omit or 0 for infinit)")
, Title(L"p", L"The title (or prompt) of the ballon")
, Text(L"m", L"The message text")
, Fix(L"e", L"Enable ballon tips in the registry (for this user only)")
, NoSound(L"q", L"Do not play a sound when the tooltip is displayed")
, NoRespect(L"w", L"Show the tooltip even if the user is in the quiet period that follows his very first login (Windows 7 and up)")
, ForceXP(L"xp", L"Force WindowsXP ballon tips behavior")
, IconFileName(L"i", L"Specify an icon to use ('parent' uses the icon of the parent process)")
{
}


void CCommandLine::Setup()
{
	Type.AddValue(L"info", L"The message is an informational message");
	Type.AddValue(L"warn", L"The message is an warning message");
	Type.AddValue(L"error", L"The message is an error message");

	Delay.SetDefaultValue(0);
	Type.SetDefaultValue(L"info");
	//Title.SetDefaultValue(L"Notifu message");
	IconFileName.SetDefaultValue(L"parent"); 

	AddFlag(Type);
	AddFlag(Delay);
	AddFlag(Title);
	AddFlag(Text);
	AddFlag(IconFileName);
	AddFlag(Fix);
	AddFlag(NoSound);
	AddFlag(NoRespect);
	AddFlag(ForceXP);
}

HRESULT CCommandLine::CopyCommandLineToParams(NOTIFU_PARAM& params)
{
	params.mDelay = Delay.Value();

	if(params.mDelay < 120)
		params.mDelay *= 1000;

	if(Type.Value() == L"warn")
		params.mType = NIIF_WARNING;
	else if(Type.Value() == L"error")
		params.mType = NIIF_ERROR;
	else
		params.mType = NIIF_INFO;

	if(NoSound)
		params.mType |= NIIF_NOSOUND;

	if(!NoRespect)
		params.mType |= NIIF_RESPECT_QUIET_TIME;

   params.mForceXP = ForceXP;

	params.mIcon = GoFindAnIcon(IconFileName.Value().c_str());

	if(params.mIcon == 0)
		params.mIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_NOTIFU));

	std::vector<tstring>::const_iterator i;

	if(Title.size() == 0)
		Title.ConsumeValue(L"Notifu message");

	for(i=Title.Values().begin(); i!=Title.Values().end(); ++i)
	{
		params.mTitle += *i;
	}

	for(i=Text.Values().begin(); i!=Text.Values().end(); ++i)
	{
		params.mText += *i;
	}

	return S_OK;
}
