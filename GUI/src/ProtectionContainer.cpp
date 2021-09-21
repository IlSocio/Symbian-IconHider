// INCLUDE FILES
#include "ProtectionContainer.h"
//#include <FuzzyByte\IconHider\Data_consts_20024264.h>
#include <FuzzyByte\notes.h>
#include <aknutils.h>
#include <aknsfld.h>
#include <eiklabel.h>  // for example label control
#include <APGCLI.H>
#include <GULICON.H>
#include <akniconarray.h>
#include <eikclbd.h>

#include <akndef.H>
#include <eikdef.h>
#include <EIKENV.H>
#include <EIKAPPUI.H>
#include <EIKAPP.H>

#include <avkon.rsg>
//#include <avkon.rh>
#include <avkon.mbg>
#include <aknconsts.h>
#include "Main.hrh"
#include <FuzzyByte\GuiUtils.h>
#include <FuzzyByte\IconHider\client.h>


// ================= MEMBER FUNCTIONS =======================

CProtectionContainer::CProtectionContainer()
	{	
	}


CProtectionContainer* CProtectionContainer::NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		TBool largeIcons )
	{
	CProtectionContainer* self = CProtectionContainer::NewLC( 
			aRect, 
			aParent, 
			largeIcons);
	CleanupStack::Pop( self );
	return self;
	}


CProtectionContainer* CProtectionContainer::NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		TBool largeIcons)
	{
	CProtectionContainer* self = new ( ELeave ) CProtectionContainer();
	CleanupStack::PushL( self );
	self->ConstructL( aRect, aParent, largeIcons);
	return self;
	}
			

// ---------------------------------------------------------
// CProtectionContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CProtectionContainer::ConstructL(const TRect& aRect,
		const CCoeControl* aParent, 
		TBool largeIcons)
	{
	__LOG_OPEN("IconHider", "GUI_ProtCont.txt")
	__LOG_TXT("-----------");
	if ( aParent == NULL )
	    {
		CreateWindowL();
	    }
	else
	    {
	    SetContainerWindowL( *aParent );
	    }
    SetRect( aRect );	// Da richiamare prima della CreateListBoxL() per evitare di visualizzare sfondo bianco!

/*	iTouchSupported = EFalse;
	iAcceptTouchEvents = EFalse;
	iDBG_Down = 0;
	iDBG_Click = 0;*/

	iFindBox = NULL;
	iListBox = NULL;

//	iStyle = CAknSearchField::EAdaptiveSearch;
	iStyle = CAknSearchField::ESearch;
//	iStyle = CAknSearchField::EPopup;
	
	iFullAppList = GetApplicationsLC();
	CleanupStack::Pop( iFullAppList );

	CreateListBoxL( largeIcons, EFalse );

    ActivateL();
    DrawNow();
	}


TInt CProtectionContainer::CountProtectedL()
	{
	return iMarkedUids.Count();
	}


void CProtectionContainer::HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType)
	{
/*	if (aEventType == EEventEnterKeyPressed) // 5th Ed.
		{
		__LOG_TXT("EnterKeyPressed");
		}
	if (aEventType == EEventPenDownOnItem) // 5th Ed.
		{
		iDBG_Down++;
		TBuf<20> buf = _L("PenDown: ");
		buf.AppendNum(iDBG_Down);		
		__LOG_DES(buf);
		}
	if (aEventType == EEventItemClicked) // 5th Ed.
		{
		iDBG_Click++;
		TBuf<20> buf = _L("Item Clicked: ");
		buf.AppendNum(iDBG_Click);
		__LOG_DES(buf);
		}
	if (aEventType == EEventPenDownOnItem) // 5th Ed.
		{
		iTouchSupported = ETrue;
		iAcceptTouchEvents = ETrue;
		}*/
	switch (aEventType)
		{
		case EEventEnterKeyPressed:
		case EEventItemClicked:
			/*if (iTouchSupported)
				{ 
				if (!iAcceptTouchEvents)
					return;
				iAcceptTouchEvents = EFalse;
				}*/
			ChangeApplicationStatusL( GetSelectedUidL() );
			TBool checked = IsMarkedL();
			if (iObserver)
				{
				iObserver->HandleItemChangeEventL( GetSelectedUidL() , checked);    	
				}
			break;
		default:
			break;
		} 
	}
 

void CProtectionContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
	{
    // Remember to call base class implementation
    CCoeControl::HandlePointerEventL(aPointerEvent);


/*  
 *  TODO: Verifica se e' un evento nella listbox o nella findbox...  
  	if (aPointerEvent.iType == TPointerEvent::EButton1Up)
   	{
   		ChangeApplicationStatusL( GetSelectedUidL() );
		TBool checked = IsSelectedMarkedL();
		iObserver->HandleItemChangeEventL( GetSelectedUidL() , checked);    
    }*/
	
/*    if (IsFindBoxVisible())
    	{
    	iFindBox->HandlePointerEventL(aPointerEvent);
    	return;
    	}

    if (iListBox)
    	iListBox->HandlePointerEventL(aPointerEvent);*/
    // Your additional code here
    // ...	
	}


TBool CProtectionContainer::IsFindBoxVisible()
{
	return (iFindBox != NULL);
}


void CProtectionContainer::SetObserver(MMultiSelectionObserver* observer)
	{
	iObserver = observer;
	}


// Destructor
CProtectionContainer::~CProtectionContainer()
	{
	__LOG_TXT("Destructor");
	__LOG_TXT("FindBox");
	DeleteFindBox();
	__LOG_TXT("ListBox");
	if (iListBox)
	{
		delete iListBox;
		iListBox = NULL;
	}
	__LOG_TXT("AppList");
	if (iFullAppList)
		delete iFullAppList;
	__LOG_TXT("Markers");
	iMarkedUids.Reset();
	iMarkedUids.Close();
	__LOG_TXT("End Destructor");
	__LOG_CLOSE;
	}

_LIT(KTAB, "\t");

void CProtectionContainer::CreateListBoxItemL( TDes& aBuffer, TInt aIconIndex, const TDesC& aMainText )
	{
	// "0\tTextLabel\t1\t2"
//	_LIT(KListItemFormat, "%d\t%S\t\t");
	_LIT(KListItemFormat, "%d\t%S");
	aBuffer.Format(KListItemFormat, aIconIndex, &aMainText);
	} 

void CProtectionContainer::AddToItemListL(CDesCArray& array, const TDesC& name, const TUint icon)
	{
	TBuf<512> listString;
	CreateListBoxItemL(listString, icon, name);
	array.AppendL( listString );
	}


/*HBufC* CProtectionContainer::GetItemStringLC(const TDesC& name, const TUint icon)
	{
	// Non si e' utilizzato realloc xche' il cleanupstack avrebbe puntato ancora
	// alla vecchia locazione della stringa

//	"0\tTESTO\t\t"

    HBufC* ris = HBufC::NewLC( name.Length() + 10 );
	TPtr ptr = ris->Des();
	ptr.AppendNum( icon );
	ptr.Append( KTAB );
	ptr.Append( name );
	ptr.Append( KTAB );
	ptr.Append( KTAB );
	return ris;
	}*/


TInt32 CProtectionContainer::GetSelectedUidL()
	{
	if (iListBox == NULL)
		return 0;
	TInt index = GetOriginalIndex();
	if (index<0)
		return 0;
	TApaAppInfo appInfo = (*iFullAppList)[index];
	return appInfo.iUid.iUid;
	}


void CProtectionContainer::UpdateListBoxViewL(TBool isLarge)
	{
	CreateListBoxL( isLarge, IsFindBoxVisible() );
    iListBox->DrawNow();
	}


TInt CProtectionContainer::GetOriginalIndex() const
{
	if (iListBox == NULL)
		return -1;
	TInt indexFilt = iListBox->CurrentItemIndex();
	return GuiUtils::GetUnFilteredIndex(iListBox, indexFilt);
}


