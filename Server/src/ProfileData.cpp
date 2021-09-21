/*
============================================================================
 Name        : Data.cpp
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : CProfileData implementation
============================================================================
*/

#include "ProfileData.h"
//#include "srverr.h"

#include <FuzzyByte\FileUtils.h>

#include <S32FILE.H> 



const TUint KMaxSettingLength = NCentralRepositoryConstants::KMaxUnicodeStringLength;

CProfileData::CProfileData()
{
	// No implementation required
}


CProfileData::~CProfileData()
{
	__LOG_TXT("Destructor");
	protectedApps.Close();
	__LOG_TXT("End Destructor");
}

CProfileData* CProfileData::NewLC()
{
	CProfileData* self = new (ELeave)CProfileData();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CProfileData* CProfileData::NewL()
{
	CProfileData* self=CProfileData::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

void CProfileData::ConstructL()
{
	__LOG_OPEN("IconHider", "ProfileData.txt");
	__LOG_TXT("-------------");
	Default();
}



void CProfileData::Default()
		{
		Filler1 = EFalse;
		fakeRegistration7 = EFalse;
		Filler8 = EFalse;
		Filler3 = 60;
		Filler9 = EFalse;
//		inactivityDelay = 0;	// TODO: DA CAMBIARE IN RELEASE
		fakeRegistration = ETrue;
		Filler4 = EFalse;
		Filler7 = EFalse;
		Filler5.Zero();
		Filler6.Zero();
		Filler13 = Filler14 = Filler15 = ETrue;
//		imeiOnStore.Zero();
		Filler10 = Filler11 = Filler12 = 0x5F;
		protectedApps.Reset();
		}
 

void  CProfileData::GetProtectedAppsL(RArray<TUint32>& protApps)
	{
	for (TInt i=0; i<protectedApps.Count(); i++)
		protApps.Append( protectedApps[i] );
	}


void CProfileData::GetProtectedAppsL(TDes& buf)
	{
	for (TInt i=0; i<protectedApps.Count(); i++)
		{
		if (buf.Length() + 9 < buf.MaxLength())
			{
			if (buf.Length() > 0)
				buf.Append( TChar(44) ); 			// , = 44 da file
			TUint32 num = protectedApps[i];
			buf.AppendNumUC(num, EHex);
			} else
			{
			// TODO: Exceed Max. 1024
			// piu' o meno equivale a 113 Applicazioni
			}
		}	
	}


TInt CProfileData::LoadDataL(RFs& fs, const TDesC& configFile)
	{

//	FileUtils::CompleteWithPrivatePathL( fs, configFile );
//	__LOG_TXT("LoadDataL"));
	TEntry entry;

	TInt ris = fs.Entry(configFile, entry);
	if (ris != KErrNone)
	{
//		__LOG_TXT("Default"));
		this->Default();
		return ris;
	}

	CDictionaryStore* dictstore = CDictionaryFileStore::OpenLC (fs, configFile, KServerUid3 );

	RDictionaryReadStream in;
	in.OpenLC(*dictstore, KServerUid3);

//	in >> *(iData);
	in >> *(this);

	in.Close();
	CleanupStack::PopAndDestroy(2); // in dictstore
	
//	__LOG_TXT("End LoadDataL"));
	return ris;
	}


TInt CProfileData::LoadData(RFs& fs, const TDesC& fname)
	{
	TInt res = KErrNone;
	TRAPD(err, res = LoadDataL(fs, fname));
	if (err != KErrNone)
		{
		// Config Corrupt...
		Default();
		}
	return res;
	}


TInt CProfileData::CountProtectedApps()
	{
	return protectedApps.Count();
	}


void CProfileData::SaveDataL(RFs& fs, const TDesC& configFile)
	{
//	FileUtils::CompleteWithPrivatePathL( fs, configFile );
	
	fs.Delete( configFile );
	fs.MkDirAll( configFile );
	CDictionaryStore* dictstore = CDictionaryFileStore::OpenLC (fs, configFile, KServerUid3 );

	RDictionaryWriteStream out;
	out.AssignLC(*dictstore, KServerUid3);

	out << *(this);
//	out << *(iData);

	out.CommitL();            // Commit changes to the stream
	dictstore->CommitL();

	out.Close();
	CleanupStack::PopAndDestroy(2); // out dictstore
//	__LOG_TXT("End SaveDataL"));
	}


	TInt CProfileData::AddProtectedApp(const TInt32 uid)
		{
		if (protectedApps.Count() >= 25)
			return KErrOverflow;
		for (TInt i=0; i<protectedApps.Count(); i++)
			if (protectedApps[i] == uid)
				return KErrAlreadyExists;
		protectedApps.Append(uid);
		return KErrNone;
		}


	TInt CProfileData::RemProtectedApp(const TInt32 uid)
		{
		TInt err = KErrNotFound;
		for (TInt i=0; i<protectedApps.Count(); i++)
			if (protectedApps[i] == uid)
			{
				protectedApps.Remove(i);
				err = KErrNone;
			}
		return err;
		}


	
	void CProfileData::ExternalizeL(RWriteStream& aStream) const
		{
		// TODO: Semplice XOR...
		// scrive usando imei che proviene dallo store...
		aStream << profileName;

		TInt32 tot;
		tot=protectedApps.Count();
		// Anticrack
		//aStream << tot;
		aStream << (tot);
		// Anticrack
		for (TInt i=0; i<tot; i++)
			{
			TInt32 uid = protectedApps[i];
			aStream << uid;
			}
		}

	
	void CProfileData::InternalizeL(RReadStream& aStream)
		{
		// TODO: Semplice XOR...
		__LOG_TXT("InternalizeL"); 
		aStream >> profileName;
		
		TInt32 tot;
		aStream >> tot;
		protectedApps.Reset();
		for (TInt32 i=0; i<tot; i++)
			{
			TInt32 uid = 0;
			aStream >> uid;
			protectedApps.Append(uid);
			}
		
		__LOG_TXT("End InternalizeL");
		}

	
