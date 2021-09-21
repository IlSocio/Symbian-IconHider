/*
============================================================================
 Name        : CSettingsListboxContainer from SettingsListboxContainer.cpp
 Author      : Marco Bellino
 Version     :
 Copyright   : All Rights Reserved
 Description : Container control implementation
============================================================================
*/
#include "SettingsListboxContainer.h"
#include "Settings.hrh"
#include "SettingsItemList.h"
#include "main.hrh"

#include  <IconHider_20024264.rsg>

#include <FuzzyByte\IconHider\Client.H>
#include <FuzzyByte\GuiUtils.H>
#include <FuzzyByte\Notes.H>
#include <FuzzyByte\FileUtils.H>

#include <AKNSETTINGITEMLIST.H>
#include <AKNiconarray.H>
#include <eiklbi.H>
#include <EIKTXLBX.H>
#include <stringloader.H>
#include <eiklabel.h>  // for example label control
#include <AknLists.h>
#include <barsread.h>
#include <CAknMemorySelectionSettingItem.h>
#include <AKNVIEWAPPUI.H>
#include <aknview.H>


// ================= MEMBER FUNCTIONS =======================


CSettingsListboxContainer* CSettingsListboxContainer::NewL(const TRect& aRect, const CCoeControl* aParent) 
	{
	CSettingsListboxContainer* self = new ( ELeave ) CSettingsListboxContainer();
	CleanupStack::PushL(self);
	self->ConstructL(aRect, aParent);
	CleanupStack::Pop();
	return self;
	}

/*
    CAknBigSettingItemBase 
    CAknEnumeratedTextSettingItem 	<- CAknEnumeratedTextPopupSettingItem	<- CAknBinaryPopupSettingItem
    CAknIntegerSettingItem 			<- CAknIntegerEdwinSettingItem
    CAknIpFieldSettingItem 
    CAknMemorySelectionSettingItem 
    CAknSliderSettingItem
    CAknTextSettingItem 			<- CAknPasswordSettingItem
    CAknTimeOffsetSettingItem
    CAknTimeOrDateSettingItem 
    CAknVolumeSettingItem 
 */

#include <IconHiderImg_20024264.mbg> // Contiene i numeri delle icone di IconHider.mbm


_LIT(KListItemFormat, "%d\t%S\t%S\t");