TBool CProtectionContainer::SetApplicationStatusL(TInt32 uid, TBool protect)
	{
	TInt i=GetIndexFromUidL(uid);
	if (i<0)	// L'applicazione non esiste...
		return EFalse;

	MDesCArray* textArray = iListBox->Model()->ItemTextArray();
	CDesCArray* listBoxItems = static_cast<CDesCArray*>( textArray );
	TBuf<512> listString = textArray->MdcaPoint(i);
	//Notes::Debug_InfoL(listString);
	TBuf<2> newIcon  = _L("0");
	if (!protect)
		{
		newIcon.Zero();
		newIcon.AppendNum(i+1);
		}

	TInt posTab = listString.Find(_L("\t"));
	listString.Replace(0, posTab, _L(""));
	listString.Insert(0, newIcon);
	
	// update item
	listBoxItems->Delete( i );
	listBoxItems->InsertL( i, listString );
		
	// Update new item
	TInt indexFilt = GuiUtils::GetFilteredIndex(iListBox, i);
	if (indexFilt >= 0)
		{
		iListBox->DrawItem( indexFilt );
		}   

	TInt markedPos = iMarkedUids.Find( uid );
	if (markedPos >= 0 && !protect) 
		iMarkedUids.Remove( markedPos );
	
	if (markedPos < 0 && protect) 
		iMarkedUids.Append( uid );
	
	return ETrue;
	}


TInt CProtectionContainer::GetIndexFromUidL(TInt32 uid)
	{
	for (TInt i=0; i<iFullAppList->Count(); i++)
		{
		if ((*iFullAppList)[i].iUid.iUid == uid)
			return i;
		}
	return -1;
	}


TBool CProtectionContainer::ChangeApplicationStatusL(TInt32 uid)
	{
	__LOG("ChangeApplicationStatusL");
	TBool marked = IsMarkedL(uid);
	return SetApplicationStatusL(uid, !marked);
/*	TInt i=GetIndexFromUidL(uid);
	if (i<0)
		return EFalse;

	TInt indexFilt = GuiUtils::GetFilteredIndex(iListBox, i);
	if (indexFilt >= 0)
		{
		// iListBox->View()->ToggleItemL(indexFilt);
		MDesCArray* textArray = iListBox->Model()->ItemTextArray();
		CDesCArray* listBoxItems = static_cast<CDesCArray*>( textArray );
		TBuf<512> listString = textArray->MdcaPoint(i);
		if (i<10)
			listString.Replace(0, 2, _L(""))
		Notes::Debug_InfoL(listString);
//		CreateListBoxItemL( listString, 0, _L("aaa") );
		// update item
		listBoxItems->Delete( i );
		listBoxItems->InsertL( i, listString );
		// select new item
		iListBox->DrawItem( indexFilt );
		}

	TInt markedPos = iMarkedUids.Find( uid );
	if (markedPos >= 0) 
		{
		iMarkedUids.Remove( markedPos );
		}
	else
		{
		iMarkedUids.Append( uid );
		}
	
	return ETrue;*/
	}


TBool CProtectionContainer::IsMarkedL(TInt32 uid)
	{
		return (iMarkedUids.Find( uid ) >= 0);
	}


TBool CProtectionContainer::IsMarkedL()
	{
		TInt32 uid = GetSelectedUidL();
		if (uid == 0)
			return EFalse;
		return IsMarkedL(uid);
		
/*		if (!iListBox)
			return EFalse;
		TInt indexOrig = GetOriginalIndex();
		if (indexOrig < 0)
			return EFalse;
		TUint32 uid = (*iFullAppList)[indexOrig].iUid.iUid;
		return (iMarkedUids.Find( uid ) >= 0);*/
	}


// ---------------------------------------------------------
// CProtectionContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CProtectionContainer::SizeChanged()
	{
//    if (iListBox)
//		iListBox->SizeChanged();
	if (iListBox)
		iListBox->SetRect(Rect());
	if (!iFindBox)
		return;
	if (iStyle == CAknSearchField::EPopup)
		AknFind::HandlePopupFindSizeChanged( this, iListBox, iFindBox );
	else
		AknFind::HandleFixedFindSizeChanged( this, iListBox, iFindBox );
	}

// ---------------------------------------------------------
// CProtectionContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CProtectionContainer::CountComponentControls() const
	{
	TInt count(0);
	if (iListBox) count++;
	if (iFindBox) count++;
    return count;
	}


