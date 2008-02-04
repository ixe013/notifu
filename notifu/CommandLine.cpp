// CommandLine.cpp: implementation of the CCommandLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandLine.h"
#include "resource.h"
#include "Icon.h"

CCommandLine gCommandLine;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandLine::CCommandLine()
: Type(_T("t"), _T("The type of message to display"))
, Delay(_T("d"), _T("The number of milliseconds to display (omit or 0 for infinit)"))
, Title(_T("p"), _T("The title (or prompt) of the ballon"))
, Text(_T("m"), _T("The message text"))
, Fix(_T("e"), _T("Enable ballon tips in the registry (for this user only)"))
, IconFileName(_T("i"), _T("Specify an icon to use (\"parent\" uses the icon of the parent process)"))
{
}


void CCommandLine::Setup()
{
	Type.AddValue(_T("info"), _T("The message is an informational message"));
	Type.AddValue(_T("warn"), _T("The message is an warning message"));
	Type.AddValue(_T("error"), _T("The message is an error message"));

	Delay.SetDefaultValue(0);
	Type.SetDefaultValue(_T("info"));
	Title.SetDefaultValue(_T("Notifu message"));
	IconFileName.SetDefaultValue(_T("parent")); 

	AddFlag(Type);
	AddFlag(Delay);
	AddFlag(Title);
	AddFlag(Text);
	AddFlag(IconFileName);
	AddFlag(Fix);
}

HRESULT CCommandLine::CopyCommandLineToParams(NOTIFU_PARAM& params)
{
	params.mDelay = Delay.Value();

	if(params.mDelay < 120)
		params.mDelay *= 1000;

	if(Type.Value() == _T("warn"))
		params.mType = NIIF_WARNING;
	else if(Type.Value() == _T("error"))
		params.mType = NIIF_ERROR;
	else
		params.mType = NIIF_INFO;

	params.mIcon = GoFindAnIcon(IconFileName.Value().c_str());

	if(params.mIcon == 0)
		params.mIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_NOTIFU));

	params.mTitle = Title.Value();

	params.mText = Text.Value();

	return S_OK;
}