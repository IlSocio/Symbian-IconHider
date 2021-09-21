

// TODO: Al max ne riesce a nascondere 25... Per sicurezza lo limito a 20...
// Switca da un profilo ad un altro tramite hotkey... Pen + Tasto vedi programma screenshot
// 
// 1) Componi il numero di telefono del profilo per attivarlo?
// 2) Quando si verifica un certo evento mostra la box di selezione del profilo e lo seleziona da li?
// 3) HotKey associato al profilo, oppure HotKey associato al programma?
//		HotKey non va bene per i cell. touch.
//
// Tiene pigiato il bottone Rosso per 3 secondi per far comparire il menu' di switch tra un profilo ed un'altro...
// Lancia l'applicazione passandogli un parametro? e l'applicazione presenta la box di scelta del profilo.
// BISOGNA PROVARE: Forse non e' possibile intercettare il bottone rosso perche' portera' alla chiusura della box appena lanciata...
// Si puo' anche pensare di lanciare l'applicazione quando viene rilasciato il rosso dopo che e' stato tenuto per 3 secondi.
//
// La trial ha la possibilita' di modificare solo 2 profili.
// La trial puo' nascondere soltanto 2 applicazioni.
// La trial termina dopo 10 giorni di utilizzo. Check effettuato dal processo che gira in background.
// Rename Profile / New Profile
// 
// Visualizza il profilo attivo nella schermata principale del programma.
// Cambia opzioni Always / Never con soltanto Attivo / Disattivo
// Se lo imposto attivo gira in background e nascode le app.
// Se lo imposto su disattivo non gira in background e visualizza le app.
//
// Status: 
// Crea Nuovo Profilo
// Profilo Corrente
// -
// Rinomina Profilo / Elimina Profilo
//

/*
========================================================================
 Name        : MainAppUi.cpp
 Author      : Marco Bellino
 Copyright   : FuzzyByte
 Description : 
========================================================================
*/
#include "MainAppUi.h"
#include "MainAPPLICATION.h"
#include "main.hrh"

#include <e32Math.h>
#include <eikmenub.h>
#include <akncontext.h>
#include <akntitle.h>
#include <aknnavide.h>
#include <aknnavi.h>
#include <akntabgrp.h>
#include <aknquerydialog.h>
#include <stringloader.h>
#include <hal.h>
#include <hal_data.h>
#include <bautils.h>

#include <IconHider_20024264.rsg>
#include <IconHiderImg_20024264.mbg>

#include <FuzzyByte\IconHider\client.h>
#include <FuzzyByte\Notes.h>
#include <FuzzyByte\Phone.h>
#include <FuzzyByte\FileUtils.h>
#include <FuzzyByte\GuiUtils.h>
#include <FuzzyByte\Utils.h>
#include <FuzzyByte\FuzProducts.h>
#include <FuzzyByte\RepoUtils.h>
//#include <FuzzyByte\IconHider\Data.h>

_LIT(KBuyServerUrl, "http://www.Symbian-Toys.com/Buy.aspx?");
_LIT(KRegServerUrl, "http://www.Symbian-Toys.com/LicRequest.aspx?");


/**
 * Construct the CMainAppUi instance
 */ 
CMainAppUi::CMainAppUi()
	{
	iViewsCounter=(ELastViewId);
	}

/** 
 * The appui's destructor removes the container from the control
 * stack and destroys it.
 */
CMainAppUi::~CMainAppUi()
	{
	delete iTimer;
	delete iProgressBar;
	if ( iNaviDecorator_ != NULL )
		{
		delete iNaviDecorator_;
		iNaviDecorator_ = NULL;
		}
/*	for (TInt i=0; i<iProfFiles.Count(); i++)
		{
		delete iProfFiles[i];
		iProfFiles[i] = NULL;
		delete iProfNames[i];
		iProfNames[i] = NULL; 
		}*/
	}

void CMainAppUi::InitializeContainersL()
	{
/*	Notes::Debug_InfoL(_L("LocalZoom:%d"), LocalUiZoom());
	SetLocalUiZoomL( EAknUiZoomVerySmall );
	Notes::Debug_InfoL(_L("LocalZoom:%d"), LocalUiZoom());*/

	CAknNavigationControlContainer* naviPane = ( CAknNavigationControlContainer* )
		StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) );
	iNaviDecorator_ = naviPane->ResourceDecorator();
	if ( iNaviDecorator_ != NULL )
		{
		iNaviTabs1 = ( CAknTabGroup* ) iNaviDecorator_->DecoratedControl();	
		}

	// Replace Tabs with SVG graphics
