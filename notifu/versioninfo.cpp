#include "stdafx.h"
#include "VersionInfo.h"

	CVersionInfo::CVersionInfo(LPCTSTR pszTrans)
    :   m_pVerInfo(0)
    {
        _ASSERTE(_tcslen(pszTrans) < sizeof(m_szTrans)/sizeof(*m_szTrans));
        _tcscpy(m_szTrans, pszTrans);
    }

    HRESULT CVersionInfo::LoadVersion(HINSTANCE hinst)
    {
        TCHAR   szModule[MAX_PATH+1];
        if( !GetModuleFileName(hinst, szModule, MAX_PATH) ) return HRESULT_FROM_WIN32(GetLastError());
        return LoadVersion(szModule);
    }

    HRESULT CVersionInfo::LoadVersion(LPCTSTR pszModule)
    {
        _ASSERTE(pszModule && *pszModule);

        Clear();

        DWORD   nVerInfo = 0;
        DWORD   nVerInfoSize = GetFileVersionInfoSize(const_cast<LPTSTR>(pszModule), &nVerInfo);
        if( !nVerInfoSize ) return E_FAIL;

        m_pVerInfo = new BYTE[nVerInfoSize];
        if( !m_pVerInfo ) return E_OUTOFMEMORY;

        if( !GetFileVersionInfo(const_cast<LPTSTR>(pszModule), nVerInfo, nVerInfoSize, m_pVerInfo) )
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        return S_OK;
    }

    void CVersionInfo::Clear()
    {
        delete[] m_pVerInfo;
        m_pVerInfo = 0;
    }
    
    CVersionInfo::~CVersionInfo()
    {
        Clear();
    }
    
    LPCTSTR CVersionInfo::QueryValueString(LPCTSTR pszVerInfo) const
    {
        _ASSERTE(pszVerInfo && *pszVerInfo);
        _ASSERTE(m_pVerInfo);
        
        LPTSTR  pszBuf = 0;
        UINT    nBufSize = 0;
        TCHAR   szQuery[128];
        wsprintf(szQuery, __T("\\StringFileInfo\\%s\\%s"), m_szTrans, pszVerInfo);

        if( VerQueryValue(m_pVerInfo, szQuery, (void**)&pszBuf, &nBufSize) )
        {
            return pszBuf;
        }

        return 0;
    }

    CVersionInfo::operator bool()
    {
        return m_pVerInfo != 0;
    }
    
