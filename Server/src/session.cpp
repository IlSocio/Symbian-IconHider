
#include "session.h"
#include "server.h"
#include "base64.h"
#include "srverr.h"
#include "clientserver.h"
#include "settings_consts.h"

#include <e32base.h>
#include <e32des8.h>
#include <f32file.h>
#include <S32MEM.H>
#include <etel3rdparty.h>

_LIT(KPanic, "IconHiderCli");

inline CMyServer& CMySession::Server()
	{return *static_cast<CMyServer*>(const_cast<CServer2*>(CSession2::Server()));}


CMySession::CMySession()
	{
	}

//#define __LOG_OPEN_LIT(a, b)
 
///////////////////////

void CMySession::CreateL()
//
// 2nd phase construct for sessions - called by the CServer framework
//
	{
	Server().AddSession();
	TBuf<20> buf = _L("Session");
	buf.AppendNum(Server().iSessionCount);
	buf.Append(_L(".txt"));
	__LOG_OPEN_LIT(_L("IconHider"), buf);
	} 


CMySession::~CMySession()
	{
	__LOG_TXT("Destructor");
	Server().DropSession();
	__LOG_TXT("End Destructor");
	__LOG_CLOSE;
	}


void CMySession::SetEnabledL(const RMessage2& aMessage)
	{
	__LOG_TXT("Set Enabl");
    TPckgBuf<TBool> pckg(EFalse);
	aMessage.ReadL(0, pckg);
	TBool val = pckg();

	if (val == Server().iSettings->IsEnabled())
	{
		__LOG_TXT("ErrValue");
		aMessage.Complete(KErrSameValue);
		return;
	}
	
	Server().iSettings->SetEnabled(val);
	Server().NotifyChange(this, EEnableChange);
	// Server().SaveSettingsL();
	// Quando vengono aggiornati dei parametri di configurazione bisogna segnalare la modifica a tutti gli altri Client diversi da questo
	__LOG_TXT("End SetEnabl");
	aMessage.Complete(KErrNone);
	}


void CMySession::SetPasswordL(const RMessage2& aMessage)
	{
	__LOG_TXT("SetPass");
	TPassCode buff;
	if (aMessage.GetDesLength(0) > buff.MaxLength())
	{
		__LOG_TXT("Err Args");
		aMessage.Complete(KErrArgument);
		return;
	}
	
	aMessage.ReadL(0, buff);
	buff.Trim();
	if (buff.Length() < 1)
	{
		__LOG_TXT("Err Short");
		aMessage.Complete(KErrPassShort);
		return;
	}
	for (TInt i=0; i<buff.Length(); i++)
	{
		if (buff[i] > '9' || buff[i] < '0')
		{
			__LOG_TXT("Err Args");
			aMessage.Complete(KErrArgument);
			return;
		}
	}

	if (Server().iSettings->iPassword == buff)
	{
		__LOG_TXT("Err SameVal");
		aMessage.Complete(KErrSameValue);
		return;
	}

	// ANTICRACK
	Server().iSettings->iRepoUid += Server().iSettings->GetValue();
	// ANTICRACK
	
	Server().iSettings->iPassword = buff;
	Server().NotifyChange(this, EPasswordChange);
	// Server().SaveSettingsL();
	// Quando vengono aggiornati dei parametri di configurazione bisogna segnalare la modifica a tutti gli altri Client diversi da questo
	// -> ConfigChanged()
	// Stara' ai client rilevare i nuovi parametri di configurazione...
	__LOG_TXT("End SetPass");
	aMessage.Complete(KErrNone);
	}


void CMySession::IsEnabledL(const RMessage2& aMessage)
	{
	__LOG_TXT("IsEnab");
	TBool ris=Server().iSettings->IsEnabled();
    TPckgBuf<TBool> p(ris);
    aMessage.WriteL(0, p);
	aMessage.Complete(KErrNone);
	__LOG_TXT("End IsEnab");
	}


