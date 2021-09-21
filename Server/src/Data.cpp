/*
 ============================================================================
 Name        : Data.cpp
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : CData implementation
 ============================================================================
 */
#include "Data.h"
#include "ProfileData.h"
//#include "srverr.h"
#include <FuzzyByte\FileUtils.h>
#include <FuzzyByte\RepoUtils.h>
#include <FuzzyByte\Processes.h>
#include <FuzzyByte\AES.h>
#include <s32mem.H>
#include <bautils.H>
#include <S32FILE.H>
#include <mmf\common\mmfcontrollerpluginresolver.h>
_LIT(KConfigFile, "IconHider.dat");

CData::CData(RFs& aFs) :
	CBase(), iFs(aFs)
	{
	// No implementation required
	}

CData::~CData()
	{
	__LOG_TXT("Destructor");
	__LOG_TXT("End Destructor");
	}

CData* CData::NewLC(RFs& aFs)
	{
	CData* self = new (ELeave) CData(aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CData* CData::NewL(RFs& aFs)
	{
	CData* self = CData::NewLC(aFs);
	CleanupStack::Pop(); // self;
	return self;
	}

void CData::ConstructL()
	{
	__LOG_OPEN("IconHider", "Data.txt");
	__LOG_TXT("-------------");
	Default();
	}

void CData::Default()
	{
	iTime.HomeTime();
	iSaltForFile = (TUint8) iTime.DateTime().MicroSecond();
	largeIcons = iZero = ETrue;
	iPassword = _L("0000");
	iSaltForRegCode = iPassword.MaxLength() + iPassword.Length();
	iIv.Zero();
	iIv.AppendNumFixedWidth(iSaltForRegCode, EHex, 8);
	regCode.Zero();		
	Filler1 = EFalse;
	Filler2 = EFalse;
	origRepoValue.Zero();
	Filler8 = EFalse;
	Filler3 = 60;
	iEnabled = EFalse;
	iRepoUid = 0x101f8847;
	Filler9 = EFalse;
	//		inactivityDelay = 0;	// TODO: DA CAMBIARE IN RELEASE
	fakeRegistration = ETrue;
	iFakeRegistration6 = EFalse;
	Filler4 = EFalse;
	Filler7 = EFalse;
	Filler5.Zero();
	Filler6.Zero();	
	iMaxProtCount = regCode.Length();	// 0
	// 9 nel caso di 1 sola applicazione
	// 190 nel caso di 21 applicazioni
	empty.Zero();

	regCode64.Zero(); 

	regReqCode.Zero();
	Filler13 = Filler14 = Filler15 = ETrue;
	imeiOnStore.Zero();
	Filler10 = Filler12 = 0x5F;
	i190 = Filler10 << 1;
	currentProfileFile.Zero();
	iIv.AppendNumFixedWidth(Filler12, EHex, 8);
	}

void CData::GetAvailableProfilesL(RFs& fs, RPointerArray<HBufC>& arrayFiles,
		RPointerArray<HBufC>& arrayNames)
	{
	RPointerArray<HBufC> files;
	CleanupResetAndDestroyPushL(files);

	TFileName path;
	FileUtils::CompleteWithPrivatePathL(fs, path);
	path.Append(_L("*.*"));
	FileUtils::ListFilesInDirectoryL(fs, path, files);

	for (TInt i = 0; i < files.Count(); i++)
		{
		CProfileData* prof = CProfileData::NewLC();
		prof->LoadData(fs, files[i]->Des());
		arrayFiles.Append(files[i]->Alloc());
		arrayNames.Append(prof->profileName.Alloc());
		CleanupStack::PopAndDestroy(prof);
		}

	CleanupStack::PopAndDestroy(&files);
	}

void CData::GetProfileNameL(RFs& fs, const TDesC& fname, TDes& name)
	{
	CProfileData* prof = CProfileData::NewLC();
	prof->LoadData(fs, fname);
	name = prof->profileName;
	CleanupStack::PopAndDestroy(prof);
	}

void CData::RenameProfileL(RFs& fs, const TDesC& fname, const TDesC& newName)
	{
	CProfileData* prof = CProfileData::NewLC();
	prof->LoadData(fs, fname);
	prof->profileName = newName;
	prof->SaveDataL(fs, fname);
	CleanupStack::PopAndDestroy(prof);
	}

TBool CData::IsEnabled()
	{
	return iEnabled;
	}
 
void CData::SetEnabled(TBool value)
	{
	iEnabled = value;
	iFakeRegistration6 = iMaxProtCount & (imeiOnStore.Length() ^ 6);
	// Se e' registrato, vale sempre 0 (perche' length ^ 6 = 0)
	// Se non e' registrato, vale sempre 0 (perche' 1 & 6 = 0)
	HBufC* buf = NULL;
	if (iEnabled)
		{
		buf = GetProtectedAppsLC(currentProfileFile);
		}
	else
		{
		buf = KNullDesC().AllocLC();
		}
	RepoUtils::SetL(iRepoUid, 8, *buf);
	CleanupStack::PopAndDestroy(buf);

	// Kills Menu'... 
	__LOG_TXT("Kills Menu...");
	TInt res = Processes::KillL(_L("*101F4CD2*"));
	__LOG("KillProc: %d", res);
	//	res = Processes::KillL( _L("*101F4cd2*") );
	//	__LOG("KillProc: %d", res);
	// 101F4CD2 Menu 
	}

TPtrC CData::GetCurrentProfile()
	{
	if (currentProfileFile.Length() <= 0 || !BaflUtils::FileExists(iFs,
			currentProfileFile))
		{
		CreateDefaultProfileL();
		}
	return currentProfileFile;
	}

void CData::SetCurrentProfileL(const TDesC& fname)
	{
	currentProfileFile = fname;
	SetEnabled(iEnabled);
	}

void CData::CreateDefaultProfileL()
	{
	TFileName fname = _L("Profile.dat");
	FileUtils::CompleteWithPrivatePathL(iFs, fname);
	if (!BaflUtils::FileExists(iFs, fname))
		{
		iFs.Delete(fname);
		}
	CProfileData* prof = CProfileData::NewLC();
	prof->profileName = _L("Default");
	prof->SaveDataL(iFs, fname);
	CleanupStack::PopAndDestroy(prof);
	SetCurrentProfileL(fname);
	}

void CData::CreateNewProfileL(const TDesC& name, TDes& fname)
	{
	RFile file;
	TInt res = KErrNone;
	TFileName path;
	FileUtils::CompleteWithPrivatePathL(iFs, path);
	iFs.MkDirAll(path);
	TRAPD(err, res = file.Temp(iFs, path, (TFileName&) fname, EFileWrite ));
	file.Close();
	CProfileData* prof = CProfileData::NewLC();
	prof->profileName = name;
	prof->SaveDataL(iFs, fname);
	CleanupStack::PopAndDestroy(prof);

	if (currentProfileFile.Length() <= 0)
		SetCurrentProfileL(fname);
	}

void CData::DeleteProfile(const TDesC& fname)
	{
	if (currentProfileFile == fname)
		SetCurrentProfileL(KNullDesC());
	iFs.Delete(fname);
	}

void CData::GetProtectedAppsL(RFs& fs, const TDesC& fname,
		RArray<TUint32>& protApps)
	{
	CProfileData* prof = CProfileData::NewLC();
	prof->LoadData(fs, fname);
	prof->GetProtectedAppsL(protApps);
	CleanupStack::PopAndDestroy(prof);
	}

HBufC* CData::GetProtectedAppsLC(const TDesC& fname)
	{
	HBufC* result = HBufC::NewLC(iSaltForRegCode);
	TPtr buf = result->Des();
	CProfileData* prof = CProfileData::NewLC();
	prof->LoadData(iFs, fname);	
	prof->GetProtectedAppsL(buf);
	CleanupStack::PopAndDestroy(prof);
	return result;
	}

/*
void LOCAL_C SwapNibbles(TUint8& byte)
	{
	TUint8 hi = byte >> 4;
	TUint8 lo = (byte & 0x0F) << 4;
	byte = lo | hi;
	}

void LOCAL_C SwapBytes(TDes8& buf, TInt pos1, TInt pos2)
	{
	TUint8 tmp = buf[pos1];
	buf[pos1] = buf[pos2];
	buf[pos2] = tmp;
	}

void LOCAL_C EncrIMEI(TDes8& imei, TInt8 salt)
	{
	// 0 Salt  1..15 IMEI
	TBuf8<1> salt_buf;
	salt_buf.Append(salt);
	imei.Insert(0, salt_buf); 
	for (TInt i=1; i<imei.Length(); i++)
		{
		SwapNibbles(imei[i]);
		imei[i] = imei[i] ^ salt;
		salt %= 0xF0;
		salt++;
		}
	for (TInt i=0; i<imei.Length() / 2; i++)
		{
		if (i%2 == 0) // 0 2 4 6 8
			SwapBytes(imei, i, imei.Length() - i - 1);
		}
	}

TInt LOCAL_C DecrIMEI(TDes8& imei)
	{
	// 0..14 IMEI  15 Salt
	for (TInt i=0; i<imei.Length() / 2; i++)
		{
		if (i%2 == 0)
			SwapBytes(imei, i, imei.Length() - i - 1);
		}
	TInt salt_ori = imei[0]; 
	TInt salt = salt_ori;
	for (TInt i=1; i<imei.Length(); i++)
		{
		imei[i] = imei[i] ^ salt;
		SwapNibbles(imei[i]);
		salt %= 0xF0;
		salt++;
		}
	return salt_ori;
	}*/


HBufC8* CData::DecryptConfigFileL(RFs& fs, const TDesC& fname)
	{
	if (!BaflUtils::FileExists(fs, fname))
		return HBufC8::NewL(0);

	HBufC8* buf = FileUtils::ReadFileContentsL(fs, fname);
	CleanupStack::PushL(buf);

	if (buf->Des().Length() < 1)
		User::Leave(KErrCorrupt);
	
	// removes salt 
	TUint8 salt = buf->Des()[0];
	buf->Des().Delete(0, 1);
	TBuf8<16> key;
	key.Copy( imeiOnMemory.Left(15) );
	key.Append( salt );

	//	__FLOG(_L8("AES::DecryptL() Begin"));
	RBuf8 plain(AES::DecryptL(buf->Des(), iIv, key));
	plain.CleanupClosePushL();
	//	__FLOG(_L8("AES::DecryptL() End"));

	/*	if (plain.Length() == 0)
	 {
	 CleanupStack::PopAndDestroy(&plain);
	 CleanupStack::Pop(buf);
	 buf->Des().SetLength(0);
	 return buf;
	 }*/

	buf->Des().Copy(plain);
	buf->Des().SetLength(plain.Length());
	CleanupStack::PopAndDestroy(&plain);
	CleanupStack::Pop(buf);
	return buf;
	}

void CData::EncryptConfigFileL(RFs& fs, const TDesC& fname,
		const TDesC8& buffer)
	{
	//  Usa AES per cifrare l'intero buffer...
//	TBuf8<16> key = _L8("1234567812345678");
	TBuf8<16> key;
	key.Copy( imeiOnMemory.Left(15) );
	key.Append(iSaltForFile);
	//EncrIMEI(key, iSaltForFile);
	
	RBuf8 encrypted(AES::EncryptL(buffer, iIv, key));	// key = imei in chiaro...
	encrypted.CleanupClosePushL();

	// Saves the file
	TBuf8<10> buf;
	buf.Append( iSaltForFile );
	FileUtils::DumpToFileL(fs, fname, buf, EFalse); 
	FileUtils::DumpToFileL(fs, fname, encrypted, ETrue); 
	TInt size = FileUtils::GetFileSize(fs, fname);
	CleanupStack::PopAndDestroy(&encrypted);
	} 
 
TInt CData::LoadDataL()
	{
	TFullName configFile = KConfigFile();
	FileUtils::CompleteWithPrivatePathL(iFs, configFile);

	//	__LOG_TXT("LoadDataL"));

	this->Default();

	TEntry entry;
	TInt ris = iFs.Entry(configFile, entry);
	if (ris != KErrNone)
		{
		return ris;
		}

	// Decrypt config file...
	RBuf8 configBuffer(DecryptConfigFileL(iFs, configFile));
	configBuffer.CleanupClosePushL();
	TBool isValid = (configBuffer.Length() > 0);
	if (isValid)
		{
		RMemReadStream memStream(configBuffer.Ptr(), configBuffer.Length()); 
		CleanupClosePushL(memStream);
		memStream >> *(this);
		CleanupStack::PopAndDestroy(&memStream);
		}
	CleanupStack::PopAndDestroy(&configBuffer);

	if (!isValid)
		return KErrNotFound;
	return KErrNone;
	}

TInt CData::DeleteFileDataL()
	{
	TFullName configFile = KConfigFile(); // TODO: Decrypt Config FileName

	RPointerArray<HBufC> files;
	CleanupResetAndDestroyPushL(files);

	TFileName path;
	FileUtils::CompleteWithPrivatePathL(iFs, path);

	path.Append(_L("*.*"));
	FileUtils::ListFilesInDirectoryL(iFs, path, files);
	for (TInt i = 0; i < files.Count(); i++)
		{
		DeleteProfile(files[i]->Des());
		}

	CleanupStack::PopAndDestroy(&files);

	return iFs.Delete(configFile);
	}


TInt CData::GetValue()
	{
	return iSaltForRegCode - i190 - iLastImeiDigitOnMemory;
	}


TInt CData::LoadData()
	{
	TInt res = KErrNone;
	TRAPD(err, res = LoadDataL());
	if (err != KErrNone)
		{
		// Config Corrupt...
		Default(); res = KErrCorrupt;
		CreateDefaultProfileL(); 
		}
	return res;
	}
 
void CData::SaveDataL()
	{
	TFullName configFile = KConfigFile();
	FileUtils::CompleteWithPrivatePathL(iFs, configFile);

	CBufBase* buffer = CBufFlat::NewL(50);
	CleanupStack::PushL(buffer);
	RBufWriteStream strm;
	CleanupClosePushL(strm);
	strm.Open(*buffer);
	strm << (*this);
	strm.CommitL();
	CleanupStack::PopAndDestroy(&strm);

	EncryptConfigFileL(iFs, configFile, buffer->Ptr(0));

	CleanupStack::PopAndDestroy(buffer);
	}

void CData::Swap(TDes& a, TInt pos1, TInt pos2)
	{
	TUint16 val = a[pos1];
	a[pos1] = a[pos2];
	a[pos2] = val;
	}

void CData::GetCodeL(TDes& buff)
	{ 
	buff.Zero();
	if (regCode.Length() < 6 || imeiOnMemory.Length() < 7)
		{
		return;
		}		
	TBuf<7> tmp = imeiOnMemory.Right(7);
	tmp.SetLength(6); 

	TUint16 ori_salt = regCode.Mid(5)[0]; // Rileva il salt dal codice di registrazione fornito... 
	TUint16 salt = ori_salt;
	// elabora il codice IMEI 
	buff.SetLength(6);
	buff.FillZ();
	TUint16 add = 0;
	for (TInt i = 0; i < tmp.Length(); i++)
		{
		tmp[i] ^= salt;
		salt += iFakeRegistration6;
		salt += 31;
		salt = salt % 0xFF;
		add += tmp[i];
		TInt pos = add % (i + 1); 
		Swap(tmp, i, pos);
		} 
	buff = tmp;
	buff[5] = ori_salt;
	}

TInt CData::IsValidRegCodeL()
	{
	TBuf<20> buff; // regCode contiene il codice inserito dall'utente...
	GetCodeL(buff);
	return ((regCode.Length() >= 6) && (regCode == buff));
	}

TInt CData::AddProtectedApp(const TDesC& fname, const TInt32 uid)
	{
	TInt result = KErrOverflow;
	CProfileData* prof = CProfileData::NewLC();
	prof->LoadData(iFs, fname); 

	TInt totProt = prof->CountProtectedApps();
	if (totProt <= regCode64.Length() * 4)
		{
		result = prof->AddProtectedApp(uid);
		iMaxProtCount |= totProt;
		prof->SaveDataL(iFs, fname);
		} else
		{
		}
	CleanupStack::PopAndDestroy(prof);
	
	if (result != KErrNone)
		return result;

	if (!IsEnabled())
		return KErrNone;

	if (fname != GetCurrentProfile())
		return KErrNone;

	SetEnabled(ETrue);
	return KErrNone;
	}

TInt CData::RemProtectedApp(const TDesC& fname, const TInt32 uid)
	{
	CProfileData* prof = CProfileData::NewLC();
	prof->LoadData(iFs, fname);
	TInt result = prof->RemProtectedApp(uid);
	prof->SaveDataL(iFs, fname);
	CleanupStack::PopAndDestroy(prof);
	if (result != KErrNone)
		return result;

	if (!IsEnabled())
		return KErrNone;

	if (fname != GetCurrentProfile())
		return KErrNone;

	SetEnabled(ETrue);
	return KErrNone;
	}


void CData::ExternalizeL(RWriteStream& aStream) const
	{
	TUint32 aaa = 0;
	// scrive usando imei che proviene dallo store...
	aStream << KSETTINGS_VERSION;
	aaa = largeIcons; aStream << aaa;
	aStream << iPassword;
	aStream << i190;
	aaa = Filler1; aStream << aaa;
	aStream << iSaltForRegCode;
	aaa = Filler7; aStream << aaa;
	aaa = iEnabled; aStream << aaa;
	aaa = fakeRegistration; aStream << aaa;
	aStream << key;
	aaa = Filler4; aStream << aaa;
	aStream << currentProfileFile;
	aStream << iFakeRegistration6;
	aStream << Filler10;
	aStream << Filler3;
	aStream << iMaxProtCount;
	aStream << Filler2;
	aStream << Filler12;
	aaa = Filler13; aStream << aaa;
	aStream << origRepoValue;
	aaa = Filler14; aStream << aaa;
	aaa = Filler15; aStream << aaa;
	aaa = Filler9; aStream << aaa;
	aStream << regReqCode;
	aStream << Filler5;
	aStream << Filler6;
	aStream << iRepoUid;
	aStream << regCode; 
	aStream << empty;
	aaa = Filler8; aStream << aaa;
	aStream << regCode64;
	aStream << imeiOnStore;/**/
	}

void CData::InternalizeL(RReadStream& aStream)
	{
	__LOG_TXT("InternalizeL");
	TUint32 aaa = 0; 
	TBuf<3> version; 
	aStream >> version;
	if (version != KSETTINGS_VERSION)
		{
		Default();
		return;
		}
	aStream >> aaa; largeIcons = aaa;
	aStream >> iPassword;
	aStream >> i190;
	aStream >> aaa; Filler1 = aaa;
	aStream >> iSaltForRegCode;
	aStream >> aaa;	Filler7 = aaa;
	aStream >> aaa; iEnabled = aaa;
	aStream >> aaa;	fakeRegistration = aaa;
	aStream >> key;
	aStream >> aaa;	Filler4 = aaa;
	aStream >> currentProfileFile;
	aStream >> iFakeRegistration6;
	aStream >> Filler10;
	aStream >> Filler3;
	aStream >> iMaxProtCount;
	aStream >> Filler2;
	aStream >> Filler12;
	aStream >> aaa;	Filler13 = aaa;
	aStream >> origRepoValue;
	aStream >> aaa;	Filler14 = aaa;
	aStream >> aaa;	Filler15 = aaa;
	aStream >> aaa;	Filler9 = aaa;
	aStream >> regReqCode;
	aStream >> Filler5;
	aStream >> Filler6;
	aStream >> iRepoUid;
	aStream >> regCode;
	aStream >> empty;
	aStream >> aaa;	Filler8 = aaa;
	aStream >> regCode64;
	aStream >> imeiOnStore;/**/
	__LOG_TXT("End InternalizeL");
	}

