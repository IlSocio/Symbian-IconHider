/*
 ============================================================================
 Name		: RomPatcherAuto.h
 Author	  : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : Exe header file
 ============================================================================
 */

#ifndef __ICONHIDERAUTOCONSOLE_H__
#define __ICONHIDERAUTOCONSOLE_H__

//  Include Files
#include <FuzzyByte\IconHider\settings_consts.h>
#include <FuzzyByte\pubsubobserver.h>
#include <FuzzyByte\callsmonitor.h>
#include <FuzzyByte\FileLogger.h>

#include <e32base.h>

//  Function Prototypes


class CHider : public CBase, MPubSubObserver, MCallsObserver
	{
public:
	static CHider* NewLC(TBool fromGui);
	virtual ~CHider();
	void StartL();

private:
	void ConstructL();
	CHider(TBool fromGui);
	
private:
	virtual void PropertyChangedL(TUid category, TUint key, TInt value);
	virtual void CallStatusChangedL(CTelephony::TCallStatus& aStatus, TInt aError);

private:
	TBool iFromGui;
	CPubSubObserver* iPubSubMonitor;
	CCallsMonitor* iCallsMonitor;
	TPassCode iCode;
	__LOG_DECLARATION_MEMBER
	};


GLDEF_C TInt E32Main();

#endif  // __ICONHIDERAUTO_H__

