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

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#ifndef EXTERN
#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif
#endif

#define MAX_DOMAIN  512
#define MAX_USERNAME 512
#define MAX_PASSWORD 512
#define MAX_GROUPNAME 512

EXTERN HRESULT GetGroupName(const wchar_t *key, wchar_t *group, DWORD size);
EXTERN HRESULT GetNoticeText(const wchar_t *name, wchar_t *text, DWORD size);
EXTERN HRESULT GetDebugSetting(const wchar_t *name, wchar_t *text, DWORD size);
EXTERN HRESULT GetSelfServeSetting(const wchar_t *name, wchar_t *text, DWORD size);

EXTERN HRESULT GetSettingText(const wchar_t *key, const wchar_t *name, wchar_t *text, DWORD size);

EXTERN wchar_t *InterpretCarriageReturn(wchar_t *text);

EXTERN const wchar_t* gUnlockGroupName;
EXTERN const wchar_t* gForceLogoffGroupName;
EXTERN const wchar_t* gExcludedGroupName;

#endif
