/*
============================================================================
 Name        : CSettingsContainer from SettingsContainer.cpp
 Author      : Marco Bellino
 Version     :
 Copyright   : All Rights Reserved
 Description : Container control implementation
============================================================================
*/
//#include <GuardGUI_200131CC.rsg>
#include  <IconHider_20024264.rsg>
#include <AKNSETTINGITEMLIST.H>
// INCLUDE FILES
#include "SettingsContainer.h"

#include "Settings.hrh"
#include <eiklabel.h>  // for example label control
#include <AknLists.h>
#include <barsread.h>
#include <CAknMemorySelectionSettingItem.h>
#include "SettingsItemList.h"

_LIT(KTAB, "\t");

// ================= MEMBER FUNCTIONS =======================


CSettingsContainer* CSettingsContainer::NewL(const TRect& aRect, MEikListBoxObserver& observer, const CCoeControl* aParent) 
	{
	CSettingsContainer* self = new ( ELeave ) CSettingsContainer();
	CleanupStack::PushL(self);
	self->ConstructL(aRect, observer, aParent);
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

// ---------------------------------------------------------
// CSettingsContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CSettingsContainer::ConstructL(const TRect& aRect, MEikListBoxObserver& observer, const CCoeControl* aParent)
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
    iSettingItemArray.RecalculateVisibleIndicesL();
    
    iItemList = CSettingsItemList::NewL( iData, aRect );
/*	iListBox = new (ELeave) CAknSettingStyleListBox();
	iListBox->ConstructL(this, EAknListBoxSelectionList);
	
	CTextListBoxModel* model = iListBox->Model();
	model->SetItemTextArray( &iSettingItemArray );
	model->SetOwnershipType(ELbmDoesNotOwnItemArray );
	
	// Crea la scrollbar|
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
	iListBox->SetContainerWindowL(*this);
	iListBox->SetListBoxObserver(&observer);
	
	iListBox->HandleItemAdditionL();
	iListBox->UpdateScrollBarsL();
	iListBox->SetFocus(ETrue);*/

	SetRect(aRect);
	ActivateL();

//	UpdateListBoxViewL(EFalse, _L(""), _L(""), _L(""), _L(""));
	}


void CSettingsContainer::CreateSettingItemsL()
	{
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
	}


void CSettingsContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
	{
    // Remember to call base class implementation
    CCoeControl::HandlePointerEventL(aPointerEvent);
    
    //if (iListBox)
    //	iListBox->HandlePointerEventL(aPointerEvent);
    // Your additional code here
    // ...	
	}


HBufC* CSettingsContainer::GetItemStringLC(TInt resourceID, const TDesC& value)
	{
	// iCoeEnv->ReadResourceL();
	HBufC* string = CCoeEnv::Static()->AllocReadResourceLC(resourceID);

/*	Stranamente non funzia
    E' colpa di ReAllocL() !!!! Non aumenta la dimensione...
	string->ReAllocL(string->Length() + value.Length());
	CleanupStack::Pop();
	CleanupStack::PushL(string);
	string->Des().Append(value);
	return string;*/

	// Non si e' utilizzato realloc xche' il cleanupstack avrebbe puntato ancora
	// alla vecchia locazione della stringa
	HBufC* ris = HBufC::NewL(string->Length() + value.Length() + 6);
	TPtr ptr = ris->Des();
	ptr.Append(KTAB);
	ptr.Append( string->Des() );
	ptr.Append(KTAB);
	ptr.Append(KTAB);
	ptr.Append( value );
	CleanupStack::PopAndDestroy(string);
	CleanupStack::PushL(ris);
	return ris;/**/
	}


void CSettingsContainer::UpdateListBoxViewL(TInt isEnabled, TBool isRegistered )
	{
/*	CDesCArray* itemList = new (ELeave) CDesCArrayFlat(1);
	TBuf<20> enab;

	enab = _L("No");
	if (isEnabled)
	{
		iCoeEnv->ReadResourceL(enab, R_YES );
	} else 
	{
		iCoeEnv->ReadResourceL(enab, R_NO );
	}
	
	TBuf<20> number;
	TBuf<20> password;
	TBuf<200> message;
	iGuardian.GetDestAddress(number);
	iGuardian.GetMessage(message);
	iGuardian.GetPassword(password);

	itemList->AppendL( GetItemStringLC( R_STATUS, enab )->Des() );	CleanupStack::PopAndDestroy();
//	itemList->AppendL( GetItemStringLC( R_STATUS, enab )->Des() );	CleanupStack::PopAndDestroy();
	/*itemList->AppendL( GetItemStringLC( R_PHONE_NUMBER, number )->Des() ); CleanupStack::PopAndDestroy();
	itemList->AppendL( GetItemStringLC( R_TEXT_SMS, message )->Des() );	CleanupStack::PopAndDestroy();

	TBuf<8> pass;
	for (TInt i=0; i<password.Length(); i++)
	{
		pass.Append('*');
	}
	itemList->AppendL( GetItemStringLC( R_PASSWORD, pass )->Des() );	CleanupStack::PopAndDestroy();
	itemList->AppendL( GetItemStringLC( R_PROTECT_APP, delay )->Des() );	CleanupStack::PopAndDestroy();
	iListBox->Model()->SetItemTextArray(itemList);

	iListBox->HandleItemAdditionL();
//    iListBox->ActivateL();
    iListBox->DrawNow();*/
	}

// Destructor
CSettingsContainer::~CSettingsContainer()
	{
	if (iListBox)
		delete iListBox;
	if (iItemList)
		delete iItemList;
	iSettingItemArray.ResetAndDestroy();
	}

// ---------------------------------------------------------
// CSettingsContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CSettingsContainer::SizeChanged()
	{
    if (iListBox)
		iListBox->SetExtent(TPoint(0,0), iListBox->MinimumSize());
	}

// ---------------------------------------------------------
// CSettingsContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CSettingsContainer::CountComponentControls() const
	{
	TInt count(0);
	if (iListBox) 
		count++;
	if (iItemList) 
		count++;
    return count;
	}

// ---------------------------------------------------------
// CSettingsContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CSettingsContainer::ComponentControl(TInt aIndex) const
	{
	return iItemList;
	 switch ( aIndex )
		{
		case 0:
			return iListBox;
		case 1:
			return iItemList;
		default:
			return NULL;
		}
	}

	TKeyResponse CSettingsContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
    if ( aType != EEventKey )
        return EKeyWasNotConsumed;

	//
	// Pass any key event to the listbox
	// (this is required if you want to move the cursor through the list)
	//
	if (!iListBox && !iItemList)
		return EKeyWasNotConsumed;

    switch ( aKeyEvent.iCode )
		{
        // Left Arrow pressed
		case EKeyUpArrow:
		case EKeyDownArrow:
		case EKeyDevice3:
			return iItemList->OfferKeyEventL( aKeyEvent, aType );
			return iListBox->OfferKeyEventL( aKeyEvent, aType );
		break;
		}
	return EKeyWasNotConsumed;
	}

// ---------------------------------------------------------
// CSettingsContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CSettingsContainer::Draw(const TRect& aRect) const
	{
	CWindowGc& gc = SystemGc();
	gc.SetPenStyle( CGraphicsContext::ENullPen );
	gc.SetBrushColor( KRgbGray );
	gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
	gc.DrawRect( aRect );
	}

// ---------------------------------------------------------
// CSettingsContainer::HandleControlEventL(
//    CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CSettingsContainer::HandleControlEventL(
	CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
	{
	}