void CMySession::PasswordL(const RMessage2& aMessage)
	{
	__LOG_TXT("PassL");
	if (aMessage.GetDesMaxLength(0) < Server().iSettings->iPassword.Length())
	{
		__LOG_TXT("Err Len");
		aMessage.Complete(KErrArgument);
		return;
	}
	aMessage.WriteL(0, Server().iSettings->iPassword);
/*
// Anticrack
	__LOG("F4_b:%d", Server().iSettings->fakeRegistration4);
	aMessage.WriteL(Server().iSettings->fakeRegistration4, Server().iSettings->password);
// Anticrack
 */
	__LOG_TXT("End PassL");
	aMessage.Complete(KErrNone);
	}


// Metodo di Gold o Platinum...
void CMySession::ImeiL(const RMessage2& aMessage)
	{
	__LOG_TXT("ImeiL");
	if (aMessage.GetDesMaxLength(0) < Server().iSettings->imeiOnMemory.Length())
	{
		__LOG_TXT("Err Args");
		aMessage.Complete(KErrArgument);
		return;
	}
	aMessage.WriteL(0, Server().iSettings->imeiOnMemory);
	__LOG_TXT("End ImeiL");
	aMessage.Complete(KErrNone);
	}


void CMySession::GetVersionL(const RMessage2& aMessage)
	{
	__LOG_TXT("GetVersionL");
	if (aMessage.GetDesMaxLength(0) < KSETTINGS_VERSION().Length())
	{
		__LOG_TXT("Err Args");
		aMessage.Complete(KErrArgument);
		return;
	}
	aMessage.WriteL(0, KSETTINGS_VERSION());
	__LOG_TXT("End GetVersionL");
	aMessage.Complete(KErrNone);	
	}


void CMySession::RemProtectedUidL(const RMessage2& aMessage)
	{
	__LOG_TXT("RemProtUid");
    TPckgBuf<TInt32> pckg(0);
	aMessage.ReadL(0, pckg);
	TInt32 uid = pckg();

	TPtrC currProf = Server().iSettings->GetCurrentProfile();
	TInt ris = Server().iSettings->RemProtectedApp(currProf, uid);
	if (ris == KErrNone)
		{
		Server().NotifyChange(this, EProtectedAppsChange);
		}
	__LOG("Ris:%d", ris);
	__LOG_TXT("End RemProtUid");
	aMessage.Complete(ris);
	}


void CMySession::AddProtectedUidL(const RMessage2& aMessage)
	{
	__LOG_TXT("AddProtUid");
    TPckgBuf<TInt32> pckg(0);
	aMessage.ReadL(0, pckg);
	TInt32 uid = pckg();

	/*
// ANTICRACK Verifica registrazione...
	RBuf plat;
	RBuf gold;
	TPtrC regCode = Server().iSettings->regCode;
	TPtrC regReqCode = Server().iSettings->iRegReqCode;
	Server().iSettings->GetRegRequestCodeLC(regReqCode, plat);
	Server().iSettings->GetRegRequestCodeLC(plat, gold);
// ANTICRACK Verifica registrazione...
	*/

	TPtrC currProf = Server().iSettings->GetCurrentProfile();
	TInt ris = Server().iSettings->AddProtectedApp(currProf, uid);

	/*
// ANTICRACK Verifica registrazione...
	TInt i = 0;
	while (i<gold.Length())
	{
		if (i >= regCode.Length() || (gold[i] != regCode[i] && plat[i] != regCode[i]))
			Server().iSettings->fakeRegistration2 |= ETrue;
		i++;
	}
	CleanupStack::PopAndDestroy(); // plat
	CleanupStack::PopAndDestroy(); // gold
// ANTICRACK Verifica registrazione...
	 */

	if (ris == KErrNone)
		{
		Server().NotifyChange(this, EProtectedAppsChange);
		}
	__LOG("ris:%d", ris);
	aMessage.Complete(ris);
	__LOG_TXT("End AddProtUid");
	}


void CMySession::GetRegCodeL(const RMessage2& aMessage)
{
	__LOG_TXT("GetRegCode");
	if (aMessage.GetDesMaxLength(0) < Server().iSettings->regCode64.Length())
	{
		__LOG_TXT("Err Arg");
		aMessage.Complete(KErrArgument);
		return;
	}
	aMessage.WriteL(0, Server().iSettings->regCode64);
	aMessage.Complete(KErrNone);
	__LOG_TXT("End GetRegCode");
}