// ---------------------------------------------------------
// CSettingsListboxContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CSettingsListboxContainer::ConstructL(const TRect& aRect, const CCoeControl* aParent)
	{
	const CCoeControl* ctrl = this;
	if ( aParent == NULL )
	    {
		CreateWindowL();
	    }
	else
	    { 
	    ctrl = aParent;
	    SetContainerWindowL( *aParent );
	    }
	
	CreateSettingItemsL();
    
	iListBox = new (ELeave) CAknDoubleLargeStyleListBox();
//	iListBox = new (ELeave) CAknDoubleGraphicStyleListBox();
//	iListBox = new (ELeave) CAknSingleLargeStyleListBox();  
	iListBox->ConstructL(this, EAknListBoxSelectionList);
	
/*	CAppEngine* engine = CAppEngine::InstanceL();
	TUint32 enab = engine->iData->enabled;*/
	
	// construct listbox item array
	// TODO: Construct Listbox using values...
	CDesCArray *itemList = new (ELeave) CDesCArrayFlat(5);
/*	TBuf<50> item;
	item.Format(KListItemFormat, 0, &_L("Hide Icons"), &_L("Always"));	// Never / Automatic / On Sim Change / Always
	itemList->AppendL(item);
	item.Format(KListItemFormat, 1, &_L("Demo Version"), &_L("Buy Full"));
	itemList->AppendL(item);
	item.Format(KListItemFormat, 2, &_L("Register"), &_L(""));
	itemList->AppendL(item);
/*	item.Format(KListItemFormat, 3, &_L("Send SISX"), &_L("Bluetooth or Infrared"));
	itemList->AppendL(item);
	item.Format(KListItemFormat, 4, &_L("Check Updates"), &_L(""));
	itemList->AppendL(item);*/
	
	CTextListBoxModel* model = iListBox->Model();
	model->SetItemTextArray(itemList);
	model->SetOwnershipType(ELbmOwnsItemArray);
	
	// construct icon array with granularity 3
	CArrayPtr<CGulIcon>* iconList = new (ELeave) CAknIconArray(5);
	CleanupStack::PushL(iconList);
#ifdef __WINSCW__
		TFileName filePath = _L("z:\\resource\\apps\\IconHiderImg_20024264.mif");
#else
		TFileName filePath = _L("\\resource\\apps\\IconHiderImg_20024264.mif");
		FileUtils::CompleteWithCurrentDrive(filePath);
#endif
	CGulIcon* icon1 = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(), KAknsIIDQsnBgColumn0, filePath, EMbmIconhiderimg_20024264Enable, EMbmIconhiderimg_20024264Enable_mask );
	iconList->AppendL( icon1 ); 
	icon1 = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(), KAknsIIDQsnBgColumn0, filePath, EMbmIconhiderimg_20024264Disable, EMbmIconhiderimg_20024264Disable_mask );
	iconList->AppendL( icon1 );
	icon1 = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(), KAknsIIDQsnBgColumn0, filePath, EMbmIconhiderimg_20024264Code, EMbmIconhiderimg_20024264Code_mask );
	iconList->AppendL( icon1 );
	icon1 = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(), KAknsIIDQsnBgColumn0, filePath, EMbmIconhiderimg_20024264About, EMbmIconhiderimg_20024264About_mask );
	iconList->AppendL( icon1 );
	icon1 = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(), KAknsIIDQsnBgColumn0, filePath, EMbmIconhiderimg_20024264Register, EMbmIconhiderimg_20024264Register_mask );
	iconList->AppendL( icon1 );
	icon1 = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(), KAknsIIDQsnBgColumn0, filePath, EMbmIconhiderimg_20024264Update, EMbmIconhiderimg_20024264Update_mask );
	iconList->AppendL( icon1 );
	icon1 = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(), KAknsIIDQsnBgColumn0, filePath, EMbmIconhiderimg_20024264Buy, EMbmIconhiderimg_20024264Buy_mask );
	iconList->AppendL( icon1 );
	
	CEikFormattedCellListBox*  fmtList = (CEikFormattedCellListBox *) iListBox;
	fmtList->ItemDrawer()->ColumnData()->SetIconArray( iconList );
	CleanupStack::Pop();
	
	// Crea la scrollbar|
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
	iListBox->SetContainerWindowL(*this);
	iListBox->SetListBoxObserver(this);
	iListBox->SetCurrentItemIndex(0);

	iListBox->HandleItemAdditionL();
	iListBox->UpdateScrollBarsL();
	iListBox->SetFocus(ETrue);/**/

	SetRect(aRect);
	ActivateL();
	}


