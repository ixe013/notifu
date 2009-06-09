#ifndef __NOTIFY_USER_H__
#define __NOTIFY_USER_H__

#include <string>
#include <shobjidl.h>

//#include <windows.h> 
//before NotifyUser.h

struct NOTIFU_PARAM
{
	int mDelay;
	DWORD mType;
	bool mForceXP;
	std::basic_string<TCHAR> mTitle;
	std::basic_string<TCHAR> mText;
	HICON mIcon;
};



HRESULT NotifyUser(const NOTIFU_PARAM& params, IQueryContinue *querycontinue = 0, IUserNotificationCallback *notifcallback = 0);

#endif
