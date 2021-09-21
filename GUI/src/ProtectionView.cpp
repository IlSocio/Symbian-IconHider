/*
============================================================================
 Name        : CProtectionView from GuardGUIView2.h
 Author      : Marco Bellino
 Version     :
 Copyright   : All Rights Reserved
 Description : CProtectionView implementation
============================================================================
*/

// INCLUDE FILES
#include <akntitle.h>
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include <aknview.h>
#include <eiktxlbx.h> 
#include <aknlists.h>
#include <aknpopup.h>
#include <aknquerydialog.h>
#include <aknnotewrappers.h>
#include <aknmessagequerydialog.h>
#include  "ProtectionView.h"
#include  "ProtectionContainer.h"
#include  "Protection.hrh"
#include  <IconHider_20024264.rsg>
#include  "Main.hrh"
//#include "notes.h"
#include <FuzzyByte\FileUtils.h>
#include <FuzzyByte\RepoUtils.h>
#include <FuzzyByte\StringUtils.h>
#include <FuzzyByte\Notes.h>
#include <FuzzyByte\GuiUtils.h>
#include <CENTRALREPOSITORY.H>
//#include <FuzzyByte\IconHider\Data.h>
//#include <FuzzyByte\IconHider\ProfileData.h>


//const TUint KMaxSettingLength = NCentralRepositoryConstants::KMaxUnicodeStringLength;


// ================= MEMBER FUNCTIONS =======================
CProtectionView::CProtectionView(TInt aProfileId, TBool regged) : iViewId(aProfileId), iRegistered(regged)
	{	
	}

		
