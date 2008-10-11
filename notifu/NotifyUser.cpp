#include "stdafx.h"
#include <shappmgr.h>

#include "NotifyUser.h"
#include "QueryContinue.h"

//		case 0x800704C7 : errorlevel = eClosedBallon; break;
//		case S_OK :	errorlevel = eClickedBallon; break;
//		case S_FALSE :	errorlevel = eTimedOut; break;

HRESULT NotifyUser(const NOTIFU_PARAM& params, IQueryContinue *querycontinue)
{
	HRESULT result;

	IUserNotification *un = 0;

	result = CoCreateInstance(CLSID_UserNotification, 0, CLSCTX_ALL, IID_IUserNotification, (void**)&un); 

	if(SUCCEEDED(result))
	{
		const std::basic_string<TCHAR> crlf_text(_T("\\n"));
		const std::basic_string<TCHAR> crlf(_T("\n"));
		std::basic_string<TCHAR> text(params.mText);
		size_t look = 0;
		size_t found;
		
		//Replace \n with actual CRLF pair
		while((found = text.find(crlf_text, look)) != std::string::npos)
		{
			text.replace(found, crlf_text.size(), crlf);
			look = found+1;
		}

		result = un->SetIconInfo(params.mIcon, params.mTitle.c_str());
		result = un->SetBalloonInfo(params.mTitle.c_str(), text.c_str(), params.mType);

		//Looks like it controls what happends when the X button is
		//clicked on
		result = un->SetBalloonRetry(0, 250, 0); 

		result = un->Show(querycontinue, 250);

		un->Release();
	}

	return result;
}