#ifdef __WINSCW__
		TFileName svgFilePath = _L("z:\\resource\\apps\\IconHiderImg_20024264.mif");
#else
		TFileName svgFilePath = _L("\\resource\\apps\\IconHiderImg_20024264.mif");
		FileUtils::CompleteWithCurrentDrive(svgFilePath);
#endif
 	GuiUtils::StatusPaneReplaceNaviTabIconL(StatusPane(), iNaviDecorator_, ESettingsViewId, svgFilePath, EMbmIconhiderimg_20024264Tab_sett);
// 	GuiUtils::StatusPaneReplaceNaviTabIconL(StatusPane(), iNaviDecorator_, EProtectionViewId, svgFilePath, EMbmIconhiderimg_20024264Tab_prot);
 	
 	iTimer = CTimeOutTimer::NewL(*this);
	iProgressBar = CProgressBar::NewL(this);
	
	// Initialize iIsRegistered;
	RFs& fs = iEikonEnv->FsSession();
	
	TFileName fname = _L("reg.dat");
	FileUtils::CompleteWithPrivatePathL(fs, fname); 
	iIsRegistered = BaflUtils::FileExists(fs, fname);
	
	iSettView = CSettingsView::NewL( iIsRegistered );
	AddViewL( iSettView );

	// Add ProtectionViews...
	iViewsCounter++;
	AddNewProfileTabL( iViewsCounter );
	
	SetDefaultViewL( *iSettView );
	ActivateLocalViewL( iSettView->Id() );
	SetActiveTabByViewId( iSettView->Id() );
	
	iTimer->AfterTime( TTimeIntervalSeconds(1) );
 	}
 

void CMainAppUi::TimerExpiredL(TAny* src)
	{
	if (!iIsRegistered)
		Notes::ShowLongTextL( iEikonEnv->FsSession(), _L("Trial.txt"), R_LONGTEXT_DIALOG, R_DEMO);
	}


void CMainAppUi::AddNewProfileTabL( TInt aViewId )
	{
#ifdef __WINSCW__
		TFileName svgFilePath = _L("z:\\resource\\apps\\IconHiderImg_20024264.mif");
#else
		TFileName svgFilePath = _L("\\resource\\apps\\IconHiderImg_20024264.mif");
		FileUtils::CompleteWithCurrentDrive(svgFilePath);
#endif
	iNaviTabs1->AddTabL( aViewId, KNullDesC(), NULL, NULL );
 	GuiUtils::StatusPaneReplaceNaviTabIconL(StatusPane(), iNaviDecorator_, aViewId, svgFilePath, EMbmIconhiderimg_20024264Tab_prot);
	CProtectionView* protView = CProtectionView::NewL( aViewId, EFalse, iIsRegistered );
	AddViewL( protView );
	
	iProtView = protView;
	UpdateTabSizeL();
	}


void CMainAppUi::UpdateTabSizeL()
	{	
	switch (iNaviTabs1->TabCount())
		{
		case 2:
			iNaviTabs1->SetTabFixedWidthL(KTabWidthWithTwoTabs);
		break;
		case 3:
			iNaviTabs1->SetTabFixedWidthL(KTabWidthWithThreeTabs);
		break;
		default:
			iNaviTabs1->SetTabFixedWidthL(KTabWidthWithFourTabs);
		break;
		}	
	iNaviTabs1->DrawNow();
	}


void CMainAppUi::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane )
{
	// Invia -> via BT / via IR
	if (aResourceId == R_COMMON_MENU)
		{
			if (iIsRegistered)
				{
				GuiUtils::DeleteMenuItem(aMenuPane, ERegister);
				GuiUtils::DeleteMenuItem(aMenuPane, EBuyFull);
				}
		}
}


void CMainAppUi::RequestCancelled()
	{ 
	if (iWebReg)
        iWebReg->CancelRegistrationRequestL();
	}


