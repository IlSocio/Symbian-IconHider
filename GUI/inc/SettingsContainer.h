/*
============================================================================
 Name        : CSettingsContainer from SettingsContainer.h
 Author      : Marco Bellino
 Version     :
 Copyright   : All Rights Reserved
 Description : Declares container control for application.
============================================================================
*/

#ifndef __SettingsContainer_H
#define __SettingsContainer_H

// INCLUDES
#include <coecntrl.h>
#include <eiktxlbx.h>
#include <eiklbo.h>
#include "SettingsItemList.h"

// FORWARD DECLARATIONS
class CEikLabel;        // for example labels
class TSettingsData1;

// CLASS DECLARATION

/**
*  CSettingsContainer  container control class.
*  
*/
class CSettingsContainer : public CCoeControl, MCoeControlObserver
	{
	public: // Constructors and destructor
		static CSettingsContainer* NewL(const TRect& aRect, MEikListBoxObserver& observer, const CCoeControl* aParent=NULL);
		
		CSettingsContainer() : iSettingItemArray( 3, EFalse, 0 ) {} 
		 
		/**
		* Destructor.
		*/
		~CSettingsContainer();
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
		void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	public: // New functions
		void UpdateListBoxViewL(TInt isEnabled, TBool isRegistered);

	private:
		void ConstructL(const TRect& aRect, MEikListBoxObserver& observer, const CCoeControl* aParent);
		HBufC* GetItemStringLC(TInt resourceID, const TDesC& value);

	private: // Functions from base classes

		/**
		* From CoeControl,SizeChanged.
		*/
		void SizeChanged();

		/**
		* From CoeControl,CountComponentControls.
		*/
		TInt CountComponentControls() const;

		/**
		* From CCoeControl,ComponentControl.
		*/
		CCoeControl* ComponentControl(TInt aIndex) const;

		/**
		* From CCoeControl,Draw.
		*/
		void Draw(const TRect& aRect) const;

		/**
		* From MCoeControlObserver
		* Acts upon changes in the hosted control's state. 
		*
		* @param aControl	The control changing its state
		* @param aEventType	The type of control event 
		*/
		void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

		void CreateSettingItemsL();
	public:
		CEikTextListBox *iListBox;
		CSettingsItemList* iItemList;
		CAknSettingItemArray iSettingItemArray;		
		TSettingsData1 iData;
  	    TInt iValue;
	private: //data
	};

#endif


