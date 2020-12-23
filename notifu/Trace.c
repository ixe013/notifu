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
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <tchar.h>
#include "trace.h"
#include "settings.h"

#define MAX_TRACE_BUFFER 1024

typedef int (*WriteBufferProc)(LPCWSTR buffer);

int WriteBufferToStream(const wchar_t *buffer)
{
   //Targetted for release 1.5
   return 0;
}

int WriteBufferToOutputDebugString(const wchar_t *buffer)
{
   OutputDebugString(buffer);
   return 1;
}


/* -------------------------------------------------------------------------- */
/** @brief Reads the registry and returns a function that will do the logging.
*
* Output will go to OutputDebugString if the "Output" value is present. 
* If a debug level is given, it will be honored. It defauls to eWARN.
*
* @return A pointer to a logging function
*/
/* ---------------------------------------------------------------------------- */
WriteBufferProc GetOutputWriter(int level)
{
   WriteBufferProc result = 0;
   int reglevel = eWARN;

   wchar_t buffer[512];
   if (GetDebugSetting(L"Level", buffer, sizeof buffer / sizeof *buffer) == S_OK)
   {
      if (_wcsicmp(L"None", buffer) == 0)
      {
         reglevel = eNONE;
      }
      else if (_wcsicmp(L"Error", buffer) == 0)
      {
         reglevel = eERROR;
      }
      else if (_wcsicmp(L"Warning", buffer) == 0)
      {
         reglevel = eWARN;
      }
      else if((_wcsicmp(L"Info", buffer) == 0) 
           || (_wcsicmp(L"Information", buffer) == 0))
      {
         reglevel = eINFO;
      }
   }

      if (level >= reglevel)
      {
         if (GetDebugSetting(L"Output", buffer, sizeof buffer / sizeof *buffer) == S_OK)
         {
            //That test will be usefull when more than one method will be available
	         //if (_wcsicmp(L"OutputDebugString", buffer) == 0) 
            {
               result = &WriteBufferToOutputDebugString;
            }
         }
      }

   return result;
}


void Trace(int level, const wchar_t* file, int line, const wchar_t *format, ...)
{
   WriteBufferProc output_proc = GetOutputWriter(level);

   if (output_proc)
   {
      wchar_t buffer[MAX_TRACE_BUFFER];
      va_list args;

      if (file && line)
      {
         swprintf_s(buffer, sizeof buffer / sizeof *buffer, L"[%s:%ld] (%d) ", file, line, level);
         output_proc(buffer);
      }

      va_start(args, format);

      if (format && vswprintf_s(buffer, sizeof buffer / sizeof *buffer, format, args) >= 0)
      {
         output_proc(buffer);
      }
   }
}

void TraceMessage(const wchar_t* file, int line, DWORD dw)
{
   LPVOID lpMsgBuf;

   FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      FORMAT_MESSAGE_FROM_SYSTEM,
      NULL,
      dw,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPTSTR) &lpMsgBuf,
      0, NULL);

   Trace(eERROR, file, line, L"0x%08X: %s", dw, lpMsgBuf);

   LocalFree(lpMsgBuf);
}
