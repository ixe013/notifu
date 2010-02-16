// CommandLine.h: interface for the CCommandLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDLINE_H__B29DBFFE_D683_4C3B_B042_89C0AF41B895__INCLUDED_)
#define AFX_COMMANDLINE_H__B29DBFFE_D683_4C3B_B042_89C0AF41B895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommandLineParser.h"
#include "NotifyUser.h"

class CCommandLine 
	: public StandardCommandLineParser  
{
public:
	RestrictedValueArg<tstring> Type;
	ValueArg<int> Delay;
	MultiValueArg<tstring, 0>  Title;
	MultiValueArg<tstring, 1> Text;
	FlagArg Fix;
	FlagArg NoSound;
	FlagArg NoRespect;
	FlagArg ForceXP;
	ValueArg<tstring> IconFileName;

public:
	CCommandLine();

	void Setup();

	HRESULT CopyCommandLineToParams(NOTIFU_PARAM& param);
};

extern CCommandLine gCommandLine;

#endif // !defined(AFX_COMMANDLINE_H__B29DBFFE_D683_4C3B_B042_89C0AF41B895__INCLUDED_)
