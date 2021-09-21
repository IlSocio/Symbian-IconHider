//#define _LOGGING


/*
============================================================================
 Name        : Data.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : CProfileData declaration
============================================================================
*/

#ifndef ProfileData_H
#define ProfileData_H


// INCLUDES
#include "Settings_Consts.h"
#include <FuzzyByte\Filelogger.h>

#include <e32std.h>
#include <e32base.h>
#include <s32strm.h>
#include <f32file.h>
#include <CENTRALREPOSITORY.H>


class CProfileData : public CBase
{
public: // Constructors and destructor

	~CProfileData();

	static CProfileData* NewL();
	static CProfileData* NewLC();

	TInt LoadData(RFs& fs, const TDesC& fname);
	void SaveDataL(RFs& fs, const TDesC& fname);

	TInt AddProtectedApp(const TInt32 uid);
	TInt RemProtectedApp(const TInt32 uid);
	
	void ExternalizeL(RWriteStream& aStream) const;
	void InternalizeL(RReadStream& aStream);
	void Default();
	void GetProtectedAppsL(TDes& buf);
	void GetProtectedAppsL(RArray<TUint32>& protApps);
	TInt CountProtectedApps();

private:
	TInt LoadDataL(RFs& fs, const TDesC& fname);
//	TTime GetDate();
//	void SetDate(TTime& time);

	CProfileData();
	void ConstructL();

public:
	TBool				Filler1;
	TUint32				fakeRegistration7;
	TUint32				Filler3;
	TBool				fakeRegistration;
	TBool				Filler4;
	TBool				Filler7;
	TBool				Filler8;
	TBool				Filler9;
	TUint32				Filler10;
	TUint32				Filler11;
	TUint32				Filler12;
	TBool				Filler13;
	TBool				Filler14;
	TBool				Filler15;
	TProfileName 		profileName;
	TBuf<KNAME_LEN> 	Filler5;
	TBuf<KIMEI_LEN> 	Filler6;
	RArray<TUint32>		protectedApps;			// Deve essere TUint32 altrim genera errore la AppendNumUC
private:

	__LOG_DECLARATION_MEMBER
};

#endif // Data_H





