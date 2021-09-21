/*
============================================================================
 Name        : CProtectionContainer from ProtectionContainer.h
 Author      : Marco Bellino
 Version     :
 Copyright   : All Rights Reserved
 Description : Declares container control for application.
============================================================================
*/

#ifndef ProtectionContainer_H
#define ProtectionContainer_H

// INCLUDES
#include <aknutils.h>
#include <aknsfld.h>
#include <coecntrl.h>
#include <eiktxlbx.h>
#include <eiklbo.h>
#include <aknlists.h>
#include <FuzzyByte\FileLogger.h>
#include <eikedwob.h>

// FORWARD DECLARATIONS
class CEikLabel;        // for example labels


class MMultiSelectionObserver
{
public:
	virtual void HandleItemChangeEventL(const TInt32 uid, const TBool checked) = 0;
};
// CLASS DECLARATION

/**
*  CProtectionContainer  container control class.
*
*/
class CProtectionContainer : public CCoeControl, public MCoeControlObserver, public MEikListBoxObserver
	{
	public: // Constructors and destructor
		static CProtectionContainer* NewL(const TRect& aRect, const CCoeControl* aParent, TBool largeIcons);
		static CProtectionContainer* NewLC(const TRect& aRect, const CCoeControl* aParent, TBool largeIcons);
		void ConstructL(const TRect& aRect, const CCoeControl* aParent, TBool largeIcons);

		void SetObserver(MMultiSelectionObserver* observer);
		CProtectionContainer();
		~CProtectionContainer();
		void HandlePointerEventL(const TPointerEvent& aPointerEvent);
		void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);
		
	public: // New functions
		TInt CountProtectedL();
		void UpdateListBoxViewL(TBool isLarge);
			// Restituisce True se la sim selezionata e' la prima
		TInt GetIndexFromUidL(TInt32 uid);
		TInt32 GetSelectedUidL();
		TBool IsMarkedL();
		TBool IsMarkedL(TInt32 uid);
		TBool SetApplicationStatusL(TInt32 uid, TBool protect);
		TBool ChangeApplicationStatusL(TInt32 uid);
		TBool IsFindBoxVisible();

	public: // Functions from base classes
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
		//void GetMarkedUids(RArray<TUint32>& uidList);

	
	private: // Functions from base classes
		TKeyResponse OfferKeyToFindBoxL(const TKeyEvent& aKeyEvent, TEventCode aType);
		void DeleteFindBox();
		void AddToItemListL(CDesCArray& array, const TDesC& name, const TUint icon);
		void CreateListBoxItemL( TDes& aBuffer, TInt aIconIndex, const TDesC& aMainText );

		TInt GetOriginalIndex() const;

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
		void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType);

		void CreateListBoxL(TBool large, TBool findBoxVisible);
		CArrayFixFlat<TApaAppInfo>* GetApplicationsLC();

	public:
		CArrayFixFlat<TApaAppInfo>* iFullAppList;
		
	private:
		__LOG_DECLARATION_MEMBER
		CEikColumnListBoxTypedef* iListBox;
		MMultiSelectionObserver* iObserver;
		RArray<TInt32> iMarkedUids;

		CAknSearchField::TSearchFieldStyle iStyle;
        CAknSearchField* iFindBox;
		HBufC* iOldFilter;
//		TBool iTouchSupported;
//		TBool iAcceptTouchEvents;
//		TInt iDBG_Down;
//		TInt iDBG_Click;
	};

#endif


