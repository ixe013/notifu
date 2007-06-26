#include "StdAfx.h"

#include <shobjidl.h>
//#include <shappmgr.h>

#include "QueryContinueOneInstance.h"
#include "Serialize.h"

CQueryContinueOneInstance::CQueryContinueOneInstance(DWORD d)
: CQueryContinue(d)
{
}


STDMETHODIMP CQueryContinueOneInstance::QueryContinue(VOID)
{
	HRESULT result = CQueryContinue::QueryContinue();

	if((result != S_FALSE) && ThreadWaiting())
	{
		result = S_FALSE;
	}

	return result;
}