TBool CMainAppUi::GotRegCodeL(const TDesC& regCode64)
	{
#ifndef _TEST_
	RHider hider;
	CleanupClosePushL(hider);
	User::LeaveIfError(hider.Connect());
	TInt regResult = hider.Register(regCode64);
	CleanupStack::PopAndDestroy(&hider);
	if (!regResult)
	{
		Notes::ErrorL(R_ERR_CODE);
		return EFalse;
	}
#endif
	
	Notes::ConfirmationL(R_OK_CODE);
	RFs &fs= iCoeEnv->FsSession();

	TFileName fname = _L("reg.dat");
	FileUtils::CompleteWithPrivatePathL(fs, fname);
	TBuf8<100> buf;
	for (TInt i=0; i<10; i++)
		{
		TUint32 num = Math::Random();
		buf.AppendNum( num, EHex );
		} 
	FileUtils::DumpToFileL( fs, fname, buf, EFalse);
	
	iIsRegistered = ETrue;
	iSettView->UpdateViewL( ETrue );
	iProtView->UpdateViewL( ETrue );
	return ETrue;
/*	User::LeaveIfError(guard.Connect());
	CleanupClosePushL(guard);
	TInt regResult = guard.Register(regCode);
	if (!regResult)
	{
		Notes::ErrorL(R_ERR_CODE);
		return EFalse;
	}

	if (regResult == 2)
	{
		iIsRegistered++;
	} else
	{
		// regResult = 1
		iIsRegistered = regResult + 1;
	}
	TBuf8<10> data;
	data.AppendNum(iIsRegistered);
	CleanupStack::PopAndDestroy();

	Notes::ConfirmationL(R_OK_CODE);
	RFs &fs= iCoeEnv->FsSession();
	RBuf regName( Utils::CompleteWithPrivatePathL(fs, iMachineId) );
	regName.CleanupClosePushL();
	RFile file;
	file.Create(fs, regName, EFileWrite);
	file.Write(data);
	file.Close();
	CleanupStack::PopAndDestroy(); // regName

	if (iTabGroup->ActiveTabId() == EGuardGUIView1Tab)
	{
		__FLOG(_T8("UpdateContainerL"));
		this->iView1->UpdateContainerL(iIsRegistered);
	}
	return ETrue;*/
	}


/*
void CMainAppUi::ShowLongTextFromFileL(const TDesC& fileName, const TInt aTitleRes)
	{
	HBufC* title = iCoeEnv->AllocReadResourceLC(R_EDITIONS_HEADER);
	RFs &fs = iCoeEnv->FsSession();
	TFileName fullFileName = fileName;
	FileUtils::CompleteWithPrivatePathL(fs, fullFileName);

#ifdef __WINSCW__
	if (BaflUtils::FileExists(fs, fullFileName))
#endif		
		{
		RBuf text(FileUtils::ReadTextFileL(fs, fullFileName));
		text.CleanupClosePushL();
		CAknMessageQueryDialog *dialog = CAknMessageQueryDialog::NewL(text);
		dialog->PrepareLC(R_LONGTEXT_DIALOG);
		dialog->SetHeaderTextL(*title);
		dialog->RunLD();
		CleanupStack::PopAndDestroy(&text);
		}
	CleanupStack::PopAndDestroy(title);
	}
*/

#include <CNTDB.H>
#include <caliterator.h>
#include <caltime.h>
#include <calcommon.h>
#include <cemailaccounts.h>
#include <calsession.h>
#include <calentryview.h>
#include <mmf\common\mmfcontrollerpluginresolver.h>


/**
 * Handle a command for this appui (override)
 * @param aCommand command id to be handled
 */
