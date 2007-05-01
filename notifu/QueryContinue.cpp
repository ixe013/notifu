#include "StdAfx.h"

#include <shobjidl.h>
//#include <shappmgr.h>

#include "QueryContinue.h"
#include "Serialize.h"

CQueryContinue::CQueryContinue(DWORD d)
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
	static int n = 0;

	if(ThreadWaiting() || TimeoutReached())
	{
		result = S_FALSE;
	}

	return result;
}

void CQueryContinue::SetTimeout(DWORD d)
{
	 mDelay = d;
	 mStarted = GetTickCount();
}

bool CQueryContinue::TimeoutReached() const
{
	bool result = false;

	if(mDelay)
		result = (GetTickCount() - mStarted) > mDelay;

	return result;
}