void CProtectionContainer::CreateListBoxL(TBool large, TBool findBoxVisible)
    {
	// Retrieve the updated marked uids
	iMarkedUids.Reset();
	
	
	// TODO: Questa parte di pulizia era meglio farla nella View... (vedi Guardian)
	RHider hider;
	CleanupClosePushL( hider );
	User::LeaveIfError( hider.Connect() );
	hider.GetProtectedListL( iMarkedUids );

	// Removes old Entries...
	for (TInt i=0; i<iMarkedUids.Count(); i++)
		{
		TInt32 uid = iMarkedUids[i];
		TBool existsApp = (GetIndexFromUidL(uid) >= 0);
		if (!existsApp)
			{
			hider.RemProtectedUid( uid );
			} 
		}
	iMarkedUids.Reset();
	hider.GetProtectedListL( iMarkedUids );
	CleanupStack::PopAndDestroy( &hider );

	DeleteFindBox();

	__LOG_TXT("Delete ListBox");
	if (iListBox)
		delete iListBox; 

	if (large)
		iListBox = new (ELeave) CAknSingleLargeStyleListBox();	 // Icone Grandi
	else
		iListBox = new (ELeave) CAknSingleGraphicStyleListBox(); // Icone Piccole

//	iListBox->ConstructL(this, CEikListBox::ELoopScrolling | EAknListBoxMarkableList);
	iListBox->ConstructL(this, CEikListBox::ELoopScrolling);

	iListBox->SetContainerWindowL(*this);
	iListBox->Model()->SetOwnershipType(ELbmOwnsItemArray);

	CAknIconArray* iconList = new (ELeave) CAknIconArray(10);
	CleanupStack::PushL(iconList);

	CDesCArray* itemList = new (ELeave) CDesCArrayFlat(10);
	CleanupStack::PushL(itemList);
 
	CFbsBitmap*	iMyIcon(NULL);
	CFbsBitmap*	iMyIconMask(NULL);

//	AknIconUtils::CreateIconL(iMyIcon, iMyIconMask, KAvkonBitmapFile, EMbmAvkonQgn_indi_marked_add, EMbmAvkonQgn_indi_marked_add_mask);
	__LOG_TXT("GetMarkIcon");
    TRgb defaultColor;
    defaultColor = CEikonEnv::Static()->Color(EColorControlText);
    AknsUtils::CreateColorIconLC(AknsUtils::SkinInstance(),
    			KAknsIIDQgnIndiMarkedAdd,
    			KAknsIIDQsnIconColors,
    			EAknsCIQsnIconColorsCG13,
    			iMyIcon,
    			iMyIconMask,
    			KAvkonBitmapFile,
    			EMbmAvkonQgn_indi_marked_add,
    			EMbmAvkonQgn_indi_marked_add_mask,
    			defaultColor
    			);
    CGulIcon* markIcon = CGulIcon::NewL( iMyIcon, iMyIconMask );
	CleanupStack::Pop(2);	// iMyIcon, iMyIconMask
	CleanupStack::PushL(markIcon);
	iconList->AppendL(markIcon);
	CleanupStack::Pop(markIcon);	// markIcon

	__LOG_TXT("GetAppIconL loop");
	TInt i=0;
		while (i < iFullAppList->Count())
			{
			TApaAppInfo appInfo = (*iFullAppList)[i];
			// __FLOG(appInfo.iFullName);
			// TODO: Confronta la caption e la dimensione dell'icona con quelle che sono gia' state inserite e skippale se sono uguali...
			// TODO: Aggiungi solo quelle che non sono gia' state inserite
			CGulIcon* icon = NULL;
			TRAPD(err, icon = GuiUtils::GetApplicationIconL(appInfo.iUid));
			if (err != KErrNone)
			{
				const TUid KServerIcon = { 0x2005E763 };
				TRAPD(err, icon = GuiUtils::GetApplicationIconL( KServerIcon ));
			}
			CleanupStack::PushL(icon);
			iconList->AppendL( icon );
			CleanupStack::Pop(icon);
			// __LOG_TXT("GetItemString"));
			TBool marked = IsMarkedL(appInfo.iUid.iUid);
			
			if (marked)
				AddToItemListL(*itemList, appInfo.iShortCaption, 0);
			else
				AddToItemListL(*itemList, appInfo.iShortCaption, i+1);
			//itemList->AppendL( GetItemStringLC(appInfo.iCaption, i+1)->Des() );
			//CleanupStack::PopAndDestroy();
			i++;
			}/**/

	__LOG_TXT("SetItemTextArrayL");
	iListBox->Model()->SetItemTextArray( itemList );
	CleanupStack::Pop( itemList ); // itemList

	iListBox->ItemDrawer()->ColumnData()->SetIconArray( iconList );
	CleanupStack::Pop( iconList ); // iconList

	// Crea la scrollbar
	CEikScrollBarFrame* scrollBar = iListBox->CreateScrollBarFrameL(ETrue);
	scrollBar->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

	iListBox->SetRect( Rect() );

	// Se necessario crea FindBox
	if (findBoxVisible)
		iFindBox = GuiUtils::CreateSearchBoxL(*this, *iListBox, iStyle);
 
	iListBox->SetListBoxObserver(this);
	
	SizeChanged();

	ActivateL();	// Necessario! Altrimenti si verificano problemi di refresh quando si passa da icone piccole a grandi...	
    }


