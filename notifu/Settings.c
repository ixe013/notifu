/*
 Copyright (c) 2008, Guillaume Seguin (guillaume@paralint.com)
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.
*/

#include <windows.h>
#include "settings.h"

HRESULT GetDebugSetting(const wchar_t *name, wchar_t *text, DWORD size)
{
   return GetSettingText(L"SOFTWARE\\Paralint.com\\Notifu\\Debug", name, text, size);
}

HRESULT GetSettingText(const wchar_t *key, const wchar_t *name, wchar_t *text, DWORD size)
{
   HRESULT result = E_FAIL;
   DWORD type;
   DWORD returnedsize = size;

   HKEY reg;

   if (RegOpenKeyEx(HKEY_CURRENT_USER, key, 0, KEY_READ, &reg) == ERROR_SUCCESS)
   {
      if (RegQueryValueEx(reg, name, 0, &type, (LPBYTE)text, &returnedsize) == ERROR_SUCCESS)
      {
         if ((type == REG_SZ) && (returnedsize < size) && (returnedsize > 0))
         {
            result = S_OK;
         }
      }

      RegCloseKey(reg);
   }

   return result;
}

