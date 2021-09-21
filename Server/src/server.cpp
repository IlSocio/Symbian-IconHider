#include "server.h"
#include "session.h"
#include "srverr.h"

//#include <FuzzyByte\utils.h>
//#include <FuzzyByte\FileUtils.h>
#include <FuzzyByte\Processes.h>

#include <e32des8.h>
#include <f32file.h>
#include <COEMAIN.H> 
#include <e32property.h>


const TTimeIntervalSeconds KMyShutdownDelaySec(5);


CMyServer* CMyServer::NewLC()
	{
	CMyServer* server = new(ELeave) CMyServer(EPriorityNormal);
	CleanupStack::PushL(server);
	server->ConstructL();
	return server;
	}

CMyServer::CMyServer(TInt aPriority) : CServer2(aPriority)
	{
	}

void CMyServer::TimerExpiredL(TAny* src)
	{
	// Rinomina il processo prima di terminarlo perche' puo' accadere che il processo morto resti ancora visibile nella lista dei processi.
	// (Almeno questo accade nell'emulatore)
	RProcess::RenameMe(_L("GfxTransLayerSrv"));
	CActiveScheduler::Stop();
	}


void CMyServer::ConstructL()
	{
	__LOG_OPEN("IconHider", "Server.txt");
	TInt err = Start(KMyServerName);
	__LOG_DES(KMyServerName);
	if (err != KErrNone)
		{
		__LOG("ERRORE STARTL SERVER : %d", err);
		User::Leave(err);
		}
	
//	TUint32 sid = User::CreatorSecureId().iId; 
	iPhone = CPhone::NewL();
	TInt FIVE = 5;
	TUint32 sid = RProcess().SecureId().iId ^ 0x3660;
	// > 200131CB
	__LOG("SID:%d", sid);

	iShutdown = CTimeOutTimer::NewL(*this);
	iShutdown->AfterTime(KMyShutdownDelaySec);

	User::LeaveIfError( iFs.Connect() );

	iSettings = CData::NewL(iFs);
	
	iPhone->GetImei( iSettings->imeiOnMemory );

	// rileva l'ultimo byte del codice IMEI... 
	iSettings->iLastImeiDigitOnMemory = iSettings->imeiOnMemory.Right(7).Mid(5)[0];
	
	/*
// ANTICRACK //
	if (iSettings->imeiOnMemory == KMyServerImg)
		{
		// QUESTO CODICE NON DOVREBBE ESSERE MAI ESEGUITO
		RProcess::RenameMe(KMyServerName);
		fake8 = FIVE;
		} else 
		{
		iImei = iSettings->imeiOnMemory;
		fake8 = 2;
		} 
// ANTICRACK //
	 */
	
	TInt ris = KErrNone;
	err = iSettings->LoadData();	// TODO: if doesn't exists create backup file for repository settings
	
	if (err != KErrNone)
		// Il file di configurazione ha un formato non valido
		// Oppure il file di configurazione non esiste...
		{
		__LOG_TXT("!!Reset Settings!!");
		
		iSettings->Default();

		hasChanged = ETrue;
		TRAPD(err, SaveSettingsL());
		}
	__LOG("iFakeRegistration6: %d", iSettings->iFakeRegistration6);
	__LOG("fakeRegistration: %d", iSettings->fakeRegistration);
	__LOG("i190: %d", iSettings->i190);
//	__LOG("iZero: %d", iSettings->iZero);
	__LOG("iMaxProtCount: %d", iSettings->iMaxProtCount);
	__LOG("iSaltForFile: %d", iSettings->iSaltForFile);						// 90
	__LOG("iSaltForRegCode: %d", iSettings->iSaltForRegCode);				// 239
	__LOG("iLastImeiDigitOnMemory: %d", iSettings->iLastImeiDigitOnMemory); // 49   
	__LOG("iRepoUid: %x", iSettings->iRepoUid);
	
	__LOG_DES(iSettings->iPassword);	// 0000
	__LOG_DES(iSettings->regCode64);	// W3s727rv
	__LOG_DES(iSettings->imeiOnStore);	// 442671
	
	static _LIT_SECURITY_POLICY_PASS(KAllowAllPolicy);
	ris = RProperty::Define(KPSChangeConfig, RProperty::EInt, KAllowAllPolicy, KAllowAllPolicy);
	__LOG("PS_Define:%d", ris);
	}



CSession2* CMyServer::NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const
//
// Cretae a new client session. This should really check the version number.
//
	{
//	__LOG_TXT("NewSessionL"));
	// Check we're the right version
	if (!User::QueryVersionSupported(TVersion(KServMajorVersionNumber,
                                              KServMinorVersionNumber,
                                              KServBuildVersionNumber), aVersion))
        {
    //	__LOG_TXT("Version NOT Supported"));
		User::Leave(KErrNotSupported);
        }

	return new(ELeave) CMySession();
	}


