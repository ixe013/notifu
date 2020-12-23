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
// notifu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CommandLine.h"
#include "NotifyUser.h"
#include "RegistryFix.h"
#include "Serialize.h"
#include "returnvalue.h"
#include "QueryContinueOneInstance.h"
#include "UserNotificationCallback.h"
#include "Trace.h"
#include "License.h"

//CAppModule _Module;
 
/* -------------------------------------------------------------------------- */
/** @brief The entry point of the program (it is command line).
*
* This function serves as a demonstration on how to use the NotifyUser function.
* You must somehow fill a NOTIFU_PARAM structure and pass it to NotifyUser.
* You will most probably throw everything else away, and that's ok.
*
* The SerializeEnter and SerializeLeave are there to provide the "ballon
* replacement" feature of the command line utility.
*
* Good luck !
*
* @param hInstance This module's instance
* @param hPrevInstance Legacy parameter, always 0
* @param lpCmdLine The command line parameters. They will be sent to the command
*                  line parser object
* @param nCmdShow  Recommemded way of showing a window that does not exist in
*                  this program. It is not used.
*
* @return One of the error codes documeneted in the enum above.
*/
/* ---------------------------------------------------------------------------- */
#ifdef _CONSOLE
int _tmain(int argc, const TCHAR *argv[])
{
   int errorlevel = eUnknown;

   TRACE(eINFO, L"Notifu process %d starting (http://www.paralint.com/projects/notifu/)\n", GetProcessId(GetCurrentProcess()));
   TRACE(eINFO, L"%s build\n", __TIMESTAMP__);

   gCommandLine.Setup();

   gCommandLine.Parse(argc, argv);

#else
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
   int errorlevel = eUnknown;

   TRACE(eINFO, L"Notifu process %d starting (http://www.paralint.com/projects/notifu/)\n", GetProcessId(GetCurrentProcess()));
   TRACE(eINFO, L"%s build\n", __TIMESTAMP__);

   gCommandLine.Setup();

   gCommandLine.Parse(lpCmdLine);
#endif

   if (gCommandLine.Fix)
   {
      if (EnableBalloonTips() == ERROR_SUCCESS)
      {
         if (gCommandLine.HasErrors())
         {
            //just wanted to fix the registry, that's ok
            errorlevel = eSuccess;
            gCommandLine.Show(L"Balloon tips were enabled in the registry.\n\nThe key EnableBalloonTips=1 was added under HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced");
            goto end;
         }
      }
      else
      {
         gCommandLine.Show(L"Unable to enable balloon tips in the registry.\n\nPlease add EnableBalloonTips=1 under HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced");
      }
   }
   else if(gCommandLine.KillAll)
   {
       TerminateAllProcess();
   }

   if(gCommandLine.License)
   {
       ShowLicense();
   }
   else if (gCommandLine.Continue())
   {
      HRESULT result;
      CoInitialize(0);

      NOTIFU_PARAM params;

      gCommandLine.CopyCommandLineToParams(params);

      CQueryContinue *cqc = 0;

      if(gCommandLine.Queue)
      {
          cqc = new CQueryContinue(params.mDelay);
      }
      else
      {
          cqc = new CQueryContinueOneInstance(params.mDelay);
      }
          
      CUserNotificationCallback unc;

      SerializeEnter();

      result = NotifyUser(params, cqc, &unc);

      SerializeLeave();

      DestroyIcon(params.mIcon);

      errorlevel = unc.GetResultCode();

      if (errorlevel == eUnknown) switch (result)
         {
            case 0x800704C7 : errorlevel = eClosedBallon; break;	  //The operation was cancelled by the user.
            case S_OK : errorlevel = eClickedBallon; break;
            case S_FALSE :						//This operation returned because the timeout period expired.
               if (cqc->TimeoutReached())
               {
                  errorlevel = eTimedOut;
               }
               else if(cqc->getWhatHappened() == MAKE_HRESULT(0, FACILITY_ITF, 1000))
               {
                  errorlevel = eReplaced;
	           }
               else
               {
						TRACE(eWARN, L"Unknown reason for exiting (0x%08X)\n", result);
               }
               break;
            case E_NOINTERFACE:
            case REGDB_E_CLASSNOTREG:
               errorlevel = eNotSupported;
               gCommandLine.Show(L"IUserNotification is not supported on this version of Windows.");
               break;
         }

      delete cqc;

      CoUninitialize();
   }
   else
   {
      errorlevel = eBadArgs;
   }

end:

   TRACE(eINFO, L"Returning %d\n", errorlevel);
   return errorlevel;
}

