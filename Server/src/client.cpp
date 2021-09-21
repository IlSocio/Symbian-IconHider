
// Transient server example - client interface implementation


#include "client.h"
#include "clientserver.h"
#include "srverr.h"

#include <FuzzyByte\FileLogger.h>
#include <FuzzyByte\utils.h>
#include <FuzzyByte\processes.h>
#include <FuzzyByte\IconHider\settings_consts.h>

#include <S32MEM.H> 


const TInt KServerDefaultMessageSlots = 1; //Number of message slots available per session.

LOCAL_C TInt CreateServerProcess(RSemaphore& sem)
	{
	const TUidType serverUid(KNullUid, KNullUid, KServerUid3);

	RProcess server;
	TInt err = server.Create(KMyServerImg, KNullDesC, serverUid);
	if(err != KErrNone)
		{
		return err;
		}

	server.Resume();	
#ifdef _DEBUG
	sem.Wait();
	err = KErrNone;
#else
	err = sem.Wait(5000000);
#endif
	TInt res=KErrNone;
	if (server.ExitType()==EExitPanic) 
		res = KErrGeneral;
	else
		res = err;
	server.Close();
	return res; 
	}


LOCAL_C TBool IsRunning(const TDesC& exeName)
	{
	TFileName procName;
	procName.Copy(exeName);
	procName.Trim();
	procName.Append(_L("*"));

	TFindProcess processFinder(procName);
	TFullName fullName;
	if (processFinder.Next(fullName) == KErrNone)
		{
		return ETrue;
		}
	return EFalse;
	}


LOCAL_C TInt StartServer()
	{
	// Il FindServer non e' suffic. perche' il nome del server sara' assegnato diverso tempo dopo la creazione del processo... 

	/* QUESTI CONTROLLI DOVREBBERO ESSERE SUPERFLUI PERCHE' IL RENDEZVOUS DOVREBBE GIA' BASTARE...
 	if (Utils::ProcessIsRunningL( KMyServerName ))
		return KErrNone;
	if (Utils::ProcessIsRunningL( KMyServerImg ))
		return KErrNone;*/
	
	TBool exists = IsRunning(KMyServerName);
	if (exists)
		return KErrNone;
	
	// Probably the Server is not Running
	RSemaphore sem;
    TInt err = sem.CreateGlobal( KMyServerImg, 0 );
    if ( err != KErrNone )
        {
        // Server is already Running...
        return KErrNone;
        }
	// Server not Running		
	err = CreateServerProcess(sem);
	sem.Close();
	return err;
	}


EXPORT_C TBool RHider::IsEnabled()
	{
    TBool exists=EFalse;
    TPckgBuf<TBool> pckg;  // Questo e' di tipo TDes8
    TIpcArgs args(&pckg);
    SendReceive(EIsEnabled, args);
    exists = pckg();
    return exists;
	}



EXPORT_C TInt RHider::Connect()
	{
	TInt i=0;
	TInt errno;
	do {
		errno = StartServer();
		i++;
	} while (errno != KErrNone && i < 5);
	if(errno == KErrNone)
		{
		errno = CreateSession(KMyServerName,
			TVersion(KServMajorVersionNumber,KServMinorVersionNumber,KServBuildVersionNumber),
			KServerDefaultMessageSlots);
		}
	return errno;
	}


EXPORT_C TInt RHider::Register(const TDesC& regCode)
	{
    TInt regged=0;
    TPckgBuf<TInt> pckg;
    TIpcArgs args(&pckg, &regCode);
    SendReceive(ERegister, args); // restituisce KErrSame value nel caso in cui si tenti di chiamare la routine di registrazione per 2 volte con il codice corretto...
    regged = pckg();
	return regged;
	}


// Il buffer di ricezione deve essere abbastanza grande da contenere i dati...
EXPORT_C TInt RHider::GetPassword(TDes& pass)
	{
//	RDebug::Print(_L("### Log %d %08x"), 5, 0xABCDEF12);
    TIpcArgs args(&pass);
    return SendReceive(EPassword, args);
// KErrNone, if the send operation is successful; KErrServerTerminated, if the server no longer present; KErrServerBusy, if there are no message slots available; KErrNoMemory, if there is insufficient memory available.
// TODO: ??? Restituire messaggi di errore differenti??? Licenza non trovata / non valida / ecc...
//	return KErrNone;
	}


EXPORT_C TInt RHider::GetRegCode(TDes& dest)
	{
    TIpcArgs args(&dest);
    return SendReceive(EGetRegCode, args);
	}

EXPORT_C TInt RHider::SetPassword(const TDesC& pass)
	{
    TIpcArgs args(&pass);
    return SendReceive(ESetPassword, args);
	}

EXPORT_C TInt RHider::SetEnabled(TBool enabled)
	{
    TPckgBuf<TBool> pckg(enabled);
    TIpcArgs args(&pckg);
    return SendReceive(ESetEnabled, args);
	}

EXPORT_C TInt RHider::GetImei(TDes& imei)
	{
    TIpcArgs args(&imei);
    return SendReceive(EGetImei, args);	
	}

EXPORT_C TInt RHider::GetVersion(TDes& msg)
	{
    TIpcArgs args(&msg);
    return SendReceive(EGetVersion, args);
	}


EXPORT_C TInt RHider::AddProtectedUid(const TInt32& uid)
	{
	TPckgBuf<TInt32> pckg(uid);
    TIpcArgs args(&pckg);
    return SendReceive(EAddProtectedUid, args);
	}


EXPORT_C TInt RHider::RemProtectedUid(const TInt32& uid)
	{
	TPckgBuf<TInt32> pckg(uid);
    TIpcArgs args(&pckg);
    return SendReceive(ERemProtectedUid, args);
	}


EXPORT_C TInt RHider::GetProtectedListL(RArray<TInt32>& uidList)
	{
    TInt totQty=0;
    TPckgBuf<TInt> pckg;
	HBufC8* buffer = HBufC8::NewLC( 4 * 10 ); // Legge 10 uid alla volta
	TPtr8 ptr = buffer->Des();
	TIpcArgs args(&ptr, &pckg);

	uidList.Reset();
	TInt ris = 0;
	do {
		ptr.Zero();
		ris = SendReceive(EGetProtectedList, args);
		if (ris != KErrNone && ris != KErrCompletion)
		{
			CleanupStack::PopAndDestroy(buffer);	// buffer
			return ris;
		}
		totQty = pckg();	// Quantita' di uid prelevati...
		const TUint8* memPtr = ptr.Ptr();
		for (TInt i=0; i<totQty; i++)
		{
			TInt32* uid = (TInt32*) memPtr;
			uidList.AppendL(*uid);
			memPtr += 4;
		}
	} while (ris != KErrCompletion);

	CleanupStack::PopAndDestroy(buffer);

	return KErrNone;
	}



/*#ifndef __SERIES60_3X__
	GLDEF_C TInt E32Dll(TDllReason )
	// DLL entry point
	{
	return(KErrNone);
	}
#endif*/