// ---------------------------------------------------------
// CProtectionContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CProtectionContainer::ComponentControl(TInt aIndex) const
	{
	 switch ( aIndex )
		{
		case 0:
			return iListBox;
		case 1:
			return iFindBox;
		default:
			return NULL;
		}
	}

// ---------------------------------------------------------
// CProtectionContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CProtectionContainer::Draw(const TRect& aRect) const
	{
	}


void CProtectionContainer::DeleteFindBox()
{
	if (iFindBox && iListBox)
	{
		CAknFilteredTextListBoxModel* model = static_cast<CAknFilteredTextListBoxModel*>(iListBox->Model());
		model->RemoveFilter();
		delete iFindBox;
		iFindBox = NULL;
	}
}


/*
void CProtectionContainer::GetMarkedUids(RArray<TUint32>& uidList)
	{
	for (TInt i=0; i<iMarkedUids.Count(); i++)
		{
		TUint32 uid = iMarkedUids[i];
		uidList.Append( uid );
		}
	}
*/

TKeyResponse CProtectionContainer::OfferKeyToFindBoxL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
// Deseleziona tutti gli elementi che sono marcati
// Non e' possibile xche' la listbox ciuccia tutti gli eventi di keypress...
/*			CAknFilteredTextListBoxModel* model = STATIC_CAST(CAknFilteredTextListBoxModel*,iListBox->Model());
			TInt tot = model->Filter()->FilteredNumberOfItems();
			for (int i=0; i<tot; i++)
			{
				iListBox->View()->DeselectItem( i );
			}*/
/*			for (int i=0; i<iMarkers.Count(); i++)
			{
				TInt oriIndex = iMarkers[i];
				TInt filtIndex = GetFilteredIndexL(oriIndex);
				if (filtIndex >= 0)
					iListBox->View()->DeselectItem( filtIndex );
			}*/

            TBool needRefresh( EFalse );
            // Offers the key event to find box.
			TBool popup = (iStyle == CAknSearchField::EPopup);
			TKeyResponse ris = AknFind::HandleFindOfferKeyEventL(
  			   aKeyEvent, aType, this, iListBox, iFindBox, popup, needRefresh );

            if ( needRefresh )
                {
                SizeChanged();
                DrawNow();
                }
            if ( ris == EKeyWasConsumed )
                {
				__LOG_TXT("FindBox Key");
                } else
				{
				__LOG_TXT("FindBox NOKey");
				}

			__LOG("TxtLength:%d", iFindBox->TextLength());
//			iFindBox->GetSearchText(txt);
//			__FLOG(txt);

/*			// Riseleziona tutti gli elementi che erano marcati
			for (int i=0; i<iMarkers.Count(); i++)
			{
				TInt oriIndex = iMarkers[i];
				TInt filtIndex = GetFilteredIndexL(oriIndex);
				if (filtIndex >= 0)
					iListBox->View()->SelectItemL( filtIndex );
			}*/
			return ris;
}


