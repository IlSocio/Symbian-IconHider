/*  Copyright (c) 2004, Nokia. All rights reserved */

// INCLUDE FILES
#include <aknviewappui.h>
#include "SettingsItemList.h"
#include "Settings.hrh"
#include  <IconHider_20024264.rsg>

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CSettingsItemList::NewL()
// Two phase constructor.
// ----------------------------------------------------------------------------
//
CSettingsItemList* CSettingsItemList::NewL(TSettingsData1& aData, const TRect& aRect, const CCoeControl* aParent) 
	{
	CSettingsItemList* self = new ( ELeave ) CSettingsItemList(aData);
	CleanupStack::PushL(self);
	self->ConstructL(aRect, aParent);
	CleanupStack::Pop();
	return self;
	}

// ----------------------------------------------------------------------------
// CSettingsItemList::~CSettingsItemList()
// Deststructor.
// ----------------------------------------------------------------------------
//
CSettingsItemList::~CSettingsItemList()
	{
	}

// ----------------------------------------------------------------------------
// CSettingsItemList::CSettingsItemList()
// Default constructor
// ----------------------------------------------------------------------------
//
CSettingsItemList::CSettingsItemList(TSettingsData1& aData) : CFuzSettingItemList(), iData(&aData)
	{
	}

// ----------------------------------------------------------------------------
// CSettingsItemList::ConstructL()
// Second phase constructor.
// ----------------------------------------------------------------------------
//
void CSettingsItemList::ConstructL(const TRect& aRect, const CCoeControl* aParent)
	{
	ConstructFromResourceL( R_SETTINGS_ITEM_LIST );
	if ( aParent == NULL )
		{
		// Window-Owning Control
		CreateWindowL();
		}
	else
		{
		// Non-Window-Owning Control
		SetContainerWindowL( *aParent );
		}
	
	// TODO: Create compound controls
	
	SetRect( aRect );
	ActivateL();	// Not needed if it is a component inside a compound control
	
	//	LoadSettingValuesL();
	//	LoadSettingsL();
	}


// ----------------------------------------------------------------------------
// CSettingsItemList::CreateSettingItemL()
// Create items of the list.
// Called when create setting page.
// ----------------------------------------------------------------------------
//
void CSettingsItemList::EditItemL(TInt aIndex, TBool aCalledFromMenu)
	{
	CAknSettingItem* item = SettingItemArray()->At(aIndex);
	//TInt idx = SettingItemArray()->ItemIndexFromVisibleIndex(aIndex);
	TInt itemId = item->Identifier();
	switch (itemId)
		{		
		}
	
	
/*	if (itemId == ESkebbyItemPrivacy)
		{
		LaunchBrowserL(iPrivacyURL);
		return;
		}
	if (itemId == ESkebbyItemTerms)
		{
		LaunchBrowserL(iTermsURL);
		return;
		}
/*	if (itemId == ESkebbyItemOkPrivacy)
		{
		iData->acceptPrivacy = !iData->acceptPrivacy;
		// TODO: Change Value...
		iAcceptPrivacyValue = _L("No");
		if (iData->acceptPrivacy)
			iAcceptPrivacyValue = _L("Yes");
		SettingItemArray()->At(aIndex)->LoadL();
		SettingItemArray()->At(aIndex)->UpdateListBoxTextL();
		return;
		}
/*	if (itemId == ESkebbyItemOkTerms)
		{
		iData->acceptTerms = !iData->acceptTerms;
		iAcceptTermsValue = _L("No");
		if (iData->acceptTerms)
			iAcceptTermsValue = _L("Yes");
		SettingItemArray()->At(aIndex)->LoadL();
		SettingItemArray()->At(aIndex)->UpdateListBoxTextL();
		return;
		}*/
	
	CAknSettingItemList::EditItemL(aIndex, aCalledFromMenu);
	
	switch (itemId)
		{		
		}
	
	item->StoreL();
//	SaveSettingValuesL();
	
/*	if (iData->iDescriptionReadOnly&&(*(SettingItemArray()))[aIndex]->Identifier()==ESkebbyItemWizardreg71)
		return;
	CAknSettingItemList::EditItemL(aIndex, aCalledFromMenu);
	(*(SettingItemArray()))[aIndex]->UpdateListBoxTextL();
	StoreSettingsL();
	if ((*(SettingItemArray()))[aIndex]->Identifier()==ESkebbyItemWizardreg74)
		{
		// Marco Bellino: Bug-FIX 31/32 Start
		TBool showDateCtrl = (iData->iValueWizardreg74 >=4 );
		ItemMakeVisible(ESkebbyItemWizardreg75, showDateCtrl);
		}*/
	}

CAknSettingItem* CSettingsItemList::CreateSettingItemL(TInt aSettingId)
	{
	switch (aSettingId)
		{
        case EStatus:
            return new (ELeave) CAknEnumeratedTextPopupSettingItem(aSettingId, iData->status);
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
	
/*	if (aSettingId == ESkebbyItemUser)
		{
		return new ( ELeave ) CAknTextSettingItem(
				aSettingId, iData->username );
		}
	if (aSettingId == ESkebbyItemNumber)
		{
//		CAknTextSettingItem* numb = new ( ELeave ) CAknTextSettingItem(aSettingId, iData->number );
//		numb->SetSettingPageFlags() CAknTextSettingPage::EZeroLengthAllowed
		return new ( ELeave ) CAknTextSettingItem(aSettingId, iData->number );

		}
	if (aSettingId == ESkebbyItemEmail)
		{
		return new ( ELeave ) CAknTextSettingItem(
				aSettingId, iData->email );
		}
	if (aSettingId == ESkebbyItemTerms)
		{
		return new ( ELeave ) CAknTextSettingItem(
				aSettingId, iTermsURL );
		}
	if (aSettingId == ESkebbyItemOkTerms)
		{
		return new ( ELeave ) CAknBinaryPopupSettingItem( aSettingId, iData->acceptTerms );
		}
	if (aSettingId == ESkebbyItemPrivacy)
		{
		return  new ( ELeave ) CAknTextSettingItem(
				aSettingId, iPrivacyURL );
		}
	if (aSettingId == ESkebbyItemOkPrivacy)
		{
		return new ( ELeave ) CAknBinaryPopupSettingItem( aSettingId, iData->acceptPrivacy );
		}*/
	return NULL;
	}


/*void CSettingsItemList::HandleResourceChange( TInt aType )
	{
	CAknSettingItemList::HandleResourceChange( aType );
	//	SetRect( iAvkonViewAppUi->View( TUid::Uid( ESettingItemList1ViewId ) )->ClientRect() );
	// Rimpiazzato per ottenere indipendenza dalla view... Ma forse non funzia per bene...
	SetRect( iAvkonViewAppUi->iView->ClientRect() );
	}*/

// End of File
