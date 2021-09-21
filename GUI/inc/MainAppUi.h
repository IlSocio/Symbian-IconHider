/*
========================================================================
 Name        : HelloWorldAppUi.h
 Author      : Marco Bellino
 Copyright   : FuzzyByte
 Description : 
========================================================================
*/
#ifndef MainAPPUI_H
#define MainAPPUI_H

#include <aknviewappui.h>
//#include "AboutView.h"
#include "ProtectionView.h"
#include "SettingsView.h"

#include <FuzzyByte\IconHider\Settings_consts.h>

#include <FuzzyByte\FuzWebRegister.h>
#include <FuzzyByte\ProgressBar.h>
#include <FuzzyByte\TimeOutTimer.h>


class CAknNavigationDecorator;
class CAknTabGroup;


#include <calprogresscallback.h>


/**
 * @class	CHelloWorldAppUi HelloWorldAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CMainAppUi : public CAknViewAppUi, public MProgressBarObserver, MTimeOutNotifier, MCalProgressCallBack
	{
public: 
	// constructor and destructor
	CMainAppUi();
	virtual ~CMainAppUi();
	void ConstructL();

	virtual void Progress(TInt aPercentageCompleted) {};
	virtual void Completed(TInt aError)  {};
	virtual TBool NotifyProgress() {};
	
public:
	// from CCoeAppUi
	TKeyResponse HandleKeyEventL(
				const TKeyEvent& aKeyEvent,
				TEventCode aType );

	// from CEikAppUi
	void HandleCommandL( TInt aCommand );
	void HandleResourceChangeL( TInt aType );

	// from CAknAppUi
	void HandleViewDeactivation( 
			const TVwsViewId& aViewIdToBeDeactivated, 
			const TVwsViewId& aNewlyActivatedViewId );

	virtual void RequestCancelled();
	
	void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

private:
    virtual void TimerExpiredL(TAny* src);
	void CheckForUpdatesL();
	void SetActiveTabByViewId( TUid aViewId );
	void InitializeContainersL();
	TBool GotRegCodeL(const TDesC& regCode);
	void AddNewProfileTabL(TInt aViewId);
	void UpdateTabSizeL();
	void ShowLongTextFromFileL(const TDesC& fileName, const TInt aTitleRes);
	

private:
	//CAboutView* iAboutView;
	CProtectionView* iProtView;
	CSettingsView* iSettView;
	CAknNavigationDecorator* iNaviDecorator_;
	CAknTabGroup* iNaviTabs1;
	CFuzWebRegister* iWebReg;
	CProgressBar* iProgressBar;
	TInt iViewsCounter;
	TBool iIsRegistered;
	CTimeOutTimer* iTimer;
};

#endif // HELLOWORLDAPPUI_H			