TKeyResponse CProtectionContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
	{
    if ( aType != EEventKey )
        return EKeyWasNotConsumed;

	if (!iListBox)
        return EKeyWasNotConsumed;

    switch ( aKeyEvent.iCode )
		{
        // Left Arrow pressed
		case EKeyBackspace:
		{
			if (!iFindBox)
				return EKeyWasNotConsumed;
			if (iFindBox->TextLength() == 0)
			{
				DeleteFindBox();
				SizeChanged();
				DrawNow();
				return EKeyWasConsumed;
			}
			return OfferKeyToFindBoxL(aKeyEvent, aType);
		}
		case EKeyLeftArrow:
		case EKeyRightArrow:
			return EKeyWasNotConsumed;
		case EKeyUpArrow:
		case EKeyDownArrow:
			return iListBox->OfferKeyEventL( aKeyEvent, aType );
		case EKeyDevice3:	// Da gestire in maniera differente perche' di default viene mostrato il menu' delle opzioni
			{
			__LOG_TXT("EKeyDevice3");
/*			if (iTouchSupported && !iAcceptTouchEvents)
				{
				__LOG_TXT("Touch event already dispatched"); 
				return EKeyWasConsumed;				
				}*/
/*			if (iCommandObs)
				iCommandObs->ProcessCommandL( EAddApp );*/
			ChangeApplicationStatusL( GetSelectedUidL() );
			TBool checked = IsMarkedL();
			if (iObserver) 
				iObserver->HandleItemChangeEventL( GetSelectedUidL() , checked);/**/
			return EKeyWasConsumed;
			}
		default:
            {
        	return EKeyWasNotConsumed;
			if (!iFindBox)
			{
			    iFindBox = GuiUtils::CreateSearchBoxL(*this, *iListBox, iStyle);
                SizeChanged();
				iFindBox->SetFocus(ETrue, EDrawNow);
				return EKeyWasNotConsumed;
				// Non e' possibile xche' viene inserito un numero invece di una lettera...
//				return OfferKeyEventL(aKeyEvent, aType);
			}
			return OfferKeyToFindBoxL(aKeyEvent, aType);
			}
		}
	return EKeyWasNotConsumed;
	}


CArrayFixFlat<TApaAppInfo>* CProtectionContainer::GetApplicationsLC()
{
	CArrayFixFlat<TApaAppInfo>* MyArray = new(ELeave)CArrayFixFlat<TApaAppInfo>(10);
	CleanupStack::PushL(MyArray);

	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls);

	User::LeaveIfError(ls.GetAllApps());

	TApaAppInfo AppInfo;
	TApaAppCapabilityBuf appCapBuf;
	
	

/*	TUid uid = TUid::Uid(0x20029C47);
	CDesCArrayFlat* filelist = new (ELeave) CDesCArrayFlat( 0xFF );
	CleanupStack::PushL(filelist);	
	ls.GetAppOwnedFiles(*filelist, uid );
	for (int i=0; i<filelist->Count(); i++)
		{
		TAny* ptr = filelist->At(i);
		HBufC* buf = (HBufC*)(ptr);
		}
	CleanupStack::PopAndDestroy(filelist);/**/
	
	// CApaAppInfoFileReader

//	_LIT(KFAKEAPP, ".fakeapp");
	while (ls.GetNextApp(AppInfo) == KErrNone)
	{	
		AppInfo.iCaption.Trim();
		AppInfo.iShortCaption.Trim();
		
		if (AppInfo.iShortCaption.Length() <= 0)
			AppInfo.iShortCaption = AppInfo.iCaption;
		
		if (AppInfo.iShortCaption.Length() <= 0)
			continue;	// Deve avere un nome...

		// Controlla se e' registrato oppure no... No, perche' anche nella TRIAL si puo' proteggere IconHider
		//if (AppInfo.iUid.iUid == 0x20024264)
		//	continue;	// Icon Hider
		
		ls.GetAppCapability(appCapBuf, AppInfo.iUid);
		
		if (appCapBuf().iAppIsHidden)				// Ne ottiene 53 not hidden...
			continue;
		
/*		if (AppInfo.iUid.iUid == 0x200131CC ||
			AppInfo.iUid.iUid == 0x200131C7 )
			continue;	// Guardian */

//		if (AppInfo.iFullName.Right(8).MatchC(KFAKEAPP) == 0)
//			continue;	// Applicazione java
		
/*		if (AppInfo.iUid.iUid == 0x20029C47)
			{
			TPtrC shortC = AppInfo.iShortCaption;
			TPtrC longC = AppInfo.iCaption;
			TPtrC name = AppInfo.iFullName;
			}*/
		MyArray->AppendL(AppInfo);
	}

	CleanupStack::PopAndDestroy(&ls); // ls

//	Notes::Debug_InfoL(_L("Count:%d"), MyArray->Count());
	
	// Ordina array
	TKeyArrayFix captionKey(_FOFF(TApaAppInfo, iShortCaption), ECmpFolded);
	MyArray->Sort(captionKey);
	return MyArray;
}


void CProtectionContainer::HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType)
{
}
