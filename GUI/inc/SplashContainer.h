/*
============================================================================
 Name        : CSplashContainer from SplashContainer.h
 Author      : Marco Bellino
 Version     :
 Copyright   : All Rights Reserved
 Description : Declares container control for application.
============================================================================
*/

#ifndef SPLASHCONTAINER_H
#define SPLASHCONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include <W32STD.H>
#include <FuzzyByte\FileLogger.h>


/**
*  CSplashContainer  container control class.
*  
*/
class CSplashContainer : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect);

        /**
        * Destructor.
        */
        ~CSplashContainer();
		void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    public: // New functions

    public: // Functions from base classes

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
		* @param	aControl	The control changing its state
		* @param	aEventType	The type of control event 
		*/
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
        
    private: //data        
		CFbsBitmap* iBackBitmap;
		TPixelsTwipsAndRotation iPixRot;
        CFbsBitmap* iGoldIcon;
        CFbsBitmap* iGoldIconMask;
		__LOG_DECLARATION_MEMBER
    };

#endif

// End of File
