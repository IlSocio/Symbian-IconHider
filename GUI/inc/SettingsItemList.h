/*  Copyright (c) 2004, Nokia. All rights reserved */

#ifndef __SettingsItemList_H_
#define __SettingsItemList_H_

// INCLUDES
#include <AknSettingItemList.h>
#include "FuzSettingItemList.h"



class TSettingsData1
	{
public:
	TInt status;		// sempre / mai / al cambio sim / automatico
	
	};

/**
 * CSkebbySetPageSubscribe class.
 */
class CSettingsItemList : public CFuzSettingItemList
	{
public:
	// Constructors and destructor
	/**
	 * NewL
	 * 2-phase constructor.
	 * @param aResourceId : resource ID of setting page.
	 */
	static CSettingsItemList* NewL(TSettingsData1& aData, const TRect& aRect, const CCoeControl* aParent=NULL);

	/**
	 * ~CSkebbySetPageSubscribe
	 * Destructor.
	 */
	virtual ~CSettingsItemList();

	// From CAknSettingItemList 
	/**
	 * From CAknSettingItemList, CreateSettingItemL
	 * Create items of setting page.
	 * @param aSettingId : item ID.
	 */
	virtual CAknSettingItem* CreateSettingItemL(TInt aSettingId);

	virtual void EditItemL(TInt aIndex, TBool aCalledFromMenu);

private:
	
	/**
	 * CSkebbySetPageSubscribe
	 * Default constructor.
	 */
	CSettingsItemList(TSettingsData1& aData);

	/**
	 * ConstructL
	 * Second phase constructor.
	 * @param aResourceId : resource ID of setting page.
	 */
	void ConstructL(const TRect& aRect, const CCoeControl* aParent);
	
private:
	TSettingsData1* iData;
	};

#endif 
// End of File
