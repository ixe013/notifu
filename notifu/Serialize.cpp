#include "StdAfx.h"
#include "Serialize.h"

static const TCHAR *gAvailableSemaphoreName = L"Local\\NA-06603283-A2A6-4fbc-A659-60851EC426DA";
static const TCHAR *gWaitingSemaphoreName = L"Local\\NW-D20C9303-7E39-452c-8E3C-CE6020778CB9";

static HANDLE gAvailableSemaphore = CreateSemaphore(0, 1, 1, gAvailableSemaphoreName); 
static HANDLE gWaitingSemaphore = CreateSemaphore(0, 0, 1024, gWaitingSemaphoreName); //event
static bool gInside = false;



void SerializeEnter()
{
	if(SignalObjectAndWait(gWaitingSemaphore, gAvailableSemaphore, INFINITE, FALSE) == WAIT_OBJECT_0)
	{
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


