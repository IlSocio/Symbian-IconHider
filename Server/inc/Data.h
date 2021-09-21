//#define _LOGGING


/*
============================================================================
 Name        : Data.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : CData declaration
============================================================================
*/

#ifndef Data_H
#define Data_H


// INCLUDES
#include "Settings_Consts.h"

#include <e32std.h>
#include <e32base.h>
#include <s32strm.h>
#include <f32file.h>
#include <FuzzyByte\Filelogger.h>
#include <CENTRALREPOSITORY.H>


const TUint KMaxSettingLength = NCentralRepositoryConstants::KMaxUnicodeStringLength;



class CData : public CBase
{
public: // Constructors and destructor

	~CData();

	static CData* NewL(RFs& aFs);
	static CData* NewLC(RFs& aFs);

	TInt LoadData();
	void SaveDataL();
	TInt DeleteFileDataL();

	TBool IsEnabled();
	void SetEnabled(TBool value);
	
	TInt AddProtectedApp(const TDesC& fname,  const TInt32 uid);
	TInt RemProtectedApp(const TDesC& fname,  const TInt32 uid);

	HBufC* GetProtectedAppsLC(const TDesC& fname);
	static void GetProtectedAppsL(RFs& fs, const TDesC& fname,  RArray<TUint32>& protApps);
	static void GetAvailableProfilesL(RFs& fs, RPointerArray<HBufC>& arrayFiles, RPointerArray<HBufC>& arrayNames);
	static void GetProfileNameL(RFs& fs, const TDesC& fname, TDes& name);
	static void RenameProfileL(RFs& fs, const TDesC& fname, const TDesC& newName);
	
	void CreateDefaultProfileL();
	void CreateNewProfileL(const TDesC& name, TDes& fname);
	void DeleteProfile(const TDesC& fname);
	TPtrC GetCurrentProfile();
	void SetCurrentProfileL(const TDesC& fname);
	
	void ExternalizeL(RWriteStream& aStream) const;
	void InternalizeL(RReadStream& aStream);
	void Default();
	TInt IsValidRegCodeL();
	TInt GetValue();
	
private:
	HBufC8* DecryptConfigFileL(RFs& fs, const TDesC& fname);
	void EncryptConfigFileL(RFs& fs, const TDesC& fname, const TDesC8& buffer);

	void GetCodeL(TDes& buff);
	void Swap(TDes& a, TInt pos1, TInt pos2);

	TInt LoadDataL();
//	TTime GetDate();
//	void SetDate(TTime& time);

	void Ror(TDes& data);
	HBufC* CryptLC(const TDesC& buf) const;
	void CryptAndExtL(RWriteStream& aStream, const TDesC& src) const;
	void CryptAndExtL(RWriteStream& aStream, const TBool flag) const;
	void CryptAndExtL(RWriteStream& aStream, const TUint32 flag) const;
	void IntAndDeCryptL(RReadStream& aStream, TDes& out) const;
	void IntAndDeCryptL(RReadStream& aStream, TBool& val) const;
	void IntAndDeCryptL(RReadStream& aStream, TUint32& val) const;
	CData(RFs& aFs);
	void ConstructL();

public:
	TTime				iTime;
	TBool				largeIcons;
	TUint32				iFakeRegistration6;
	TBool				Filler1;
	TUint32				Filler2;
	TUint32				Filler3;
	TBool				fakeRegistration;
	TUint32				key;			
	TUint32				iZero;		// Non viene ne' letta, ne' salvata, ne' controllata e non vale zero...
	TBool				Filler4;
	TBool				Filler7;
	TBool				Filler8;
	TBool				Filler9;
	TUint32				i190;
	TUint32				Filler10;
	TUint32				iMaxProtCount;
	TUint32				Filler12;
	TUint8				iSaltForFile;
	TBuf<KMaxSettingLength> origRepoValue;
	TBool				Filler13;
	TBool				Filler14;
	TUint16				iSaltForRegCode;
	TBool				Filler15;
	TPassCode			iPassword;
	TBuf<KIMEI_LEN> 	regReqCode;
	TBuf<KIMEI_LEN> 	Filler5;
	TBuf<KIMEI_LEN> 	Filler6;
	TBuf<KIMEI_LEN> 	regCode64;
	TBuf<KMaxSettingLength> empty;
	TBuf<KIMEI_LEN> 	regCode;
	TImei 				imeiOnMemory;			// NOT STORED
	TUint16				iLastImeiDigitOnMemory;
	TImei 				imeiOnStore;
	TUint32				iRepoUid;

private:
	RFs&				iFs;
	TBool				iEnabled;
	TFileName			currentProfileFile;
	TBuf8<16>			iIv;
	__LOG_DECLARATION_MEMBER
};

#endif // Data_H





