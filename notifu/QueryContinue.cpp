#include "StdAfx.h"

#include <shobjidl.h>
#include "trace.h"

#include "QueryContinue.h"

CQueryContinue::CQueryContinue(DWORD d)
 : mDelay(0)
{
	SetTimeout(d);
}

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
		result = S_FALSE;
	}

	return result;
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
