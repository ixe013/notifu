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
// CommandLine.cpp: implementation of the CCommandLine class.
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
, Queue(L"c", L"Constant (persistent) instance, will not be dismissed by new instances")
, KillAll(L"k", L"Removes every instance of Notifu, even the ones who are waiting to be displayed")
, License(L"l", L"Diplay the license (BSD-3-Clause)")
, IconFileName(L"i", L"Specify an icon to use ('parent' uses the icon of the parent process)")
{
}


void CCommandLine::Setup()
{
	Type.AddValue(L"info", L"The message is an informational message");
	Type.AddValue(L"warn", L"The message is an warning message");
	Type.AddValue(L"error", L"The message is an error message");
	Type.AddValue(L"none", L"Do not display an icon next to the message");

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
	AddFlag(Queue);
	AddFlag(KillAll);
	AddFlag(License);
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
	else if(Type.Value() == L"none")
        //should have been the default from the stard, but I don't want to introduce a breaking change now
		params.mType = 0; 
    else
		params.mType = NIIF_INFO;

	if(NoSound)
		params.mType |= NIIF_NOSOUND;

	if(!NoRespect)
		params.mType |= NIIF_RESPECT_QUIET_TIME;

   params.mForceXP = ForceXP;
   params.mQueue = Queue;

	params.mIcon = GoFindAnIcon(IconFileName.Value().c_str());

	if(params.mIcon == 0)
		//params.mIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_NOTIFU));
		params.mIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_NOTIFU));

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
