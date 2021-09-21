// t-client.h
//
// Copyright (c) 2000 Symbian Ltd.  All rights reserved.
//

// Transient server example - client interface

#ifndef __T_CLIENT_H__
#define __T_CLIENT_H__

#include <e32std.h>
#include <e32base.h>
#include <Etel3rdParty.h>
#include <BADESCA.H>
#include <FuzzyByte\IconHider\settings_consts.h>


enum TConfigChangeEvent
	{
	ENoChanges,
	EEnableChange,
	EPasswordChange,
	EProtectedAppsChange,
	ERegistrationChange
	};

typedef TPckg<TConfigChangeEvent> TConfigPckg;



class RHider : public RSessionBase
	{
public:
	IMPORT_C TInt GetProtectedListL(RArray<TInt32>& uidList);
	
	IMPORT_C TBool IsEnabled();
	IMPORT_C TInt GetVersion(TDes& msg);

	IMPORT_C TInt GetPassword(TDes& pass);
	IMPORT_C TInt GetImei(TDes& imei);

	IMPORT_C TInt Connect();
	IMPORT_C TInt SetPassword(const TDesC& pass);
	IMPORT_C TInt SetEnabled(TBool enabled);

	IMPORT_C TInt GetRegCode(TDes& regcode);

	IMPORT_C TInt Register(const TDesC& regCode);
	IMPORT_C TInt AddProtectedUid(const TInt32& uid);
	IMPORT_C TInt RemProtectedUid(const TInt32& uid);
	};

#endif

