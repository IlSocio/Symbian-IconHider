/*
============================================================================
 Name        : CSettingsView from GuardGUIView2.h
 Author      : Marco Bellino
 Version     :
 Copyright   : All Rights Reserved
 Description : CSettingsView implementation
============================================================================
*/
#include "SettingsView.h"
#include "main.hrh"
#include "Settings.hrh" // Per avere i valori di EChange, EEnable, EDisable

// INCLUDE FILES
#include <aknradiobuttonsettingpage.h>
#include <aknviewappui.h> 
#include <avkon.hrh>
#include <aknconsts.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <aknlists.h>
#include <CPbkContactEngine.h>
#include <RPbkViewResourceFile.h>
#include <CPbkSingleEntryFetchDlg.h>
#include <cntdb.h>
#include <CPbkSmsAddressSelect.h>
#include <TPbkContactItemField.h>
#include <eikmenup.h>
#include <akntitle.h>
#include <aknutils.h>
#include <featdiscovery.h> 
#include <Featureinfo.h>
#include <bautils.h>

#include <FuzzyByte\notes.h>
#include <FuzzyByte\queries.h>
#include <FuzzyByte\guiutils.h>
#include <FuzzyByte\FileUtils.h>
#include <FuzzyByte\Processes.h>
#include <FuzzyByte\InboxAttachment.h>

#include <IconHider_20024264.rsg>

_LIT(KUpdServerUrl, "http://www.Symbian-Toys.com/CheckUpdates.aspx?");

/*#include <FuzzyByte\RepoUtils.h>
#include <FuzzyByte\IconHider\Data_consts_20024264.h>
#include <FuzzyByte\IconHider\Data.h>*/


CSettingsView::CSettingsView(TBool regged) : CAknView()
	{
	iRegistered = regged;
	}

		
