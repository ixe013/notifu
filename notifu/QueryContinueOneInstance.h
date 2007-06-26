#ifndef __QUERYCONTINUEONEINSTANCE_H__
#define __QUERYCONTINUEONEINSTANCE_H__

#include "QueryContinue.h"

class CQueryContinueOneInstance
	: public CQueryContinue
{
public:
	CQueryContinueOneInstance(DWORD d = 0);

	STDMETHOD(QueryContinue)(VOID);
};
    
#endif