TUint CMySession::ClientUID(const RMessagePtr2& aMsg)
{
	RThread ClientThread;
	aMsg.Client(ClientThread);// get client thread
	if(!ClientThread.Handle())// has handle???
	{
		PanicClient(aMsg, EPanicInvalidUID);
		return 0;
	}
	RProcess ClientProcess;
	TInt ris = ClientThread.Process(ClientProcess);// then get the process
	if (ris != KErrNone)
	{
		PanicClient(aMsg, EPanicInvalidUID);
		return 0;
	}
	TUint id=0;

#ifdef __UIQ__
	TUid uid = ClientProcess.Type()[3];
	id = uid.iUid;
#else
	// __SERIES60_3X__
	TSecureId prosID = ClientProcess.SecureId();// and check secure UID
	id = prosID.iId;
#endif
	return id;
}



void CMySession::GetProtectedListL(const RMessage2& aMessage)
	{
	__LOG_TXT("ProtList");
	TInt buffer_len = aMessage.GetDesMaxLength(0);
	HBufC8* buffer = HBufC8::NewLC(buffer_len); // Deve essere grande quanto il buffer in ingresso...


	RFs& aFs = Server().iFs;
	TPtrC currProf = Server().iSettings->GetCurrentProfile();
	
	RArray<TUint32> uidList;
	CleanupClosePushL(uidList);
	Server().iSettings->GetProtectedAppsL(aFs, currProf, uidList);
	TInt tot_uid = uidList.Count();
	
	TInt sentQty = 0;
	TUint spaceLeft = buffer_len;
	TUint spaceUsed = 0;
	while ((iProcessedUid < tot_uid) && (spaceLeft > 0))
	{
 		TInt32 uid = uidList[iProcessedUid];
 		__LOG("uid%d=%d", iProcessedUid, uid);

		if (sizeof(uid) <= spaceLeft)
		{
			const TUint8* ptr = (TUint8 *) &uid;
			buffer->Des().Append(ptr, 4);
			spaceUsed += sizeof(uid);
			spaceLeft -= sizeof(uid);
			sentQty++;
			iProcessedUid++;
		} else
		{
			spaceLeft = 0;
		}
	}
	CleanupStack::PopAndDestroy(&uidList);

	aMessage.WriteL(0, buffer->Des());	

	TPckgBuf<TInt> pck(sentQty);
	aMessage.WriteL(1, pck);

	CleanupStack::PopAndDestroy(buffer);

	if (iProcessedUid >= tot_uid)
	{
		__LOG_TXT("End ProtList KErrCompl");
		iProcessedUid = 0;
		aMessage.Complete(KErrCompletion);
		return;
	}
	
	__LOG_TXT("End ProtList KErrNone");
	aMessage.Complete(KErrNone);
	}


