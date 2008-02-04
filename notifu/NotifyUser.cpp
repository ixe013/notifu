#include "stdafx.h"
#include <shappmgr.h>

#include "NotifyUser.h"
#include "QueryContinue.h"

// {723589EC-2A89-44f4-BD3C-2992807DB4F5}
static const GUID IID_marde = 
{ 0x723589ec, 0x2a89, 0x44f4, { 0xbd, 0x3c, 0x29, 0x92, 0x80, 0x7d, 0xb4, 0xf5 } };

//		case 0x800704C7 : errorlevel = eClosedBallon; break;
//		case S_OK :	errorlevel = eClickedBallon; break;
//		case S_FALSE :	errorlevel = eTimedOut; break;

HRESULT NotifyUser(const NOTIFU_PARAM& params, IQueryContinue *querycontinue)
{
	HRESULT result;

	IUserNotification *un = 0;

	//result = CoCreateInstance(CLSID_UserNotification, 0, CLSCTX_ALL, IID_IUserNotification, (void**)&un); 
	result = CoCreateInstance(CLSID_UserNotification, 0, CLSCTX_ALL, IID_marde, (void**)&un); 

	if(SUCCEEDED(result))
	{
		result = un->SetIconInfo(params.mIcon, params.mTitle.c_str());
		result = un->SetBalloonInfo(params.mTitle.c_str(), params.mText.c_str(), params.mType);
		
		//Looks like it controls what happends when the X button is
		//clicked on
		result = un->SetBalloonRetry(0, 250, 0); 

		result = un->Show(querycontinue, 250);

		un->Release();
	}

	return result;
}

