#ifndef __USERNOTIFICATIONCALLBACK_H__
#define __USERNOTIFICATIONCALLBACK_H__

#include <shobjidl.h>

class CUserNotificationCallback
	: public IUserNotificationCallback
{
protected:
	DWORD result;

public:
	CUserNotificationCallback();

	STDMETHOD(QueryInterface)(REFIID iid, void** ppvObj);
   virtual ULONG STDMETHODCALLTYPE AddRef();
   virtual ULONG STDMETHODCALLTYPE Release();

	STDMETHOD(OnBalloonUserClick)(POINT *);
	STDMETHOD(OnContextMenu)(POINT *);
	STDMETHOD(OnLeftClick)(POINT *);

	DWORD GetResultCode();
};
#endif
