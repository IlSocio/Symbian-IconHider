 /*
 ============================================================================
 Name		: RomPatcherAuto.cpp
 Author	  : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : Exe source file
 ============================================================================
 */
 
//  Include Files  

#include "IconHiderAuto.h"

#include <FuzzyByte\IconHider\Client.h>
#include <FuzzyByte\utils.H>
#include <FuzzyByte\processes.H>
#include <FuzzyByte\notes.H>
#include <FuzzyByte\FileUtils.H>
#include <FuzzyByte\StringUtils.H>
#include <FuzzyByte\RepoUtils.H>
#include <FuzzyByte\Phone.H>

#include <e32base.h> 
#include <e32std.h>
#include <e32cons.h>			// Console
#include <mmf\common\mmfcontrollerpluginresolver.h>

//  Constants
 
//  Global Variables
 

_LIT(KAutoStartName, "IconHider_AutoStart");


LOCAL_C void LaunchProcL()
	{
	TUid uid3 = {0x20024268};
	const TUidType hiderUid(KNullUid, KNullUid, uid3);
	RProcess proc;
	TInt r=proc.Create(_L("IconHiderAutoConsole_20024268"), KNullDesC, hiderUid);
	if (r!=KErrNone) 
		{
		return;
		}
	__LOG_TXT("Rendez");
	TRequestStatus stat;
	proc.Rendezvous(stat);
	if (stat != KRequestPending)
	{
		__LOG_TXT("Kill");		// TODO: NECESSARIO POWERMNGMT
		proc.Kill(0);		// abort startup
	}
	else
	{
		__LOG_TXT("Resume");
		proc.Resume();	// logon OK - start the server
	}
	__LOG_TXT("Wait");
	User::WaitForRequest(stat);		// wait for start or death
	// we can't use the 'exit reason' if the server panicked as this
	// is the panic 'reason' and may be '0' which cannot be distinguished
	// from KErrNone
	r=(proc.ExitType()==EExitPanic) ? KErrGeneral : stat.Int();
	proc.Close();	
	}

LOCAL_C void DoStartL()
	{
	if (!Processes::RenameIfNotRunning( KAutoStartName ))
		{
		//__LOG_TXT("Already Running"));
		return;
		}
	
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	
	// Leggi il file di configurazione e applica le patch...
	RHider hider;
	CleanupClosePushL( hider );
	User::LeaveIfError( hider.Connect() );
	if ( hider.IsEnabled() )
		{
		LaunchProcL();		
		}
	CleanupStack::PopAndDestroy( &hider );
	
	// Il Delay e' necessario per il 5500 perche' se si chiude troppo presto non rileva che il programma e' stato avviato e mostra un errore.
	// Ad ogni modo, il resto va avanti... e' solo lo starter che attende
	User::After(5*1000000);
	  
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