void CSettingsListboxContainer::CreateSettingItemsL()
	{
/*	CAknSettingItem* settingItem = new( ELeave ) CAknEnumeratedTextPopupSettingItem( EStatus, (TInt&)iData->status);
	CleanupStack::PushL( settingItem );
	settingItem->ConstructL(EFalse, EStatus, _L("EnumItem"), NULL, R_SETTINGS_STATUS_SETPAGE, EAknCtPopupSettingList, 0, R_SETTING_STATUS_POPUP_TEXTS);
	iSettingItemArray.AppendL( settingItem );
	CleanupStack::Pop( settingItem );*/
/*
	CAknMemorySelectionDialog::TMemory memory = CAknMemorySelectionSettingPage::EPhoneMemory;
	CAknSettingItem* settingItem = new( ELeave ) CAknMemorySelectionSettingItem( 0, memory );
	CleanupStack::PushL( settingItem );
	settingItem->ConstructL( EFalse, 0, _L("ItemTitle"), NULL, 0, EAknCtPopupSettingList );
	iSettingItemArray.AppendL( settingItem );
	CleanupStack::Pop( settingItem );
	
	CAknMemorySelectionDialog::TMemory memory2 = CAknMemorySelectionSettingPage::EPhoneMemory;
	CAknSettingItem* settingItem2 = new( ELeave ) CAknMemorySelectionSettingItem( 11, memory2 );
	CleanupStack::PushL( settingItem2 );
	settingItem2->ConstructL( EFalse, 11, _L("ItemTitle2"), NULL, 0, EAknCtPopupSettingList );
	iSettingItemArray.AppendL( settingItem2 );
	CleanupStack::Pop( settingItem2 );
*/
	
//    iSettingItemArray.RecalculateVisibleIndicesL();
	}


void CSettingsListboxContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
	{
    // Remember to call base class implementation
    CCoeControl::HandlePointerEventL(aPointerEvent);
    
    //if (iListBox)
    //	iListBox->HandlePointerEventL(aPointerEvent);
    // Your additional code here
    // ...	
	}


void CSettingsListboxContainer::HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType)
	{
	TInt index = aListBox->CurrentItemIndex();
	CAknView* settView = iAvkonViewAppUi->View( TUid::Uid(ESettingsViewId) );
	
	switch (aEventType)
		{
		case EEventEnterKeyPressed:		// Non Touch
		case EEventItemClicked:			// Touch 
//		case EEventItemDoubleClicked:	// Touch 
			switch (index)
				{
				case 0:
					settView->HandleCommandL(EChangeStatus);
					break;
				case 1:
					settView->HandleCommandL(EChangeCode);
					break;   
				case 2:
					settView->HandleCommandL(EProtectionView);
					break;   
				case 3: 
					if (iRegistered)
						{
						settView->HandleCommandL(ECheckUpdates);
						}
					else
						{
						settView->HandleCommandL(EBuyFull);
						}
					break;
				case 4:
					if (iRegistered)
						{
						settView->HandleCommandL(EAbout);
						}
					else
						{
						settView->HandleCommandL(ERegister);
						}
					break;
				case 5:
					{
					settView->HandleCommandL(ECheckUpdates);
					break;
					}
				case 6:
					{
					settView->HandleCommandL(EAbout);
					break;
					}
				}			
			break;
		default:
			break;
		}	
	}



void CSettingsListboxContainer::AddListBoxItemL(TInt aResourceId1, const TDesC& aText, TInt aIconIndex)
	{
	HBufC* buf1 = StringLoader::LoadLC( aResourceId1 );
	
	TBuf<512> listString; 
//	_LIT ( KStringHeader, "%d\t%S" );
//	listString.Format( KStringHeader(), aIconIndex, &(*array)[0] );
//	listString.Format(KListItemFormat, aIconIndex, &_L("Hide Icons"), &_L("Always"));	// Never / Automatic / On Sim Change / Always
	TPtrC ptr1 = buf1->Des();
	listString.Format(KListItemFormat, aIconIndex, &ptr1, &aText);	// Never / Automatic / On Sim Change / Always

	GuiUtils::ListBoxAddItemL( iListBox, listString );
	CleanupStack::PopAndDestroy( buf1 );	
	}


void CSettingsListboxContainer::AddListBoxItemL(TInt aResourceId1, TInt aResourceId2, TInt aIconIndex)
	{
	HBufC* buf2 = StringLoader::LoadLC( aResourceId2 );
	AddListBoxItemL(aResourceId1, *buf2, aIconIndex);
	CleanupStack::PopAndDestroy( buf2 );
	}


