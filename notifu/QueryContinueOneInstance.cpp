#include "StdAfx.h"

#include <shobjidl.h>

#include "QueryContinueOneInstance.h"
#include "Serialize.h"
#include "trace.h"

CQueryContinueOneInstance::CQueryContinueOneInstance(DWORD d)
: CQueryContinue(d)
, mReplaced(false)
{
}


STDMETHODIMP CQueryContinueOneInstance::QueryContinue(VOID)
{
	HRESULT result = CQueryContinue::QueryContinue();

	if((result == S_OK) && ThreadWaiting())
	{
		TRACE(eINFO, L"Notifu process %d exiting because another instance is being launched\n", GetProcessId(GetCurrentProcess()));
      mReplaced = true;
		result = S_FALSE;
	}

	return result;
}

bool CQueryContinueOneInstance::WasReplaced()
{
   return mReplaced;
}
