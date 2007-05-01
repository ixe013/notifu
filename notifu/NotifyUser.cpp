#include "StdAfx.h"
#include <shobjidl.h>
#include <shappmgr.h>

#include "NotifyUser.h"
#include "QueryContinue.h"

//		case 0x800704C7 : errorlevel = eClosedBallon; break;
//		case S_OK :	errorlevel = eClickedBallon; break;
//		case S_FALSE :	errorlevel = eTimedOut; break;

HRESULT NotifyUser(const NOTIFU_PARAM& params)
{
	HRESULT result;

	IUserNotification *un = 0;

	result = CoCreateInstance(CLSID_UserNotification, 0, CLSCTX_ALL, IID_IUserNotification, (void**)&un); 

	if(SUCCEEDED(result))
	{
		CQueryContinue mqc(params.mDelay);


		result = un->SetIconInfo(params.mIcon, params.mTitle.c_str());
		result = un->SetBalloonInfo(params.mTitle.c_str(), params.mText.c_str(), params.mType);
		
		//Looks like it controls what happends when the X button is
		//clicked on
		result = un->SetBalloonRetry(0, 250, 0); 

		result = un->Show(&mqc, 250);

		if((result == S_FALSE) && !mqc.TimeoutReached())
		{
			//If there was a timeout but the ballon was closed before, 
			//error code should be S_OK
			result = S_OK;
		}

		un->Release();
	}

	return result;
}

