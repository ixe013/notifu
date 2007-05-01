// CommandLine.h: interface for the CCommandLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDLINE_H__B29DBFFE_D683_4C3B_B042_89C0AF41B895__INCLUDED_)
#define AFX_COMMANDLINE_H__B29DBFFE_D683_4C3B_B042_89C0AF41B895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommandLineParser.h"
#include "NOTIFU_PARAM.h"

class CCommandLine 
	: public StandardCommandLineParser  
{
public:
	RestrictedValueArg<tstring> Type;
	ValueArg<int> Delay;
	ValueArg<tstring> Title;
	ValueArg<tstring> Text;
	FlagArg Fix;
	ValueArg<tstring> IconFileName;

public:
	CCommandLine();

	void Setup();

	HRESULT CopyCommandLineToParams(NOTIFU_PARAM& param);
};

extern CCommandLine gCommandLine;

#endif // !defined(AFX_COMMANDLINE_H__B29DBFFE_D683_4C3B_B042_89C0AF41B895__INCLUDED_)
