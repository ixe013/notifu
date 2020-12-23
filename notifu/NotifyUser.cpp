// Copyright (c) 2019, Solutions Paralint inc.                                     
// All rights reserved.                                                            
//                                                                                 
// Redistribution and use in source and binary forms, with or without              
// modification, are permitted provided that the following conditions are met:     
//     * Redistributions of source code must retain the above copyright            
//       notice, this list of conditions and the following disclaimer.             
//     * Redistributions in binary form must reproduce the above copyright         
//       notice, this list of conditions and the following disclaimer in the       
//       documentation and/or other materials provided with the distribution.      
//     * Neither the name of the <organization> nor the                            
//       names of its contributors may be used to endorse or promote products      
//       derived from this software without specific prior written permission.     
//                                                                                 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED   
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY              
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES      
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;    
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND     
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT      
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS   
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                    
//                                                                                 
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
   IUserNotification2 *deux = 0; //French pun : "un" above stands for UserNotification but it also means 1 in French. deux means 2.

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
