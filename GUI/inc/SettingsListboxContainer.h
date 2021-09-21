/*
============================================================================
 Name        : CSettingsListboxContainer from SettingsContainer.h
 Author      : Marco Bellino
 Version     :
 Copyright   : All Rights Reserved
 Description : Declares container control for application.
============================================================================
*/

#ifndef __SettingsListboxContainer_H
#define __SettingsListboxContainer_H

// INCLUDES
#include <coecntrl.h>
#include <eiktxlbx.h>
#include <eiklbo.h>
//#include "client.h"
#include "SettingsItemList.h"
//#include <FuzzyByte\IconHider\Data_consts_20024264.h>


// FORWARD DECLARATIONS
class CEikLabel;        // for example labels



// CLASS DECLARATION

/**
*  CSettingsListboxContainer  container control class.
*  
*/
class CSettingsListboxContainer : public CCoeControl, public MEikListBoxObserver, MCoeControlObserver
	{
	public: // Constructors and destructor
		static CSettingsListboxContainer* NewL(const TRect& aRect, const CCoeControl* aParent=NULL);
		
		CSettingsListboxContainer() : iSettingItemArray( 3, EFalse, 0 ) {} 
		 
		/**
		* Destructor.
		*/
		~CSettingsListboxContainer();
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
		void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	public: // New functions

		void UpdateListBoxViewL(TBool enabled, TBool full);

		// MEikListBoxObserver
		void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);
	private:
		void ConstructL(const TRect& aRect, const CCoeControl* aParent);

	private: // Functions from base classes
		//void AddCurrentProfileL(TInt aIconIndex);
		
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
		void AddListBoxItemL(TInt aResourceId1, TInt aResourceId2, TInt aIconIndex);
		void AddListBoxItemL(TInt aResourceId1, const TDesC& aText, TInt aIconIndex);

	private: //data
		TBool iRegistered;
		CEikTextListBox *iListBox;
		CAknSettingItemArray iSettingItemArray;		
	};

#endif


