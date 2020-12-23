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
#include "StdAfx.h"

#include <shobjidl.h>
#include "trace.h"

#include "QueryContinue.h"

CQueryContinue::CQueryContinue(DWORD d)
 : mDelay(0)
 , mWhatHappened(S_OK)
{
	SetTimeout(d);
}

CQueryContinue::~CQueryContinue() {}

STDMETHODIMP CQueryContinue::QueryInterface(REFIID iid, void FAR* FAR* ppvObj)
{
	if (iid == IID_IUnknown || iid == IID_IQueryContinue)
	{
		*ppvObj = this;
		AddRef();
		return NOERROR;
	}
	return ResultFromScode(E_NOINTERFACE);
}

ULONG CQueryContinue::AddRef() 
{ 
	return 1; 
}

ULONG CQueryContinue::Release() 
{ 
	return 0;
}

STDMETHODIMP CQueryContinue::QueryContinue(VOID)
{
	HRESULT result = S_OK; //continue is the default

	if(TimeoutReached())
	{
		result = mWhatHappened = 0x800705B4;
	}

	return result;
}

HRESULT CQueryContinue::getWhatHappened() const {
	return mWhatHappened;
}

void CQueryContinue::SetTimeout(DWORD d)
{
	TRACE(eINFO, L"Timeout value set at %d milliseconds\n", d);
	 mDelay = d;
	 mStarted = GetTickCount();
}

bool CQueryContinue::TimeoutReached() const
{
	bool result = false;

	if(mDelay)
   {
      DWORD el = GetTickCount()-mStarted;
		result = el > mDelay;
      TRACE(eINFO, L"%d milliseconds elapsed (%d)\n", el, result);
   }

	return result;
}
