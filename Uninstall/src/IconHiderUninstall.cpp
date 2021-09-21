/*
 ============================================================================
 Name		: RomPatcherInstall.cpp
 Author	  : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "IconHiderUninstall.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <bautils.h>
#include <FuzzyByte\FileUtils.h>
#include <FuzzyByte\RepoUtils.h>
#include <FuzzyByte\IconHider\Data.h>



LOCAL_C void DoStartL()
	{
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	/*
RNotifier notif;
notif.Connect();
TRequestStatus stat;
TInt btn;

TBuf<20> buf = _L("Result:");
buf.AppendNum(res);
notif.Notify(_L("Uninstall"), buf, _L("ok"), _L("no"), btn, stat);
User::WaitForRequest(stat);
notif.Close();
	*/
	
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL(fs); 

	
	// Restore Original data from the private folder... bkup.dat
	TFileName bkupFile = _L("bkup.dat");
	FileUtils::CompleteWithPrivatePathL(fs, bkupFile);
	
	if ( BaflUtils::FileExists(fs, bkupFile) )
		{
		FileUtils::ReadFileContentsL(fs, bkupFile, oriValue);
		
		RBuf oriValue;
		oriValue.CleanupClosePushL();
		oriValue.Create(2000);
		RepoUtils::GetL( 0x101f8847, 8, oriValue );	
		
		fs.MkDirAll(bkupFile);	
		FileUtils::DumpToFileL(fs, bkupFile, oriValue);
		
		CleanupStack::PopAndDestroy(&oriValue);
		}

	/*CData* data = CData::NewLC(fs);
	TInt res = data->LoadData();
	if (res == KErrNone)
		{
		// Config Found And Loaded...
		RepoUtils::SetL( 0x101f8847, 8, data->origRepoValue );
		}

	// Non bisogna cancellare il file di configurazione, cosi' durante gli upgrade le impostazioni saranno mantenute.
	// res = data->DeleteFileDataL(fs);
	CleanupStack::PopAndDestroy(data);
	*/

	CleanupStack::PopAndDestroy(&fs);
	  
	// Delete active scheduler
	CleanupStack::PopAndDestroy(scheduler);
	}  

 
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