void CMainAppUi::HandleCommandL( TInt aCommand )
	{
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{ // code to dispatch to the AppUi's menu and CBA commands is generated here
		case EProtectionView:
			{
			TUid viewId = iProtView->Id();
			ActivateLocalViewL( viewId );
			SetActiveTabByViewId( viewId );
			break;
			}
		case EAknSoftkeyBack:
			{
			TUid viewId = iSettView->Id();
			ActivateLocalViewL( viewId );
			SetActiveTabByViewId( viewId );
			break;
			}
		case ERegister:
			{
			// Popup Using LicenseID / Using RegCode
			switch ( GuiUtils::ShowItemPopupL(R_REGISTER_POPUP) )
				{
				case 0:
					{
					HandleCommandL( ERegisterNet );
					return;
					}
				case 1:
					{
					HandleCommandL( ERegisterCode );
					return;
					}
				default:
					return;
				}
			break;
			}
		case EBuyFull:
			{

			// 1001 = Guardian PLATINUM
			// 1002 = Guardian GOLD 
			// 1003 = Guardian Upgrade
			// 2001 = Icon Hider
			FuzProducts::BuySoftwareL( KBuyServerUrl, EIconHiderFull );
			break; 
			}
		case ERegisterNet:
			{
			TBuf<8> parte1;
			TBuf<8> parte2;
			TBool good = EFalse;
			do {
				CAknMultiLineDataQueryDialog* dlg = CAknMultiLineDataQueryDialog::NewL(parte1, parte2);
				dlg->SetPredictiveTextInputPermitted(EFalse);
				if (!dlg->ExecuteLD(R_REGISTER_NET_DIALOG))
					return;
				good = FuzProducts::IsGoodLicenseID(parte1, parte2);
				if (!good)
					{
					Notes::ErrorL(R_ERR_LICNOTFOUND);
					}
			} while (!good);
			TBuf<16> licenseID = parte1;
			licenseID.Append(parte2);

			// Se e' il caso crea iWebReg
			if (iWebReg == NULL)
				{
				TRAPD(err, iWebReg = CFuzWebRegister::NewL( KRegServerUrl ));
				if (err != KErrNone) return;
				}
			
			// Chiamata al server per effettuare la Registrazione...
			iProgressBar->StartRequestL( R_WAIT_DIALOG, TTimeIntervalSeconds(120) );
			TBuf<20> regCode;
			TResponseID resp;
			TInt ris = iWebReg->SendRegistrationRequestL(0, KGUI_VERSION, licenseID, regCode, resp);
			iProgressBar->FinishRequestL();
			
			if (ris == KErrCancel) // Cancelled
				return;
			if (resp == EErrNone)
				{
				GotRegCodeL(regCode);
				return;
				}
			TInt res = 0;
			switch (resp)
				{
				case ELicenseIDNotFound:			// Da Gestire... "LicenseID non valido"
					res = R_ERR_LICNOTFOUND;
					break;
				case ELicenseIDHasDifferentIMEI:	// Da Gestire... "LicenseID gia' registrato"
					res = R_ERR_LICDIFIMEI;
					break;
				case EBannedFor10Minutes:			// Da Gestire... "Troppe richieste, riprovare piu' tardi"
					res = R_ERR_LICBANNED;
					break;
				case EUnknownDataReceived:			// Da Gestire... "Errore nella risposta del server, riprovare piu' tardi"
					res = R_ERR_LICUNKDATA;
					break;
				case ETransmissionError:			// Da Gestire... "Errore nella comunicazione, riprovare piu' tardi"
					res = R_ERR_LICTRANSMIS;
					break;
				default:
					res = R_ERR_LICGENERAL;			// Da Gestire... "Errore Generico"
					break;
				}
			// Messaggio di errore: "Servizio Temporaneamente Non Disponibile: Riprovare Piu' Tardi"
			Notes::ErrorL(res);
			break;
			}
		case ERegisterCode:
			{
			TBuf<10> regCode64;
			do {
				CAknTextQueryDialog* dlg = new( ELeave ) CAknTextQueryDialog( regCode64 );
				dlg->SetPredictiveTextInputPermitted(EFalse);
				if ( !dlg->ExecuteLD(R_REGISTER_DIALOG) )
					return; // Pigiato Cancel
			} while ( !GotRegCodeL(regCode64) );
			break;
			}
		case EAbout:
			{			
			// About screen... Show FuzzyByte logo...
			Notes::ShowLongTextL( iEikonEnv->FsSession(), _L("about.txt"), R_LONGTEXT_DIALOG, R_ABOUT_HEADER);		
			break;
			}
		default:
			break;
		}
	
		
	if ( !commandHandled )  
		{
		if ( aCommand == EAknSoftkeyExit || aCommand == EEikCmdExit)
			{
/*			RFs& fs = iEikonEnv->FsSession();
			CData* data = CData::NewLC();
			data->LoadData( fs );
			
			if (data->enabled == EAuto)
				{
				HBufC* buf = data->GetProtectedAppsLC();
			#ifndef __WINSCW__
				RepoUtils::SetL( 0x101f8847, 8, _L("") );
				RepoUtils::SetL( 0x101f8847, 8, buf->Des() );	// TODO: Read key pair from config...
			#endif
				CleanupStack::PopAndDestroy(buf);
				}

			CleanupStack::PopAndDestroy(data);
			
			// Bring the main menu' in foreground, so user will be forced to refresh the app list.
			// Questo perche' altrimenti l'icona del progr. nascosto potrebbe ancora essere visibile, ma non piu' accessibile... questo genera inconsistenza (parte una app differente da quella selezionata).
			const TUid KIdleUid = {0x101FD64C};
			TApaTaskList tasklist( iEikonEnv->WsSession() );
			TApaTask TaskMenu = tasklist.FindApp(KIdleUid);
			if (TaskMenu.Exists())
				TaskMenu.BringToForeground();*/

			Exit();
			}
		}
	}

/** 
 * Override of the HandleResourceChangeL virtual function
 */
