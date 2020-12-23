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

