#ifndef __QUERYCONTINUE_H__
#define __QUERYCONTINUE_H__

class CQueryContinue
	: public IQueryContinue
{
protected:
	DWORD mDelay;
	DWORD mStarted;

public:
	CQueryContinue(DWORD d = 0);

	void SetTimeout(DWORD d);
	bool TimeoutReached() const;

	STDMETHOD(QueryInterface)(REFIID iid, void** ppvObj);
   virtual ULONG STDMETHODCALLTYPE AddRef();
   virtual ULONG STDMETHODCALLTYPE Release();

	STDMETHOD(QueryContinue)(VOID);

};
    
#endif