void CMainAppUi::HandleResourceChangeL( TInt aType )
	{
//	CAknViewAppUi::HandleResourceChangeL( aType );
	CAknAppUi::HandleResourceChangeL(aType);
	if (aType == KEikDynamicLayoutVariantSwitch)
		{
		if (iSettView)
			iSettView->HandleClientRectChange();
		if (iProtView)
			iProtView->HandleClientRectChange();
		}
/*	void CGuardGUIAppUi::HandleResourceChangeL(TInt aType)
		{
		CAknAppUi::HandleResourceChangeL(aType);
		if (aType == KEikDynamicLayoutVariantSwitch)
			{
			if (iSplashView)
				iSplashView->HandleClientRectChange();
			if (iView1)
				iView1->HandleClientRectChange();
			if (iView2)
				iView2->HandleClientRectChange();
			if (iView3)
				iView3->HandleClientRectChange();
			if (iView4)
				iView4->HandleClientRectChange();
			}
		//Controls derived from CCoeControl, handled in a container class
		//iAppContainer->HandleResourceChange( aType );
		}*/
	}

/** 
 * Override of the HandleKeyEventL virtual function
 * @return EKeyWasConsumed if event was handled, EKeyWasNotConsumed if not
 * @param aKeyEvent 
 * @param aType 
 */
TKeyResponse CMainAppUi::HandleKeyEventL(
		const TKeyEvent& aKeyEvent,
		TEventCode aType )
	{
	TVwsViewId activeViewId;
	if ( GetActiveViewId( activeViewId ) == KErrNone 
		&& iNaviTabs1->TabIndexFromId( activeViewId.iViewUid.iUid ) 
			== KErrNotFound )
		{
		return EKeyWasNotConsumed;
		}
	
	TInt active = iNaviTabs1->ActiveTabIndex();
	TInt count = iNaviTabs1->TabCount();
	
	switch ( aKeyEvent.iCode )
		{
		case EKeyLeftArrow:
			if ( active > 0 )
				{
				active--;
				TUid viewId = TUid::Uid( iNaviTabs1->TabIdFromIndex( active ) );
				ActivateLocalViewL( viewId );
				// Il SetActive qui sotto serve come fix per una anomalia strana...
				// Se l'applicazione e' protetta con Guardian, la si lancia, poi si esce lasciandola in background.
				// Avviandola di nuovo sara' richiesto il codice segreto ma non si potra' piu' navigare con i Tabs.
				SetActiveTabByViewId( viewId );
				return EKeyWasConsumed;
				}
			break;
		case EKeyRightArrow:
			if ( active + 1 < count )
				{
				active++;
				TUid viewId = TUid::Uid( iNaviTabs1->TabIdFromIndex( active ) );
				ActivateLocalViewL( viewId );
				SetActiveTabByViewId( viewId );
				return EKeyWasConsumed;
				}
			break;
		default:
			return EKeyWasNotConsumed;
		}	
				
	return EKeyWasNotConsumed;
	}

/** 
 * Override of the HandleViewDeactivation virtual function
 *
 * @param aViewIdToBeDeactivated 
 * @param aNewlyActivatedViewId 
 */
void CMainAppUi::HandleViewDeactivation( 
		const TVwsViewId& aViewIdToBeDeactivated, 
		const TVwsViewId& aNewlyActivatedViewId )
	{
	CAknViewAppUi::HandleViewDeactivation( 
			aViewIdToBeDeactivated, 
			aNewlyActivatedViewId );
	if ( aNewlyActivatedViewId.iAppUid == KUidMainApplication )
		{
		// Notes::Debug_InfoL(_L("Deactivation: %x"), aNewlyActivatedViewId.iViewUid.iUid );
		// SetActiveTabByViewId( aNewlyActivatedViewId.iViewUid );
		}
	}

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here. 
 */ 
void CMainAppUi::ConstructL()
	{
	TUint32 EAknTouchCompatibleFlag = 0x00800000;
	TUint32 EAknSingleClickCompatibleFlag = 0x01000000;
	
	TUint32 flag = 0;
	if (GuiUtils::IsTouchSupportedL())
		flag = EAknTouchCompatibleFlag;
	
	BaseConstructL( EAknEnableSkin | flag );
	InitializeContainersL();
	}


void CMainAppUi::SetActiveTabByViewId( TUid aViewId )
	{
	if ( iNaviTabs1 != NULL 
		&& iNaviTabs1->TabIndexFromId( aViewId.iUid ) != KErrNotFound )
		{
		iNaviTabs1->SetActiveTabById( aViewId.iUid );
		}
	}

