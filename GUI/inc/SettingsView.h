/*
============================================================================
 Name        : CSettingsView from GuardGUIView2.h
 Author      : Marco Bellino
 Version     :
 Copyright   : All Rights Reserved
 Description : Declares view for application.
============================================================================
*/

#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H


// INCLUDES
#include "SettingsItemList.h"
#include "SettingsContainer.h"
#include "SettingsListboxContainer.h"

//#include <FuzzyByte\IconHider\Data_consts_20024264.h>
//#include <FuzzyByte\IconHider\Data.h>

#include <FuzzyByte\ProgressBar.h>
#include <FuzzyByte\FuzWebUpdate.h>
#include <FuzzyByte\MyObexClient.h>
#include <FuzzyByte\IconHider\Client.h>
	 
#include <aknview.h>
#include <eiktxlbx.h>

_LIT(KFileTmpInst, "c:\\system\\data\\20024264\\iconhider.sisx");			// Pacchetto per la redistribuzione. Path usato in fase di installazione.

// FORWARD DECLARATIONS
class CSettingsContainer;

// CLASS DECLARATION

enum TUpdateStatus 
	{
	EUpdateDownloaded,
	ENoUpdateAvail,
	EAborted
	};

/**
*  CSettingsView view class.
*
*/
class CSettingsView : public CAknView, public MProgressBarObserver
	{
	public: // Constructors and destructor
		static CSettingsView* NewL(TBool regged);
		static CSettingsView* NewLC(TBool regged);

		/**
		* Destructor.
		*/
		~CSettingsView();

		//virtual void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);
	public: // Functions from base classes
		void RequestCancelled();
		void HandleClientRectChange();
		void DynInitMenuPaneL( TInt aResourceId,	CEikMenuPane* aMenuPane );
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

		//void HandleListBoxEventL(CEikListBox* /*aListBox*/, TListBoxEvent aEventType);
		void UpdateViewL(TBool registered);
		
	protected:
		CSettingsView(TBool regged);

		/**
		* EPOC default constructor.
		*/
		void ConstructL();

	private:
		TBool AskCodeL(TDes& pass);
		TUpdateStatus CheckForUpdatesL();
		void HandleStatusPaneSizeChange();
		void SetupStatusPaneL();

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

	private: // Data
		CProgressBar* iProgressBar;
		TBool iAcceptTouchEvents;
		TBool iTouchSupported;
		TInt iOldIndex;
//		CSettingsItemList* iList;
//		CSettingsContainer* iContainer;
		CSettingsListboxContainer* iContainer;
		HBufC* iFileRedist;
		TInt iFileSizeRedist;
		CMyObexClient* iBtIr;
		TBool iHasBT;
		TBool iHasIR;
		CFuzWebUpdate* iWebUpd;
		RHider iHider;
		TBool iRegistered;
		TBool iHasRedistFile;
	};

#endif