CSettingsView* CSettingsView::NewL(TBool regged)
	{
	CSettingsView* self = CSettingsView::NewLC(regged);
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CHelloWorldContainerView
 */
CSettingsView* CSettingsView::NewLC(TBool regged)
	{
	CSettingsView* self = new ( ELeave ) CSettingsView(regged);
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CSettingsView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CSettingsView::ConstructL()
	{
	BaseConstructL( R_SETTINGS_VIEW );
	iTouchSupported = GuiUtils::IsTouchSupportedL();
	iAcceptTouchEvents = EFalse;
	
	iProgressBar = CProgressBar::NewL(this);
	
	RFs& fs = iEikonEnv->FsSession();
	TFullName sisFileName = _L("IconHider.sisx");
	
	FileUtils::CompleteWithPrivatePathL( fs, sisFileName);
	iFileRedist = sisFileName.AllocL();
	
 	if (FileUtils::GetFileSize( fs, KFileTmpInst ) > 0)
 		{
 		BaflUtils::RenameFile( fs, KFileTmpInst, iFileRedist->Des() );
 		}
	iBtIr = CMyObexClient::NewL();
	
	iHasBT = CFeatureDiscovery::IsFeatureSupportedL(KFeatureIdBt);
	iHasIR = CFeatureDiscovery::IsFeatureSupportedL(KFeatureIdIrda);

#ifndef _TEST_	
	User::LeaveIfError( iHider.Connect() );
#endif
	}

// ---------------------------------------------------------
// CSettingsView::~CSettingsView()
// destructor
// ---------------------------------------------------------
//
CSettingsView::~CSettingsView()
	{
	delete iProgressBar;
	delete iBtIr;
	delete iFileRedist;
	if ( iContainer )
		{
		AppUi()->RemoveFromStack( iContainer );
		}
	delete iContainer;
	iHider.Close();
	}

// ---------------------------------------------------------
// TUid CSettingsView::Id()
//
// ---------------------------------------------------------
//
TUid CSettingsView::Id() const
	{
	return TUid::Uid( ESettingsViewId );
	}


TBool CSettingsView::AskCodeL(TDes& pass)
{
	TBool predict = !GuiUtils::IsTouchSupportedL();
	for(;;)
	{
		if (!Queries::AskTextL(R_CODE_VISIBLE_QUERY, pass, predict))
			return EFalse;

		pass.UpperCase();
		if (pass.Length()>=1)
			return true;

		Notes::WarningL(R_ERR_SHORT_CODE);
	}
}


void CSettingsView::HandleCommandL( TInt aCommand )
    {
	switch (aCommand)
		{
		case EChangeCode:
			{
			if (!iRegistered)
				{
				Notes::WarningL(R_ERR_UNAVAILINFREE);
				return;
				}
			// Ask new Ghost Code
			TPassCode pass;
			iHider.GetPassword(pass);
			if ( !AskCodeL( pass ) ) return;
			TInt ris = iHider.SetPassword(pass);
			ris = KErrNone;
			UpdateViewL( iRegistered );
			
/*			CAknTextQueryDialog* dlg = new( ELeave ) CAknTextQueryDialog( regCode64 );
			dlg->SetPredictiveTextInputPermitted(EFalse);
			if ( !dlg->ExecuteLD(R_REGISTER_DIALOG) )
				return; // Pigiato Cancel*/

			// iHider.SetPassword();
			break;
			}
		case EChangeStatus:
			{
			iHider.Close();
			iHider.Connect();
			TBool newStatus = !iHider.IsEnabled();
			iHider.SetEnabled(newStatus);			
			if (newStatus)	
				{
				// Avvia IconHider Background...
				Processes::LaunchL(_L("IconHiderAutoConsole_20024268"), 0x20024268);
				}
			UpdateViewL( iRegistered );
			break;
			}
		case ECheckUpdates:
			{
			CheckForUpdatesL();
/*			TUpdateStatus stat = CheckForUpdatesL();
			if (stat == ENoUpdateAvail)
				{
				return;
				}
			if (stat == EUpdateDownloaded)
				{
				// Exit();
				}*/
			break;
			}
		case ESendObex:
			{
			if (!iHasIR)
				{
				HandleCommandL( ESendBT );
				return;
				}
			if (!iHasBT)
				{
				HandleCommandL( ESendIR );
				return;
				}
				
			// Popup Send Using IR / Using BT
			switch ( GuiUtils::ShowItemPopupL(R_SEND_POPUP) )
				{
				case 0: 
					{
					HandleCommandL( ESendIR );
					return;
					}
				case 1:
					{
					HandleCommandL( ESendBT );
					return;
					}
				default:
					return;
				}
			break;
			}
		case ESendIR:
			{
			if (iFileSizeRedist > 0)
				{
				TRAP_IGNORE( iBtIr->ShowQueryAndSendFileL(iFileRedist->Des(), R_WAIT_DIALOG, EFalse, iFileSizeRedist) );
				}
			break;
			}
		case ESendBT:
			{
			if (iFileSizeRedist > 0)
				{
				TRAP_IGNORE( iBtIr->ShowQueryAndSendFileL(iFileRedist->Des(), R_WAIT_DIALOG, ETrue, iFileSizeRedist) );
				}
			break;
			}
 
		/*case EChangeProfile:
			{
			// TODO: Move to IconHiderLib... Will be called by the Console too, peraps by the GUI during startup.
			// TODO: Retrieve profile list and compose the array
			 CDesCArrayFlat* array = new (ELeave) CDesCArrayFlat(4);
			 array->AppendL(_L("<DEFAULT>"));
			 array->AppendL(_L("two"));
			 array->AppendL(_L("three"));
			 array->AppendL(_L("four"));
			 TInt index = GuiUtils::ShowItemPopupL(array);
			// TODO: retrieve selected value.
			// iData->currentProfileFile = _L("aaa");
			
			// TODO: ***
			//CData::GetProfileNameL( iEikonEnv->FsSession(), iData.GetCurrentProfile(), iSetData.currProfileName );
			//UpdateViewL( iSetData.registered );
			break;
			}*/
		}
	AppUi()->HandleCommandL(aCommand);
    }


// ---------------------------------------------------------
// CSettingsView::HandleClientRectChange()
// ---------------------------------------------------------
//
void CSettingsView::HandleClientRectChange()
	{
	if ( iContainer )
		{
		iContainer->SetRect( ClientRect() );
		}
	}


void CSettingsView::UpdateViewL(TBool registered)
	{
	iRegistered = registered;
	if ( iContainer )
		{
		TBool enabled = EFalse;
#ifndef _TEST_
		enabled = iHider.IsEnabled();
#endif
		iContainer->UpdateListBoxViewL(enabled, iRegistered);
		} 
	}
 

// ---------------------------------------------------------
// CSettingsView::DoActivateL(...)
//
// ---------------------------------------------------------
//
void CSettingsView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
	{
	SetupStatusPaneL();
	
	if (!iContainer)
		{
		// Notes::Debug_InfoL(_L("DoActivateL"));
		RFs& fs = iEikonEnv->FsSession();
		
		iContainer = CSettingsListboxContainer::NewL(ClientRect(), NULL);	
		iContainer->SetMopParent(this);
		AppUi()->AddToStackL( *this, iContainer );	 
		
		UpdateViewL(iRegistered);
		
		iFileSizeRedist = FileUtils::GetFileSize( fs, iFileRedist->Des() );
		iHasRedistFile = (iHasBT || iHasIR) && (iFileSizeRedist > 0);
//		iContainer->UpdateListBoxViewL( iHider.IsEnabled(), iRegistered );
		}

	/*
	TBool expired = EFalse;
	TTime now;
	now.HomeTime();
	TMonth month = now.DateTime().Month();
	if ( (month >= EMarch) || (now.DateTime().Year() != 2012) )
		{
		expired = ETrue;
		// Show message this BETA version is expired! Please download the updated version.
		}*/
	
	// check expired...	
	/*if ( UpdateLock::NeedsUpdateL() )
		{
		TUpdateStatus stat = CheckForUpdatesL();
		if (stat == ENoUpdateAvail)
			{
			UpdateLock::DoneUpdateL();
			}
		if (stat == EUpdateDownloaded || stat == EAborted)
			{
			// TODO: Show message, Please install the new version!
			User::Exit( KErrNone );
			}
		}*/
   } 

// ---------------------------------------------------------
// CSettingsView::DoDeactivate()
//
// ---------------------------------------------------------  
//
void CSettingsView::DoDeactivate()
	{
	//	CleanupStatusPane();	<--- TODO:
	if ( iContainer ) 
		{
		AppUi()->RemoveFromStack( iContainer );
		delete iContainer; 
		iContainer = NULL;
		}
	}


void CSettingsView::DynInitMenuPaneL( TInt aResourceId,	CEikMenuPane* aMenuPane )
{
	AppUi()->DynInitMenuPaneL(aResourceId, aMenuPane);
	if (aResourceId == R_SETTINGS_MENU)
		{
			/*if (iSetData.registered)
				{
				GuiUtils::DeleteMenuItem(aMenuPane, ECheckUpdates);
				}*/
			// Check Esistenza file da inviare...
			if ((!iHasIR && !iHasBT) || iBtIr->IsBusy() || iFileSizeRedist == 0)
			{
				GuiUtils::DeleteMenuItem(aMenuPane, ESendObex);
			} else
			{
				if (!iHasIR)
					GuiUtils::DeleteMenuItem(aMenuPane, ESendIR);
				if (!iHasBT)
					GuiUtils::DeleteMenuItem(aMenuPane, ESendBT);
			}
		}
}



// deve dirmi se e' stato scaricato un aggiornamento
// se non c'e' 
// oppure se c'e' stato qualche errore
TUpdateStatus CSettingsView::CheckForUpdatesL()
	{
	if (iWebUpd == NULL)
		iWebUpd = CFuzWebUpdate::NewL( KUpdServerUrl );

	iProgressBar->StartRequestL( R_WAIT_DIALOG, TTimeIntervalSeconds(120) );
	TBuf8<100> url;
	TBool hasUpdates;
	TInt ris = iWebUpd->IsThereAnUpdatedVersionL( iRegistered, KGUI_VERSION, 0, url, hasUpdates);
	iProgressBar->FinishRequestL();

	if (ris == KErrCancel)
	{
		delete iWebUpd;	iWebUpd = NULL;
		return EAborted;
	}

	if (ris != KErrNone)
		{
		// Questo check e' stato aggiunto nella 1.03 prima non era presente.
		// Obbliga ad aggiornare trascorsi 15 gg. non e' sufficiente uno IAP non valido
		delete iWebUpd;	iWebUpd = NULL;
		// es. kerrtimedout
		Notes::ErrorL(R_ERR_LICTRANSMIS);
		return EAborted;
		}

	if (!hasUpdates)
	{
		Notes::InformationL(R_NO_UPDATE);
		delete iWebUpd; iWebUpd = NULL;	
		return ENoUpdateAvail;
	}

	
	// Dialog di conferma download aggiornamento...
	if ( Notes::QueryL(R_AVKON_SOFTKEYS_YES_NO, R_CONFIRM_UPDATE) != EAknSoftkeyYes )
		{
		delete iWebUpd; iWebUpd = NULL;
		return EAborted;
		}
/*	CAknQueryDialog* dlg = CAknQueryDialog::NewL();
	if ( !dlg->ExecuteLD(R_CONFIRM_DOWNLOAD) )
	{
		delete iWebUpd; iWebUpd = NULL;
		return;
	}*/

	// Read the TmpDownload filename
	iCoeEnv->FsSession().MkDirAll( KFileTmpInst );
	
	// Show Progress...
	iProgressBar->StartRequestL( R_WAIT_DIALOG, TTimeIntervalSeconds(120) );
	ris = iWebUpd->DownloadUpdatedVersionL(url, iCoeEnv->FsSession(), KFileTmpInst);
	iProgressBar->FinishRequestL();	
	delete iWebUpd;	iWebUpd = NULL;
	// Hide Progress...

	if (ris == KErrCancel)
		{
		// download incompleto elimina il file
		iCoeEnv->FsSession().Delete( KFileTmpInst );
		return EAborted;
		}
	if (ris != KErrNone)
		{
		// download incompleto elimina il file
		iCoeEnv->FsSession().Delete( KFileTmpInst );
		Notes::ErrorL(R_ERR_LICTRANSMIS);
		return EAborted;
		}

	Notes::ConfirmationL( R_DOWN_COMPLETED );

	// Salva il file nell'InBox...
	RFs &fs= iCoeEnv->FsSession();
	CInboxAttachment* saver = CInboxAttachment::NewLC();
	
	HBufC* details = StringLoader::LoadLC(R_WEB_NAME, iCoeEnv);
	saver->BT_IRDA_SaveFileToInboxL(fs, details->Des(), KFileTmpInst );
	CleanupStack::PopAndDestroy(details);
	CleanupStack::PopAndDestroy(saver);

	// fs.Delete( tmpDL );
	// Elimina il file che e' gia' stato scaricato e salvato nell'inbox.

//	AppUi()->Exit();
	return EUpdateDownloaded;
	}


void CSettingsView::RequestCancelled()
	{
	if (iWebUpd)
		iWebUpd->CancelDownloadL();
	}


void CSettingsView::HandleStatusPaneSizeChange()
	{
	CAknView::HandleStatusPaneSizeChange();
	
	// this may fail, but we're not able to propagate exceptions here
	TVwsViewId view;
	AppUi()->GetActiveViewId( view );
	if ( view.iViewUid == Id() )
		{
		TInt result;
		TRAP( result, SetupStatusPaneL() );
		}
	}

void CSettingsView::SetupStatusPaneL()
	{
	GuiUtils::StatusPaneSetupTitleL( StatusPane(), R_SETTINGS_TITLE_RESOURCE );
	}