CProtectionView* CProtectionView::NewL(TInt aProfileId, TBool largeIcons, TBool regged)
	{
	CProtectionView* self = CProtectionView::NewLC(aProfileId, largeIcons, regged);
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CHelloWorldContainerView
 */
CProtectionView* CProtectionView::NewLC(TInt aProfileId, TBool largeIcons, TBool regged)
	{
	CProtectionView* self = new ( ELeave ) CProtectionView(aProfileId, regged);
	CleanupStack::PushL( self );
	self->ConstructL(largeIcons);
	return self;
	}


// ---------------------------------------------------------
// CProtectionView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CProtectionView::ConstructL(TBool largeIcons)
	{
	BaseConstructL( R_PROTECTION_VIEW );
	__LOG_OPEN("IconHider", "GUI_ProtView.txt")
	__LOG_TXT("-----------");

	if (GuiUtils::IsTouchSupportedL())
		iWasFindBoxVisible = EFalse;
	else
		iWasFindBoxVisible = ETrue;
	
	iLargeIcons = largeIcons;
    iProgressBar = CProgressBar::NewL(this);
#ifndef _TEST_
    iHider.Connect();
#endif
	}

// ---------------------------------------------------------
// CProtectionView::~CProtectionView()
// destructor
// ---------------------------------------------------------
//
CProtectionView::~CProtectionView()
	{
	__LOG_TXT("Destuctor");
	if ( iContainer )
		{
		AppUi()->RemoveFromStack( iContainer );
		delete iContainer;
		iContainer = NULL;
		}
	delete iProgressBar;
	iHider.Close();
	__LOG_TXT("End Destuctor");
	__LOG_CLOSE;
	}

// ---------------------------------------------------------
// TUid CProtectionView::Id()
//
// ---------------------------------------------------------
//
TUid CProtectionView::Id() const
	{
	return TUid::Uid( iViewId );
	}


void CProtectionView::UpdateViewL(TBool registered)
	{
	iRegistered = registered;
	}


void CProtectionView::HandleItemChangeEventL(const TInt32 uid, const TBool checked)
{
	// check e mostra messaggio di errore se si tenta di nascondere piu' di una app...
	if (!iRegistered) 
		{
		if (iContainer->CountProtectedL() > 1 && checked)
			{
			Notes::WarningL(R_ERR_ONLY1);
			iContainer->SetApplicationStatusL(uid, EFalse);
			// iContainer->UpdateListBoxViewL(EFalse);
			return;
			}
		}
		
	if (checked)
		{
		iHider.AddProtectedUid( uid );
		if (!iHider.IsEnabled() && iContainer->CountProtectedL() <= 1)
			{
			Notes::InformationL(R_ERR_ACTIVE);
			}
		}
	else
		{
		iHider.RemProtectedUid( uid );
		}	

	if (uid == 0x20024264 && checked)
		{
		// Show Message...
		TPassCode code;
		iHider.GetPassword(code);
		Notes::InformationL( R_ICONHIDER_HIDDEN, (code.PtrZ()) );
		}

	/* 
	// MOVED TO CData
	// Change Repository Value only if it is the current profile!!!
	if (iEnabled) 
		{
		HBufC* buf = CData::GetProtectedAppsLC(fs, iProfileFile);
		FileUtils::DumpToFileL( fs, _L("c:\\HandleItemChange.txt"), buf->Des() );
	#ifndef __WINSCW__
		RepoUtils::SetL( 0x101f8847, 8, buf->Des() );	// TODO: Read key pair from config...
/*		TBuf<1100> tbuf;
		RepoUtils::GetL( 0x101f8847, 8, tbuf );
		FileUtils::DumpToFileL( iEikonEnv->FsSession(), _L("c:\\HandleItemChange_REPO.txt"), tbuf );
		TBuf<200> tmp;
		tmp = _L("101F86E3.0001,101F86E3,101F86E3:0001,101F86E3::0001,101F86E3!0001,101F86E3(0001),101F86E3[0001],101F86E3{0001},101F86E3%0001%,101F86E3'0001',101F86E3%0001,101F86E3=0001,200131CC");
		RepoUtils::SetL( 0x101f8847, 3, tmp );	// TODO: Read key pair from config...
	#endif
		CleanupStack::PopAndDestroy(buf);
		}
		*/
}



void CProtectionView::DynInitMenuPaneL( TInt aResourceId,
										CEikMenuPane* aMenuPane )
	{
	AppUi()->DynInitMenuPaneL(aResourceId, aMenuPane);

	if (aResourceId == R_PROTECTION_MENU)
		{
			if (iContainer->IsMarkedL())
			{
				GuiUtils::DeleteMenuItem(aMenuPane, EAddApp);
			} else
			{
				GuiUtils::DeleteMenuItem(aMenuPane, ERemoveApp);
			}

			// Gestione di Icone Piccole Grandi
			if (iLargeIcons)
			{
				GuiUtils::DeleteMenuItem(aMenuPane, ELargeIco);
			} else
			{
				GuiUtils::DeleteMenuItem(aMenuPane, ESmallIco);
			}
		}
	}
 
/*
void CProtectionView::ProtectAllAppsL(TBool aFlag)
	{
	CArrayFixFlat<TApaAppInfo>* appList = iContainer->iFullAppList;
	iProgressBar->StartRequestL(R_PROGRESS_DIALOG, appList->Count());
	TInt i=0;
//	CData* data = CData::NewLC();
//	TInt res = data->LoadData( iEikonEnv->FsSession() );
	
	iCancel = EFalse;
	while (i<appList->Count() && !iCancel)
		{
		TApaAppInfo appInfo = (*appList)[i];
		// HandleItemChangeEventL(appInfo.iUid.iUid, EFalse);
		
		iContainer->SetApplicationStatusL(appInfo.iUid.iUid, aFlag);
		if (aFlag)
			{
			iData->AddProtectedApp(appInfo.iUid.iUid);
			}
		else
			{
			iData->RemProtectedApp(appInfo.iUid.iUid);
			}
		i++; 
		iProgressBar->IncreaseProgressL(1);
		}
	iData->SaveDataL( iEikonEnv->FsSession() );	
	
	iProgressBar->FinishRequestL();	
	}
*/

void CProtectionView::HandleCommandL(TInt aCommand)
	{
	switch ( aCommand )
		{
		case EAddApp:
		case ERemoveApp:
			{
				TInt32 uid = iContainer->GetSelectedUidL();
				if (uid <= 0) return;
				iContainer->ChangeApplicationStatusL(uid);
				TBool isMarked = iContainer->IsMarkedL();
				HandleItemChangeEventL(uid, isMarked);
				break;
			} 
	
		case ERemoveAll:
			{
			RemoveAllUidEntriesL();
			break;
			}		
/*			
		case EAddAll:
			{
			ProtectAllAppsL(ETrue);
			break;
			}*/
			
/*		case ELargeIco:
		case ESmallIco:
			{			
			// TODO: Change...
				iData->largeIcons = iLargeIcons;
				iData->SaveDataL( iEikonEnv->FsSession() );
				
				iContainer->UpdateListBoxViewL(iLargeIcons);
				// MarkApplicationsL();
				break;
			}*/
		default:
			{
			AppUi()->HandleCommandL( aCommand );
			break;
			}
		}
	}


void CProtectionView::SetIconSizeL(TBool large)
	{
	iLargeIcons = large;
	if (iContainer)
		iContainer->UpdateListBoxViewL( iLargeIcons );
	}


// ---------------------------------------------------------
// CProtectionView::HandleClientRectChange()
// ---------------------------------------------------------
//
void CProtectionView::HandleClientRectChange()
	{
	if ( iContainer )
		{
		iContainer->SetRect( ClientRect() );
		}
	}


void CProtectionView::RequestCancelled()
	{
	iCancel = ETrue;
	}


void CProtectionView::RemoveAllUidEntriesL()
	{
	__LOG_TXT("RemoveAllUidEntriesL");
	RArray<TInt32> uidList;
	CleanupClosePushL(uidList);	
	iHider.GetProtectedListL( uidList );
	
	// Cicla ed imposta tutti i marker delle varie applicazioni protette...
	for (TInt i=0; i<uidList.Count(); i++)
		{
		TInt32 uid = uidList[i];
		iHider.RemProtectedUid( uid );
		}
	CleanupStack::PopAndDestroy(&uidList);

	if (iContainer)
		{
		iContainer->UpdateListBoxViewL( iLargeIcons );
		}
	}



// ---------------------------------------------------------
// CProtectionView::DoActivateL(...)
//
// ---------------------------------------------------------
//
void CProtectionView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
	{
	SetupStatusPaneL();
	if (!iContainer)
		{
		RFs& fs = iEikonEnv->FsSession();
				
		iContainer = CProtectionContainer::NewL( ClientRect(), NULL, iLargeIcons );
		iContainer->SetMopParent(this);
		iContainer->SetObserver(this);
		AppUi()->AddToStackL( *this, iContainer );
		
/*		if (iData.GetCurrentProfile() == iProfileFile)
			{
			iData.SetEnabled( iData.IsEnabled() );
			}*/
		}
   }

// ---------------------------------------------------------
// CProtectionView::DoDeactivate()
//
// ---------------------------------------------------------
//
void CProtectionView::DoDeactivate()
	{
//	CleanupStatusPane();	<--- TODO:
	if ( iContainer )
		{
		iWasFindBoxVisible = iContainer->IsFindBoxVisible();
		AppUi()->RemoveFromStack( iContainer );
		delete iContainer;
		iContainer = NULL;
		}
	}

void CProtectionView::HandleStatusPaneSizeChange()
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

void CProtectionView::SetupStatusPaneL()
	{
	TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
	CEikStatusPane* sp = StatusPane();
	CEikStatusPaneBase::TPaneCapabilities subPaneTitle = sp->PaneCapabilities( titlePaneUid );
	if ( !subPaneTitle.IsPresent() || !subPaneTitle.IsAppOwned() )
		return;
//	CAknTitlePane* title = static_cast< CAknTitlePane* >( sp->ControlL( titlePaneUid ) );

	GuiUtils::StatusPaneSetupTitleL( StatusPane(), R_HIDDEN_TITLE_RESOURCE );
	}
	

