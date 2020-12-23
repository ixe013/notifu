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


// Raw Win32
#define GetResourceInstance() GetModuleHandle(0)


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

#undef GetResourceInstance

#endif  // __VERSIONINFO_H__
