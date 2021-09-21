/*
============================================================================
 Name        : CProtectionView from GuardGUIView2.h
 Author      : Marco Bellino
 Version     :
 Copyright   : All Rights Reserved
 Description : Declares view for application.
============================================================================
*/

#ifndef ProtectionView_H
#define ProtectionView_H

// INCLUDES
#include <aknview.h>
#include <eiktxlbx.h>
#include <FuzzyByte\IconHider\client.h>
#include <FuzzyByte\FileLogger.h>
#include <FuzzyByte\ProgressBar.h>
#include "protectioncontainer.h"

// CONSTANTS
// UID of view
//const TUid KView4Id = {4};

// FORWARD DECLARATIONS
//class CProtectionContainer;

// CLASS DECLARATION

/**
*  CProtectionView view class.
* 
*/
class CProtectionView : public CAknView, public MMultiSelectionObserver, public MProgressBarObserver
	{
	public: // Constructors and destructor

		static CProtectionView* NewL(TInt aViewId, TBool largeIcons, TBool regged);
		static CProtectionView* NewLC(TInt aViewId, TBool largeIcons, TBool regged);

		CProtectionView(TInt aViewId, TBool regged);
		/**
		* Destructor.
		*/
		~CProtectionView();

		void RequestCancelled();		
		void UpdateViewL(TBool registered);

	public: // Functions from base classes

		void SetIconSizeL(TBool large);
		
		void HandleItemChangeEventL(const TInt32 uid, const TBool checked);

		/**
		* From CAknView returns Uid of View
		* @return TUid uid of the view
		*/
		TUid Id() const;

		/**
		* From MEikMenuObserver delegate commands from the menu
		* @param aCommand a command emitted by the menu 
		* @return void
		*/
		void HandleCommandL(TInt aCommand);

		/**
		* From CAknView reaction if size change
		* @return void
		*/
		void HandleClientRectChange();
		void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );


//		void HandleListBoxEventL(CEikListBox* /*aListBox*/, TListBoxEvent aEventType);

	private:
		void HandleStatusPaneSizeChange();
		void SetupStatusPaneL();

		/**
		* EPOC default constructor.
		*/
		void ConstructL(TBool largeIcons);
		
		/**
		* From CAknView activate the view
		* @param aPrevViewId 
		* @param aCustomMessageId 
		* @param aCustomMessage 
		* @return void
		*/
		void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,
			const TDesC8& aCustomMessage);

		/**
		* From CAknView deactivate the view (free resources)
		* @return void
		*/
		void DoDeactivate();

		void RemoveAllUidEntriesL();

	private: // Data
		__LOG_DECLARATION_MEMBER
		CProtectionContainer* iContainer;
		TBool iWasFindBoxVisible;
		TBool iCancel;
		CProgressBar* iProgressBar;
		TBool iLargeIcons;
		TInt iViewId;
		RHider iHider;
		TBool iRegistered;
	};

#endif


