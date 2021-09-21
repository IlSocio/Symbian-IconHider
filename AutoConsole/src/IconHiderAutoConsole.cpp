 /*
 ============================================================================
 Name		: RomPatcherAuto.cpp
 Author	  : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : Exe source file
 ============================================================================
 */
 
//  Include Files  

#include "IconHiderAutoConsole.h"

//#include <FuzzyByte\IconHider\Data.h>
#include <FuzzyByte\IconHider\client.h>
#include <FuzzyByte\processes.H>
#include <FuzzyByte\utils.H>
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
 

CHider::CHider(TBool fromGui) : CBase(), iFromGui(fromGui)
	{
	}

CHider::~CHider()
	{	
	__LOG_TXT("StartDestructor");
	delete iCallsMonitor;
	delete iPubSubMonitor;
	__LOG_TXT("EndDestructor");
	__LOG_CLOSE;
	}
	
CHider* CHider::NewLC(TBool fromGui)
	{
	CHider* self = new (ELeave) CHider(fromGui);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CHider::ConstructL()
	{
	__LOG_OPEN("IconHider","Background.txt");
	iPubSubMonitor = CPubSubObserver::NewL(*this, KServerUid3, KPSChangeConfig);
	iCallsMonitor = CCallsMonitor::NewL(*this);
	iCode.AppendNumFixedWidth(0, EHex, 4);
	}


void CHider::StartL()
	{
	__LOG_TXT("+ StartL");
	RHider cli;
	User::LeaveIfError(cli.Connect());
	CleanupClosePushL(cli);
	
/*	cli.GetPassword(iCode);
	iPubSubMonitor->StartMonitorProperty();
	iCallsMonitor->StartListening();*/	
	
	TPassCode regCode;
	TInt i=0;
	do { 
		__LOG("Do: %d", i);
		if ( i % 2 == 0)
			{
			__LOG_TXT("StartMonitor StartListening");
			iPubSubMonitor->StartMonitorProperty();
			iCallsMonitor->StartListening();
			} 
		else
			{
			__LOG_TXT("GetRegCode");
			cli.GetRegCode(regCode);
			__LOG_DES(regCode);
			while ( cli.Register(regCode) )
				{
				__LOG_TXT("GetPassword");
				// Eseguito solo se registrato...
				cli.GetPassword(iCode);
				__LOG_DES(iCode);
				regCode[0] = '0x10';	// e' un carattere che non dovrebbe mai essere presente
				}
			}
		i++;
	} while ( i % 2 != 0 );
	/**/
	__LOG_TXT("Done!");
	
	CleanupStack::PopAndDestroy(&cli);	
	__LOG_TXT("- StartL()");
	}


void CHider::PropertyChangedL(TUid category, TUint key, TInt value)
	{
/*	if (category != KServerUid3.iUid)
		return;
	if (key != KPSChangeConfig)
		return;*/
	__LOG("PropertyChanged: %d", value);
	
	switch (value)
		{
		case EEnableChange:
			{
			RHider cli;
			User::LeaveIfError(cli.Connect());
			CleanupClosePushL(cli);
			if ( !cli.IsEnabled() )
				{
				CActiveScheduler::Stop();
				}
			// Si, viene eseguito
			CleanupStack::PopAndDestroy(&cli);
			break;
			}
		case EPasswordChange:
			{
			RHider cli;
			User::LeaveIfError(cli.Connect());
			CleanupClosePushL(cli);
			cli.GetPassword(iCode);	// Rileva il nuovo valore di iCode
			CleanupStack::PopAndDestroy(&cli);
			break;
			}
		case EProtectedAppsChange:
			{
			break;
			}
		case ERegistrationChange:
			{
			break;
			}
		default:
			{
			break;
			}
		}
	}


LOCAL_C void HangUpOnGoingCall()
{
	RWsSession ws; 
	ws.Connect(); 
	TRawEvent ev1;
	User::After(1000000/2);
	ev1.Set(TRawEvent::EKeyDown, EStdKeyNo); 
	ws.SimulateRawEvent(ev1); // CAPABILITY Req. SwEvent
	User::After(1000000/2); 	// 5 non va bene
	TRawEvent ev2;
	ev2.Set(TRawEvent::EKeyUp, EStdKeyNo); 
	ws.SimulateRawEvent(ev2); // CAPABILITY Req. SwEvent
	ws.Close();
}


void CHider::CallStatusChangedL(CTelephony::TCallStatus& aStatus, TInt aError)
	{	
	if (aStatus == CTelephony::EStatusDialling)
		{
		CTelephony::TTelNumber number;
		iCallsMonitor->GetPhoneNumber(EFalse, number);
		TPassCode newCode = iCode;
		newCode.Insert(0, _L("*"));
		newCode.Append(_L("*"));
		__LOG_TXT("Number: ");
		__LOG_DES(number);
		__LOG_TXT("NewCode: ");
		__LOG_DES(newCode); 
		if (number == newCode)
			{
			HangUpOnGoingCall();
			__LOG_TXT("Launch GUI"); 
			// Avvia IconHider GUI...
			Processes::LaunchL(_L("IconHider_20024264"), 0x20024264);
			
			// TODO: Bring in Foreground...
			}
		}
	}

 

_LIT(KAutoStartName, "wifiserver");


LOCAL_C void DoStartL()
	{
	if (!Processes::RenameIfNotRunning( KAutoStartName ))
		{
		//__FLOG(_T8("Already Running"));
		return;
		}
	
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	
	RProcess myProcess;
	myProcess.Rendezvous(KErrNone);
		
	TInt fromGui = 0; // 0=Autostart  1=GUI  2=ExtendedNotifier
	User::GetTIntParameter(1, fromGui); 
#ifdef __WINSCW__
	if (fromGui != 1)
		return;
#endif
	
	RHider hider;
	User::LeaveIfError( hider.Connect() );
	CleanupClosePushL( hider );
	TBool enabled = hider.IsEnabled();
	CleanupStack::PopAndDestroy( &hider ); 
	
	if ( enabled )
		{
		CHider* h = CHider::NewLC( fromGui );
		// Launch background proc to monitor calls...
		h->StartL();
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy( h );
		}	
	
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

