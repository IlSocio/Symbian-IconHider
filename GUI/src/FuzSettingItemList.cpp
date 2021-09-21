/*  Copyright (c) 2004, Nokia. All rights reserved */

// INCLUDE FILES
#include "FuzSettingItemList.h"


void CFuzSettingItemList::ItemSetFlags(TInt itemId, TInt flags)
	{	
	CAknSettingItem* item = ItemFindById(itemId);
	if (item == NULL)
		return;
	item->SetSettingPageFlags(flags);
	}


CAknSettingItem* CFuzSettingItemList::ItemFindById(TInt itemId)
	{
	for (TInt i=0; i<SettingItemArray()->Count(); i++)
		{
		CAknSettingItem* settingItem= SettingItemArray()->At(i); 
		if (settingItem->Identifier() == itemId)
			return settingItem;
		}	
	return NULL;
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

/**
 *	Handle the "Change" option on the Options menu.  This is an
 *	alternative to the Selection key that forces the settings page
 *	to come up rather than changing the value in place (if possible).
 */
void CFuzSettingItemList::ItemChangeSelectedL()
	{
	if ( ListBox()->CurrentItemIndex() >= 0 )
		{
		EditItemL( ListBox()->CurrentItemIndex(), ETrue );
		}
	}


void CFuzSettingItemList::ItemSetHiddenL(TInt itemId, TBool hide)
	{
	CAknSettingItem* item = ItemFindById(itemId);
	if (item == NULL)
		return;
	item->SetHidden( hide );	
//	SettingItemArray()->RecalculateVisibleIndicesL(); Forse Basta???
	HandleChangeInItemArrayOrVisibilityL();
	}


void CFuzSettingItemList::SizeChanged()
	{
	if ( ListBox() ) 
		{
		ListBox()->SetRect( Rect() );
		}
	}


TKeyResponse CFuzSettingItemList::OfferKeyEventL( 
		const TKeyEvent& aKeyEvent, 
		TEventCode aType )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	if ( aKeyEvent.iCode == EKeyLeftArrow 
		|| aKeyEvent.iCode == EKeyRightArrow )
		{
		// allow the tab control to get the arrow keys
		return EKeyWasNotConsumed;
		}
	
	return CAknSettingItemList::OfferKeyEventL( aKeyEvent, aType );
	}
