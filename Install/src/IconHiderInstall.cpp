 /*
 ============================================================================
 Name		: RomPatcherInstall.cpp
 Author	  : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "IconHiderInstall.h"

#include <FuzzyByte\RepoUtils.h>
#include <FuzzyByte\FileLogger.h>
#include <FuzzyByte\FileUtils.h>
#include <FuzzyByte\Processes.h>

#include <FuzzyByte\Utils.h>
#include <FuzzyByte\InboxAttachment.h>

_LIT(KFilePath, "c:\\system\\data\\20024264\\");
_LIT(KFileRedist, "c:\\system\\data\\20024264\\iconhider.sisx");

_LIT(KBkupPath, "c:\\system\\data\\20024264\\");
_LIT(KBkupFilename, "bkup.dat");

//_LIT(KBkupFile, "c:\\private\\20024267\\import\\bkup.dat");
//	TFileName bkupFile = _L("c:\\system\\data\\20024264\\bkup.dat");


#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <bautils.h>
#include <e32property.h>
#include <sacls.h>

     enum TInstOp
        {
        EInstOpNone = 0x00000000,
        EInstOpInstall = 0x00000001,
        EInstOpUninstall = 0x00000002,
        EInstOpRestore = 0x00000004
        };

    enum TInstOpStatus
        {
        EInstOpStatusNone = 0x00000000,
        EInstOpStatusSuccess = 0x00000100,
        EInstOpStatusAborted = 0x00000200
        };

    
LOCAL_C void BackupRepositoryL(RFs& aFs, const TDesC& aFileName)
	{
	if ( BaflUtils::FileExists(aFs, aFileName) )
		return;

	RBuf oriValue;
	oriValue.CleanupClosePushL();
	oriValue.Create(2000);
	TInt res = RepoUtils::GetL( 0x101f8847, 8, oriValue );		
	if (res == KErrNone)
		{
		aFs.MkDirAll(aFileName);
		FileUtils::DumpToFileL(aFs, aFileName, oriValue);
		}
		
	CleanupStack::PopAndDestroy(&oriValue);
    }   



LOCAL_C void RestoreRepositoryL(RFs& aFs, const TDesC& aFileName)
	{
	if ( !BaflUtils::FileExists(aFs, aFileName) )
		{
		RepoUtils::SetL( 0x101f8847, 8, KNullDesC() );		
		// Mostra tutto... cosi' anche le app che sono state nascoste verranno visualizzate...
		
		// Processes::KillL( _L("*101F4CD2*") );	
		// Premendo "c" dal menu', probabilmente e' il menu' il padre del processo installer
		// e killando il menu' l'installer resta in panne...
		return;
		}

	RBuf buf( FileUtils::ReadTextFileL(aFs, aFileName) );
	buf.CleanupClosePushL();
	RepoUtils::SetL( 0x101f8847, 8, buf );
	CleanupStack::PopAndDestroy(&buf);

	// Deletes the backup file...
	aFs.Delete(aFileName);
	
	// Kills Menu'...
	// Processes::KillL( _L("*101F4CD2*") );
    }


LOCAL_C void DoStartL()
	{
	__LOG_DECLARATION_MEMBER
	TUint32 sid = RProcess().SecureId().iId;
	if (sid == 0x20024266)
		{
		__LOG_OPEN("IconHider","Install.txt");
		} 
	else
		{
		__LOG_OPEN("IconHider","Uninstall.txt");
		}
	
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	
/*	RNotifier notif;
	notif.Connect();
	TRequestStatus stat;
	TInt btn;
	notif.Notify(_L("Install"), _L("Install"), _L("ok"), _L("no"), btn, stat);
	User::WaitForRequest(stat);
	notif.Close();*/
	
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL(fs);  

	TFileName publicBkupFile = KBkupPath();
	publicBkupFile.Append( KBkupFilename );
	
	switch (sid)
		{
		case 0x20024266:
			{
			__LOG_TXT("Install");
			BackupRepositoryL(fs, publicBkupFile);	// Install
			
			TFileName filename(KFilePath);
			CInboxAttachment* inBox = CInboxAttachment::NewLC();
			TBool done = inBox->BT_IRDA_GetFileFromInboxL(fs, filename);
			CleanupStack::PopAndDestroy(inBox);
			__LOG("Done: %d", done);
			if (done)
			{
				TUint32 uid3 = Utils::GetUid3FromSisL(fs, filename);
				__LOG("Uid3: %x", uid3);
				TBool isInRange = EFalse;
				if (uid3 == 0x20024264)
					isInRange = ETrue;
				 
				if (!isInRange)
				{
					// elimina il file... 
					fs.Delete(filename);
				} else 
				{
					if (filename != KFileRedist)
						{
						fs.Delete(KFileRedist);
						BaflUtils::RenameFile(fs, filename, KFileRedist);
						__LOG_TXT("renamed");
						}
				}
		/*		if (uid3 == 0x200107AB)
				{
					if (filename != KFileInstall)
						BaflUtils::RenameFile(fs, filename, KFileInstall);
				}*/
			}
			// Quando faccio l'aggiornam. viene scaricato il pacchetto completo...
			// Posso anche inviare il pacchetto completo dal pc al cell.

			break;
			}
		case 0x20024267:
			{
			__LOG_TXT("Uninstall");
			TFileName privateBkupFile = KBkupFilename();
			FileUtils::CompleteWithPrivatePathL( fs, privateBkupFile );
			__LOG_DES(privateBkupFile);
			if (BaflUtils::FileExists( fs, publicBkupFile))
				{ 
				__LOG_TXT("Import bkup file...");
				fs.MkDirAll( privateBkupFile );
				__LOG_TXT("Rename file...");
				TInt err = BaflUtils::CopyFile( fs, publicBkupFile, privateBkupFile, 0); 		// No Overwrite
				__LOG("ErrCopy: %d", err);
				// FIX: non e' possibile fare il rename da c: a e:
				// TInt err = BaflUtils::RenameFile( fs, publicBkupFile, privateBkupFile, 0); 	// No Overwrite
				// __LOG("ErrRen: %d", err);
				if (err == KErrAlreadyExists)
					{
					__LOG_TXT("Already exists!");
					} else
					{
					__LOG_TXT("Imported!");
					}
				err = BaflUtils::DeleteFile( fs, publicBkupFile );
				__LOG("ErrDel: %d", err);
				} else
				{
				__LOG_TXT("Restore bkup file");
				RestoreRepositoryL(fs, privateBkupFile);	// Uninstall
				}
			break;
			}
		default:
			{
			break;
			}
		}
	CleanupStack::PopAndDestroy(&fs);
	  
	// Delete active scheduler
	CleanupStack::PopAndDestroy(scheduler);
	__LOG_CLOSE;
	}  

/*
	// NON FUNZIA SU N95...	
	TInt instStatus;
	TInt err = KErrNone;
	TRAPD(trap_err, err = RepoUtils::GetL(KUidSystemCategoryValue, KSAUidSoftwareInstallKeyValue, instStatus));
	instStatus = instStatus & 0x000F;
	
	__LOG("Status: %d  err: %d  trap_err: %d", instStatus, err, trap_err);
	
	switch (instStatus)
		{
		case EInstOpNone:
			__LOG_TXT("None");
			break;
		case EInstOpInstall:
			__LOG_TXT("Install");
			BackupRepository(fs, bkupFile);
			break;
		case EInstOpUninstall:
			__LOG_TXT("Uninstall");
			RestoreRepository(fs, bkupFile);
			break;
		case EInstOpRestore:
			__LOG_TXT("Restore");
			BackupRepository(fs, bkupFile);
			break;
		default:
			__LOG_TXT("Default");
			break;
		}
	*/


 
//  Global Functions

GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Run application code inside TRAP harness, wait keypress when terminated
	TRAPD(mainError, DoStartL());

	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}
