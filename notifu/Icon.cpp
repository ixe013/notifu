// Icon.cpp: implementation of the Icon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Icon.h"
#include <psapi.h>

#include <shellapi.h>

HICON GetParentProcessIcon();


typedef LONG NTSTATUS;

typedef struct 
{
    LONG ExitStatus;
    LPVOID PebBaseAddress;
    DWORD AffinityMask;
    LONG BasePriority;
    DWORD UniqueProcessId;
    DWORD InheritedFromUniqueProcessId;
	
} PROCESS_BASIC_INFORMATION;

#define DYNLOADED_FPTR( ptrname, procname, dllname)\
	FPTR_##procname ptrname = \
( FPTR_##procname ) GetProcAddress ( GetModuleHandle (  _TEXT( #dllname)), #procname);

#define CREATE_DYNFUNC_5( ptrname, procname, dllname, rettype, callconv, a1, a2, a3, a4, a5)\
	typedef  rettype (callconv *FPTR_##procname) ( a1, a2, a3, a4, a5);\
DYNLOADED_FPTR( ptrname, procname, dllname);

typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation = 0,
		ProcessWow64Information = 26
} PROCESSINFOCLASS;


DWORD   GetParentProcessId(   DWORD   dwPID)
{
    NTSTATUS                        ntStatus;
    DWORD                           dwParentPID =   0xffffffff;
	
    HANDLE                          hProcess;
    PROCESS_BASIC_INFORMATION       pbi;
    ULONG                           ulRetLen;
	
    //  create entry point for 'NtQueryInformationProcess()'
    CREATE_DYNFUNC_5    (   NtQueryInformationProcess,
		NtQueryInformationProcess,
		ntdll,
		NTSTATUS,
		__stdcall,
		HANDLE,
		PROCESSINFOCLASS,
		PVOID,
		ULONG,
		PULONG
		);
	
    //  get process handle
    hProcess    =   OpenProcess (   PROCESS_QUERY_INFORMATION,
		FALSE,
		dwPID
		);
	
    //  could fail due to invalid PID or insufficiant privileges
    if  (   !hProcess)
		return  (   0xffffffff);
	
    //  gather information
    ntStatus    =   NtQueryInformationProcess   (   hProcess,
		ProcessBasicInformation,
		( void*) &pbi,
		sizeof  (   PROCESS_BASIC_INFORMATION),
		&ulRetLen
		);
	
    //  copy PID on success
    if  (   !ntStatus)
		dwParentPID =   pbi.InheritedFromUniqueProcessId;
	
    CloseHandle (   hProcess);
	
    return  (   dwParentPID);
}

HICON GoFindAnIcon(LPCTSTR path)
{
	HICON icon = 0;

	//not using our parent's icon
	if(_tcsicmp(L"parent", path))
	{
		icon = (HICON)LoadImage(0, path, IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE|LR_LOADMAP3DCOLORS);
		if(!icon)
		{
			//Try something else
			TCHAR mypath[MAX_PATH];
			const TCHAR *cleanpath = path;
			const TCHAR *comma;			

			comma = _tcsrchr(path, ',');
			UINT index = 1;

			if(comma)
			{
				_tcsncpy(mypath, path, comma-path); //Can you exploit this buffer overflow ?
				mypath[comma-path] = TCHAR(0);

				index = _ttoi(comma+1);

				cleanpath = mypath;
			}

			ExtractIconEx(cleanpath, index, 0, &icon, 1);
		}
	}
	else
	{
		icon = GetParentProcessIcon();
	}

	return icon;
}

HICON GetParentProcessIcon()
{
	HICON icon = 0;
	
	DWORD parentid;

	parentid = GetParentProcessId(GetCurrentProcessId());
	
	if(parentid)
	{
		HANDLE parent = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, parentid);
		
		if(parent)
		{
			TCHAR parentname[MAX_PATH];

			/*
			//Clean
			HMODULE hMod;
			DWORD cbNeeded;
			
			if(EnumProcessModules(parent, &hMod, sizeof(hMod), &cbNeeded))
			{
				GetModuleFileNameEx(parent, hMod, parentname, sizeof(parentname));
				
				ExtractIconEx(parentname, 0, 0, &icon, 1);
			}
			/*/
			//Just as good, it seems...
			GetModuleFileNameEx(parent, 0, parentname, sizeof(parentname));
				
			ExtractIconEx(parentname, 0, 0, &icon, 1);
			//*/
			
			CloseHandle(parent);
		}
	}
	
	return icon;
}
