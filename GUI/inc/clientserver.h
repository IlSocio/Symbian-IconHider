// clientserver.h
//
// Copyright (c) 2000 Symbian Ltd.  All rights reserved.
//

// Transient server example - shared client/server definitions
#ifndef __CLIENTSERVER_H
#define __CLIENTSERVER_H

#include <e32std.h> 

_LIT(KMyServerName,"appregistryserver.exe");
//_LIT(KMyServerImg,"GuardSrv_200107AB");		// EXE name

const TUint KServMajorVersionNumber=0;
const TUint KServMinorVersionNumber=1;
const TUint KServBuildVersionNumber=1;

//const TInt KMaxMyMessage=100;




enum TMyMessages
	{
	EIsEnabled,		// 0
	EPassword,
	EDestAddress,
	EMessage,			// EPassword in 1.05

	EIsValidImsi,
	ESendMessage,		// 5
	EClearOutBox,

	ERegister,			// EUnRegisterImsi in 1.05

	ESetPassword,
	ESetEnabled,
	ESetDestAddress,	// 10
	ESetMessage,		
	ERegisterImsi,
	ERenameImsi,
	EUnRegisterImsi,
	EGetImsiList,		// 15

	EGetRegCode,		

	EGetProtectedList,	
	EAddProtectedUid,
	ERemProtectedUid,

//	EGetInterceptAlways,
//	ESetInterceptAlways,
	EGetInactivityDelay,	// 20
	ESetInactivityDelay,

	ESetLargeIconsGUI,	
	EIsLargeIconsGUI,

	ENotifyConfigChange,
	ECancelNotifyConfigChange,	// 25

	EUninstall
	};
/*	EEnableFullVersion,
	EIsFullVersion,		// 25
	ETrialPeriodExpired,*/



#endif

