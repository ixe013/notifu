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
#include "Serialize.h"

static const TCHAR *gAvailableSemaphoreName = L"Local\\NA-06603283-A2A6-4fbc-A659-60851EC426DA";
static const TCHAR *gWaitingSemaphoreName = L"Local\\NW-D20C9303-7E39-452c-8E3C-CE6020778CB9";
static const TCHAR *gTerminateAllEventName = L"Local\\NT-43dabdee-86ff-4830-8ef5-4d079c181003";

static HANDLE gAvailableSemaphore = CreateSemaphore(0, 1, 1, gAvailableSemaphoreName); 
static HANDLE gWaitingSemaphore = CreateSemaphore(0, 0, 1024, gWaitingSemaphoreName); //event
static HANDLE gTerminateAllEvent = CreateEvent(0, TRUE, FALSE, gTerminateAllEventName); //event
static bool gInside = false;



void TerminateAllProcess()
{
    SetEvent(gTerminateAllEvent);
	ReleaseSemaphore(gWaitingSemaphore, 1, 0);
}

void SerializeEnter()
{
	if(SignalObjectAndWait(gWaitingSemaphore, gAvailableSemaphore, INFINITE, FALSE) == WAIT_OBJECT_0)
	{
        //We got hold of the "available" semaphore. Decrement the "waiting" semaphore
        //to indicate that we are not waiting anymore
		WaitForSingleObject(gWaitingSemaphore, INFINITE);
		gInside = true;
	}
}

bool ThreadWaiting()
{
	bool result = false;

	if(WaitForSingleObject(gWaitingSemaphore, 0) == WAIT_OBJECT_0)
	{
		//There is no way to test the semaphore without
		//decreasing its count, so we just increment it 
		//again.
		ReleaseSemaphore(gWaitingSemaphore, 1, 0);

		result = true;
	}

	return result;
}

bool KillPending() {
	return WaitForSingleObject(gTerminateAllEvent, 0) == WAIT_OBJECT_0;
}

void SerializeLeave()
{
	if(gInside)
	{
		ReleaseSemaphore(gAvailableSemaphore, 1, 0);
		gInside = false;
	}

	CloseHandle(gAvailableSemaphore);
	CloseHandle(gWaitingSemaphore);
}