void CMyServer::AddSession()
	{
    iSessionCount++; // se PosE = 9 tutto ok!
	__LOG("AddSession: %d", iSessionCount);
	iShutdown->Cancel();
	}


void CMyServer::DropSession()
	{
	__LOG("DropSession: %d", iSessionCount);
	TRAPD(err, SaveSettingsL());
    iSessionCount--;
    if (iSessionCount <= 0 && !iShutdown->IsActive())
    	iShutdown->AfterTime(KMyShutdownDelaySec);
	}


// Notifica l'avvenuto cambiamento della configurazione a tutti quanti i Client che sono in attesa di cambiamenti...
void CMyServer::NotifyChange(TAny* aSource, const TConfigChangeEvent aChangeCode)
	{
	__LOG("Fake6:%d", iSettings->iFakeRegistration6);
	__LOG("PS_Set:%d", aChangeCode);
	TInt value = static_cast<TInt>(aChangeCode + iSettings->iFakeRegistration6);
	TInt ris = RProperty::Set(KServerUid3, KPSChangeConfig, value);
	ris = 0;
	hasChanged = ETrue;
	}


void CMyServer::SaveSettingsL()
	{
	if (!hasChanged) return;
	__LOG_TXT("SaveSettingsL");
	iSettings->SaveDataL();
	hasChanged = EFalse;
	__LOG_TXT("End SaveSettingsL");
	}



CMyServer::~CMyServer()
	{
	__LOG("Destructor:%d", iSessionCount );
	TInt ris = RProperty::Delete(KPSChangeConfig); // Quando fa il delete viene segnalato il cambio ai Subscriber... Ai quali viene riportato il valore 0
	__LOG("PS_Delete:%d", ris);	ris = 0;
	if (iSettings)
		delete iSettings;
	if (iPhone)
		delete iPhone;
	iFs.Close();
	__LOG("EndDestructor:%d", iSessionCount );
	__LOG_CLOSE;
	}


TInt E32Main()
	{
	return CMyServer::EntryPoint(NULL);
	}

TInt CMyServer::EntryPoint(TAny* /*aNone*/)
	{
	CTrapCleanup* cleanupStack = CTrapCleanup::New();
	if(!cleanupStack)
		{
		User::Panic(KMyServerName, ECreateTrapCleanup);
		}

	TRAPD(leave, ThreadMainL());
	if(leave)
		{
		TBuf<30> buf = _L("appihserver");
		buf.AppendNum(leave);
		User::Panic(buf, ESrvCreateServer);
		}

	delete cleanupStack;
	cleanupStack = NULL;

	return KErrNone;
	}

 
void CMyServer::ThreadMainL()
	{
	// Puo' essere richiamato appena il server e' stato distrutto... ed il sistema non ha ancora rimosso il Server dall'elenco dei server disponibili.
	// In questo caso la StartL() presente dentro la CMyServer::ConstructL() fallira' con KErrAlreadyExists.
	
	__LOG_DECLARATION_MEMBER
	__LOG_OPEN("IconHider","StartServer.txt");
 
	if (!Processes::RenameIfNotRunning( KMyServerName ))
		{
		__LOG_TXT("Unexpected situation");
		// non dovrebbe mai accadere perche' il client non dovrebbe lanciarlo se c'e' gia' un server in esecuzione...
	    RSemaphore semaphore;
	    TInt err = semaphore.OpenGlobal( KMyServerImg );
		__LOG("Err: %d", err);
	    User::LeaveIfError( err );
	    semaphore.Signal();
	    semaphore.Close();
		__LOG_TXT("Closing Log");
		__LOG_CLOSE;
		return;
		}
	CActiveScheduler* activeScheduler = new(ELeave) CActiveScheduler();
	CleanupStack::PushL(activeScheduler);

	CActiveScheduler::Install(activeScheduler);

	__LOG_TXT("Create srv");
	CMyServer* srv = CMyServer::NewLC();	// -11 KErrAlreadyExists
	//
	// Initialisation complete, now signal the client
//	RProcess::Rendezvous(KErrNone);
// Sostituito con il semaforo perche' cosi' va bene anche in caso di Processi multipli...
// La chiamata del client alla CreateGlobal fallira', in questo modo non si verranno a creare piu' processi del server...
    RSemaphore semaphore;
	__LOG_TXT("OpenGlobal");
    TInt err = semaphore.OpenGlobal( KMyServerImg );
	__LOG("Err: %d", err);
    User::LeaveIfError( err ); 
    semaphore.Signal();
    semaphore.Close();
	__LOG_TXT("Closing Log");
	__LOG_CLOSE;

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(srv);
	CleanupStack::PopAndDestroy(activeScheduler);
	}