/*
void CSettingsListboxContainer::AddCurrentProfileL(TInt aIconIndex)
	{
	// Current Profile:
	HBufC* bufProf = StringLoader::LoadLC( R_CURRENT_PROFILE );
	TBuf<512> listString;
	TPtrC ptr1 = bufProf->Des();
	listString.Format(KListItemFormat, aIconIndex, &ptr1, &iData->currProfileName);	// 3	1
	GuiUtils::ListBoxAddItemL( iListBox, listString );
	CleanupStack::PopAndDestroy( bufProf );	
	}
*/

void CSettingsListboxContainer::UpdateListBoxViewL(TBool enabled, TBool full)
	{
	iRegistered = full;
	CTextListBoxModel* model = iListBox->Model();
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	
	itemArray->Reset();
	
	if (enabled)
		AddListBoxItemL( R_STATUS, R_ACTIVE, 0 );
	else
		AddListBoxItemL( R_STATUS, R_INACTIVE, 1 );

	TPassCode pass;
#ifndef _TEST_	
	RHider hider;
	CleanupClosePushL( hider );
	User::LeaveIfError( hider.Connect() );
	hider.GetPassword(pass);	
	pass.Insert(0, _L("*"));
	pass.Append(_L("*"));
	CleanupStack::PopAndDestroy( &hider ); 
#endif	 
	
	AddListBoxItemL( R_CODE, pass, 2 );
	
	AddListBoxItemL( R_HIDDEN_APPS, KNullDesC(), 2 );
	
	//   
	
	if (!full)
		{
		AddListBoxItemL( R_DEMO, R_BUY_FULL, 6 );
		AddListBoxItemL( R_REGISTER, KNullDesC(), 4 );
		} 

	AddListBoxItemL( R_CHECK_UPDATES, R_EMPTY, 5 );
	AddListBoxItemL( R_ABOUT, R_ABOUT_2ND, 3 ); 
	
//	if (iHasRedistFile)
//		AddListBoxItemL( R_SEND, R_EMPTY, 0 );
	}

// Destructor
CSettingsListboxContainer::~CSettingsListboxContainer()
	{
	if (iListBox)
		delete iListBox;
	iSettingItemArray.ResetAndDestroy();
	}

// ---------------------------------------------------------
// CSettingsListboxContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CSettingsListboxContainer::SizeChanged()
	{
    if (iListBox)
		iListBox->SetExtent(TPoint(0,0), iListBox->MinimumSize());
	}

// ---------------------------------------------------------
// CSettingsListboxContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CSettingsListboxContainer::CountComponentControls() const
	{
	TInt count(0);
	if (iListBox) 
		count++;
    return count;
	}

// ---------------------------------------------------------
// CSettingsListboxContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CSettingsListboxContainer::ComponentControl(TInt aIndex) const
	{
	 switch ( aIndex )
		{
		case 0:
			return iListBox;
		default:
			return NULL;
		}
	}


TKeyResponse CSettingsListboxContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
    if ( aType != EEventKey )
        return EKeyWasNotConsumed;

	//
	// Pass any key event to the listbox
	// (this is required if you want to move the cursor through the list)
	//
	if (!iListBox)
		return EKeyWasNotConsumed;

    switch ( aKeyEvent.iCode )
		{
        // Left Arrow pressed
		case EKeyUpArrow:
		case EKeyDownArrow:
		case EKeyDevice3:
			return iListBox->OfferKeyEventL( aKeyEvent, aType );
		break;
		}
	return EKeyWasNotConsumed;
	}

// ---------------------------------------------------------
// CSettingsListboxContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CSettingsListboxContainer::Draw(const TRect& aRect) const
	{
	CWindowGc& gc = SystemGc();
	gc.SetPenStyle( CGraphicsContext::ENullPen );
	gc.SetBrushColor( KRgbGray );
	gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
	gc.DrawRect( aRect );
	}

// ---------------------------------------------------------
// CSettingsListboxContainer::HandleControlEventL(
//    CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CSettingsListboxContainer::HandleControlEventL(
	CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
	{
	}