LOCAL_C TBool Replace(TDes& s, const unsigned char car1, const unsigned char car2)
{
	if (s.Length() == 0)
		return EFalse;
    TInt i=0;
	bool eq=true;
	unsigned char old = s[i];
	for (; i<s.Length(); i++)
	{
		if ((s[i]<old-1) || (s[i]>old+1))
			eq=false;
		old = s[i];
		if (old == car1)
		{
			s[i] = car2;
		}
	}
	return eq;
}

	
void CMySession::RegisterL(const RMessage2& aMessage)
{
	__LOG_TXT("RegisterL");
	TInt regged = 0;

	TBuf<8> regCode64;
	if (aMessage.GetDesLength(1) > regCode64.MaxLength())
	{
		__LOG_TXT("ErrArgs");
	    TPckgBuf<TInt> p(regged);
	    aMessage.WriteL(0, p);
		aMessage.Complete(KErrArgument);
		return;
	}
	aMessage.ReadL(1, regCode64);

	if (regCode64.Length() == 0)
	{
		TPckgBuf<TInt> p(regged);
		aMessage.WriteL(0, p);
		__LOG_TXT("End RegisterL");
		aMessage.Complete(KErrNone);
		return;
	}

	// AntiCrack
	Server().iSettings->iZero = EFalse;	// Non viene ne' letta, ne' salvata, ne' controllata
	// AntiCrack

	// AntiCrack
	Server().iSettings->fakeRegistration = Replace(regCode64, 'I', 'l');
	Server().iSettings->fakeRegistration |= Replace(regCode64, 'O', '0');
	__LOG("F1_e:%d", Server().iSettings->fakeRegistration);
	// AntiCrack

	// Salva il vecchio valore del codice...
	TBuf<8> oldCode;
	oldCode.Copy(Server().iSettings->regCode);

	// Converte da Base64...
	CBase64CodecBase* base64 = new (ELeave) CBase64CodecBase();
	CleanupStack::PushL(base64);
	Server().iSettings->regCode.Zero();
	base64->Decode(regCode64, Server().iSettings->regCode);
	CleanupStack::PopAndDestroy();

	// ANTICRACK: Chiave con cui verra' decifrato il RegCode
	TImei oldImeiOnStore = Server().iSettings->imeiOnStore;
	Server().iSettings->imeiOnStore = Server().iSettings->imeiOnMemory.Right(7).Left(6);
	// ANTICRACK
	
	// Verifica Registrazione...
	__LOG_TXT("IsValidRegCodeL");
	__LOG_DES(Server().iSettings->regCode);
	regged = Server().iSettings->IsValidRegCodeL();
	if (regged)
	{
		/*if (regCode64 != Server().iSettings->regCode64)
		{
			__LOG_TXT("DIFF_64");
		}*/
		if (oldCode != Server().iSettings->regCode)
		{
			__LOG_TXT("DIFF_BIN");
			Server().iSettings->iSaltForRegCode = Server().iSettings->regCode.Mid(5)[0]; // salva il nuovo salt nella configurazione...
			Server().iSettings->regCode64 = regCode64;
			Server().NotifyChange(this, ERegistrationChange);
		}
	} else
	{
		Server().iSettings->regCode.Copy(oldCode); // ripristina il vecchio codice (null oppure valido...)
		
		// ANTICRACK: Chiave con cui verra' decifrato il RegCode
		Server().iSettings->imeiOnStore = oldImeiOnStore;
		// ANTICRACK
	}
	__LOG("Regged:%d", regged);

    TPckgBuf<TInt> p(regged);
    aMessage.WriteL(0, p);
	__LOG_TXT("End RegisterL");
	aMessage.Complete(KErrNone);
}



void CMySession::ServiceL(const RMessage2& aMessage)
//
// Handle a client request.
// Leaving is handled by CMyServer::ServiceError() which reports
// the error code to the client
//
	{
	__LOG("ServiceL:%d", aMessage.Function());
 
	switch (aMessage.Function() + Server().iSettings->iFakeRegistration6)
		{
	case EIsEnabled:
		IsEnabledL(aMessage);
		break;
	case EPassword:
		PasswordL(aMessage);
		break;
	case EGetVersion:
		GetVersionL(aMessage);
		break;		
	case ESetPassword:
		SetPasswordL(aMessage);
		break;
	case EGetImei:
		ImeiL(aMessage);
		break;
	case ESetEnabled:
		SetEnabledL(aMessage);
		break;
	case EGetRegCode:
		GetRegCodeL(aMessage);
		break;
	case EAddProtectedUid:
		AddProtectedUidL(aMessage);
		break;
	case ERemProtectedUid:
		RemProtectedUidL(aMessage);
		break;
	case EGetProtectedList:
		GetProtectedListL(aMessage);
		break;
	case ERegister:
		RegisterL(aMessage);
		break;
	default:
		__LOG_TXT("!!!PanicClient!!!");
		PanicClient(aMessage, EPanicIllegalFunction);
		// FIXED in 2.0 aMessage.Complete(KErrNone);
		break;
		}
	}


void CMySession::ServiceError(const RMessage2& aMessage, TInt aError)
//
// Handle an error from CMySession::ServiceL()
// A bad descriptor error implies a badly programmed client, so panic it;
// otherwise use the default handling (report the error to the client)
//
	{
	__LOG("!!!ServiceError:%d!!!", aError);
	if (aError==KErrBadDescriptor)
		PanicClient(aMessage,EPanicBadDescriptor);
	CSession2::ServiceError(aMessage, aError);
	}



void PanicClient(const RMessagePtr2& aMessage, TMyPanic aPanic)
//
// RMessage::Panic() also completes the message. This is:
// (a) important for efficient cleanup within the kernel
// (b) a problem if the message is completed a second time
//
	{
	aMessage.Panic(KPanic, aPanic);
	}



