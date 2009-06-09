#include "stdafx.h"
#include <ole2.h>
#include <shappmgr.h>
#include <shobjidl.h>

#include "NotifyUser.h"
#include "QueryContinue.h"
#include "Trace.h"

//  case 0x800704C7 : errorlevel = eClosedBallon; break;
//  case S_OK : errorlevel = eClickedBallon; break;
//  case S_FALSE : errorlevel = eTimedOut; break;

HRESULT NotifyUser(const NOTIFU_PARAM& params, IQueryContinue *querycontinue, IUserNotificationCallback *notifcallback)
{
   HRESULT result = E_FAIL;

   IUserNotification *un = 0;
   IUserNotification2 *deux = 0; //French pun : un above stands for UserNotification. deux means 2.

   //First try with the Vista/Windows 7 interface
   //(unless /xp flag is specified
   if (!params.mForceXP)
      result = CoCreateInstance(CLSID_UserNotification, 0, CLSCTX_ALL, IID_IUserNotification2, (void**)&deux);

   //Fall back to Windows XP
   if (!SUCCEEDED(result))
   {
      TRACE(eWARN, L"Using Windows XP interface IUserNotification\n");
      result = CoCreateInstance(CLSID_UserNotification, 0, CLSCTX_ALL, IID_IUserNotification, (void**)&un);
   }
   else
   {
      TRACE(eINFO, L"Using Vista interface IUserNotification2\n");
      un = (IUserNotification*)deux; //Rather ugly cast saves some code...
   }

   if (SUCCEEDED(result))
   {
      const std::basic_string<TCHAR> crlf_text(L"\\n");
      const std::basic_string<TCHAR> crlf(L"\n");
      std::basic_string<TCHAR> text(params.mText);
      size_t look = 0;
      size_t found;

      //Replace \n with actual CRLF pair
      while ((found = text.find(crlf_text, look)) != std::string::npos)
      {
         text.replace(found, crlf_text.size(), crlf);
         look = found+1;
      }

      result = un->SetIconInfo(params.mIcon, params.mTitle.c_str());
      result = un->SetBalloonInfo(params.mTitle.c_str(), text.c_str(), params.mType);

      //Looks like it controls what happends when the X button is
      //clicked on
      result = un->SetBalloonRetry(0, 250, 0);

      if (deux)
         result = deux->Show(querycontinue, 250, notifcallback);
      else
         result = un->Show(querycontinue, 250);

      un->Release();
   }

   return result;
}
