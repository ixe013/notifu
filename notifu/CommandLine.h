// Copyright (c) 2019, Solutions Paralint inc.                                     
// All rights reserved.                                                            
//                                                                                 
// Redistribution and use in source and binary forms, with or without              
// modification, are permitted provided that the following conditions are met:     
//     * Redistributions of source code must retain the above copyright            
//       notice, this list of conditions and the following disclaimer.             
//     * Redistributions in binary form must reproduce the above copyright         
//       notice, this list of conditions and the following disclaimer in the       
//       documentation and/or other materials provided with the distribution.      
//     * Neither the name of the <organization> nor the                            
//       names of its contributors may be used to endorse or promote products      
//       derived from this software without specific prior written permission.     
//                                                                                 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED   
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY              
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES      
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;    
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND     
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT      
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS   
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                    
//
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
	FlagArg Queue;
	FlagArg KillAll;
	FlagArg License;
	ValueArg<tstring> IconFileName;

public:
	CCommandLine();

	void Setup();

	HRESULT CopyCommandLineToParams(NOTIFU_PARAM& param);
};

extern CCommandLine gCommandLine;

#endif // !defined(AFX_COMMANDLINE_H__B29DBFFE_D683_4C3B_B042_89C0AF41B895__INCLUDED_)
