

#ifndef __FuzSettingITEMLIST_H__
#define __FuzSettingITEMLIST_H__


// INCLUDES
#include <AknSettingItemList.h>



class CFuzSettingItemList : public CAknSettingItemList
	{
public:
	void ItemSetHiddenL(TInt itemId, TBool hide);
	void ItemSetFlags(TInt itemId, TInt flags);
	CAknSettingItem* ItemFindById(TInt itemId);
	void ItemChangeSelectedL();
	
	virtual void SizeChanged();
	virtual TKeyResponse CFuzSettingItemList::OfferKeyEventL( 
			const TKeyEvent& aKeyEvent, 
			TEventCode aType );	
	};

#endif
// End of File
