#include "stdafx.h"
#include "returnvalue.h"
#include "UserNotificationCallback.h"
#include "Trace.h"


CUserNotificationCallback::CUserNotificationCallback()
  : result(eUnknown) {}

STDMETHODIMP CUserNotificationCallback::QueryInterface(REFIID iid, void FAR* FAR* ppvObj)
{
	if (iid == IID_IUnknown || iid == IID_IUserNotificationCallback)
	{
		*ppvObj = this;
		AddRef();
		return NOERROR;
	}
	return ResultFromScode(E_NOINTERFACE);
}

ULONG CUserNotificationCallback::AddRef() 
{ 
	return 1; 
}

ULONG CUserNotificationCallback::Release() 
{ 
	return 0;
}

STDMETHODIMP CUserNotificationCallback::OnBalloonUserClick(POINT *)
{
	TRACE(eINFO, L"User clicked the balloon (callback)\n");
   result = eClickedBallon;
	return S_FALSE;
}

STDMETHODIMP CUserNotificationCallback::OnContextMenu(POINT *)
{
	TRACE(eINFO, L"User tried to bring a context menu (callback)\n");
   result = eContextMenu;
	return S_FALSE;
}

STDMETHODIMP CUserNotificationCallback::OnLeftClick(POINT *)
{
	TRACE(eINFO, L"User clicked the system notification area (callback)\n");
   result = eSystemNotificationArea;
	return S_FALSE;
}

DWORD CUserNotificationCallback::GetResultCode() 
{
	return result;
}
