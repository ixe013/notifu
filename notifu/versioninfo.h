// VersionInfo.h: CVersionInfo interface
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2000, DevelopMentor.
// All rights reserved. No warranties extended.
// You may use them without royalty (and without support) in your compiled
// code. You may not redistribute the source code.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VERSIONINFO_H__
#define __VERSIONINFO_H__

#include <tchar.h>

#pragma comment(lib, "version.lib")
#pragma comment(lib, "user32.lib")  // wsprintf

#if defined(__AFXWIN_H__)

// MFC
#define GetModuleInstance() AfxGetResourceHandle()
#define GetResourceInstance() AfxGetResourceHandle()

#elif defined(__ATLBASE_H__)

// ATL
#define GetModuleInstance() _Module.GetModuleInstance()
#define GetResourceInstance() _Module.GetResourceInstance()

#else

// Raw Win32
#define GetModuleInstance() GetModuleHandle(0)
#define GetResourceInstance() GetModuleHandle(0)

#endif

class CVersionInfo
{
public:
    CVersionInfo(LPCTSTR pszTrans = _T("040904B0"));

    HRESULT LoadVersion(HINSTANCE hinst = GetResourceInstance());
    HRESULT LoadVersion(LPCTSTR pszModule);
    void Clear();
    ~CVersionInfo();
    LPCTSTR QueryValueString(LPCTSTR pszVerInfo) const;
    operator bool();
    
private:
    TCHAR   m_szTrans[16];
    void*   m_pVerInfo;
};

#undef GetModuleInstance
#undef GetResourceInstance

#endif  // __VERSIONINFO_H__
