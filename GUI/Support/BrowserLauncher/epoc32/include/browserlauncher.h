/*
* ==============================================================================
*  Name        : BrowserLauncher.h
*  Part of     : Browser UI / BrowserLauncher
*  Interface   : Browser UI, BrowserLauncher
*  Description : Browser launcher service. Clients can instruct the Browser to 
*                download something.
*  Version     : 2.0
*
*  Copyright © 2002-2004 Nokia. All rights reserved.
*  This material, including documentation and any related 
*  computer programs, is protected by copyright controlled by 
*  Nokia. All rights are reserved. Copying, including 
*  reproducing, storing, adapting or translating, any 
*  or all of this material requires the prior written consent of 
*  Nokia. This material also contains confidential 
*  information which may not be disclosed to others without the 
*  prior written consent of Nokia.
* ==============================================================================
*/


#ifndef BROWSERLAUNCHER_H
#define BROWSERLAUNCHER_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <FavouritesDb.h>

// FORWARD DECLARATIONS
class CBrowserLauncherExtension;
class MDownloadedContentHandler;
class MAknServerAppExitObserver;
class TBrowserOverriddenSettings;

// CLASS DECLARATION

/**
*  Browser Launcher.
*  It launches the Browser application embedded to download the required 
*  content. The operation can be cancelled with Cancel().
*
*  @lib BrowserLauncher.lib
*  @since Series 60 2.0
*/
NONSHARABLE_CLASS( CBrowserLauncher ): public CBase
    {
	public: // Constructors and destructor

		/** 
		* Two-phased constructor. Launch Kimono Browser.
        * @return The created instance of CBrowserLauncher
        */
		IMPORT_C static CBrowserLauncher* NewL();
		
		/** 
		* Two-phased constructor. Launch Kimono Browser.
        * @return The created instance of CBrowserLauncher
        */		
		IMPORT_C static CBrowserLauncher* NewLC();

		/** 
		* Two-phased constructor. Launch BrowserNG.
        * @param aOverrideBrowserUid Uid of BrowseNG
        * @return The created instance of CBrowserLauncher
        */
		IMPORT_C static CBrowserLauncher* NewL( TInt aOverrideBrowserUid );
		
		/** 
		* Two-phased constructor. Launch BrowserNG.
        * @param aOverrideBrowserUid Uid of BrowseNG
        * @return The created instance of CBrowserLauncher
        */		
		IMPORT_C static CBrowserLauncher* NewLC( TInt aOverrideBrowserUid );

		/**
		* Destructor
		*/
		IMPORT_C virtual ~CBrowserLauncher();

	public: // New functions

        // Asynchronous methods -----------------------------------------------

        /**
        * Launch the Browser embedded with the given parameters. Asynchronous 
        * operation.
        * @param aParams Parameters to pass to the browser through the OpenFileL method.
        * @param aContentHandler Implementation of the file based content handler.
        * @param aExitObserver Embedded Browser application exit observer.
        * @param aSettings Overridden user preferences. Ownership is not taken!
        * @return None
        */
        IMPORT_C void LaunchBrowserEmbeddedL
                      ( const TDesC& aParams, 
                        MDownloadedContentHandler* aContentHandler = NULL, 
                        MAknServerAppExitObserver* aExitObserver = NULL, 
                        TBrowserOverriddenSettings* aSettings = NULL );

        /**
        * Launch the Browser embedded with the given parameters. Asynchronous 
        * operation.
        * @param aContextId Context ID.
        * @param aContentHandler Implementation of the file based content handler.
        * @param aExitObserver Embedded Browser application exit observer.
        * @param aSettings Overridden user preferences. Ownership is not taken!
        * @return None
        */
        IMPORT_C void LaunchBrowserEmbeddedL
                      ( TInt aContextId, 
                        MDownloadedContentHandler* aContentHandler = NULL, 
                        MAknServerAppExitObserver* aExitObserver = NULL, 
                        TBrowserOverriddenSettings* aSettings = NULL );


        // Synchronous methods -----------------------------------------------

        /**
        * Launch the Browser embedded with the given parameters. 
        * The operation is synchronous, and waits until the Browser terminates.
        * Be careful when using: it uses CActiveSchedulerWait!
        * @param aParams Parameters to pass to the browser through the 
        *                OpenFileL method.
        * @param aContentHandler Implementation of the file based content handler.
        * @param aSettings Overridden user preferences. Ownership is not taken!
        * @return Integer error value returned by the embedded Browser.
        */
        IMPORT_C TInt LaunchBrowserSyncEmbeddedL
                      ( const TDesC& aParams, 
                        MDownloadedContentHandler* aContentHandler = NULL, 
                        TBrowserOverriddenSettings* aSettings = NULL );

        /**
        * Launch the Browser embedded with the given parameters. 
        * The operation is synchronous, and waits until the Browser terminates.
        * Be careful when using: it uses CActiveSchedulerWait!
        * @param aContextId Context ID.
        * @param aContentHandler Implementation of the file based content handler.
        * @param aSettings Overridden user preferences. Ownership is not taken!
        * @return Integer error value returned by the embedded Browser.
        */
        IMPORT_C TInt LaunchBrowserSyncEmbeddedL
                      ( TInt aContextId, 
                        MDownloadedContentHandler* aContentHandler = NULL, 
                        TBrowserOverriddenSettings* aSettings = NULL );

        // Cancel method -----------------------------------------------

        IMPORT_C void Cancel();

    private: // Constructors
    
    	/**
    	* Default constructor
    	*/
         CBrowserLauncher();
         
		/** 
		* Overloaded constructor
        * @param aOverrideBrowserUid Uid of BrowseNG
        * @return None
        */		
        CBrowserLauncher( TInt aOverrideBrowserUid );
        
        /** 
        * Second phase constructor
        */
        void ConstructL();

    private: // New functions

        /**
        * Launch the Browser embedded with the given parameters. Asynchronous 
        * operation.
        * @param aParams Parameters to pass to the browser through the OpenFileL method.
        * @param aContextId Context ID.
        * @param aContentHandler Implementation of the file based content handler.
        * @param aExitObserver Embedded Browser application exit observer.
        * @param aSettings Overridden user preferences. Ownership is not taken!
        * @return None.
        */
        void LaunchBrowserEmbeddedL
                      ( const TDesC& aParams, 
                        TInt aContextId, 
                        MDownloadedContentHandler* aContentHandler, 
                        MAknServerAppExitObserver* aExitObserver, 
                        TBrowserOverriddenSettings* aSettings );

        TInt FolderByContextIdL( TUint32 aContextId );
        TInt BookmarksIdByFolderIdL( TUint32 aFolderId );
        TBool EnhancedSeamlessLinkLocalFeaturesSupportedL();

    private: // Data members

        // Extension. Owned.
        CBrowserLauncherExtension* iExtension; 
        
        // The Uid of the Browser to lauch. ( KimonoBrowser/BrowserNG )
        TUid iBrowserUid;

        RFavouritesSession iFavouritesSess;

    private:

        friend class CBrowserLauncherExtension;
    };

// CLASS DECLARATION

/**
*  TLauncherPackageUtils
*  Helper class to pack data on client side, and unpack the same data on server
*  side, that was sent through IPC
*/
class TLauncherPackageUtils
    {
	public:
		/**
		* Client packs the data that is about to send throudh IPC.
	    * @param aEmbeddingAppUid The embedding application`s UID		
	    * @param aSettings Overridden user preferences. Ownership is not taken!
	    * @param aFolderUid The Uid of the folder that the Browser should open.
	    * @param seamlessParam	    	    
	    * @param aIsContentHandlerRegistered True if a content handler is registered
		* @return The packed data that can be send through IPC.
		*/		
		static HBufC8* PackLauncherDataL( const TUid aEmbeddingAppUid, 
										  const TBrowserOverriddenSettings* aSettings, 
										  TInt folderUid, 
										  const TDesC& seamlessParam, 
										  TBool aIsContentHandlerRegistered );
		
		/**
		* Server unpacks the data that was sent from client throudh IPC.
		* The method sets the parameters values.
	    * @param aEmbeddingAppUid The embedding application`s UID
	    * @param aSettings Overridden user preferences. Ownership is not taken!
	    * @param aFolderUid The Uid of the folder that the Browser should open.
	    * @param aSeamlessParam 
	    * @param aData Data to unpack.
	    * @param aIsContentHandlerRegistered True if a content handler is registered
	    * @param aIsOverriddenSettings True if the client set an overridden setting
		*/		
		static void UnPackLauncherDataL( TUid& aEmbeddingAppUid, 
										 TBrowserOverriddenSettings* aSettings, 
										 TInt& aFolderUid, 
										 TPtrC& aSeamlessParam, 
										 const TDesC8& aData, 
										 TBool& aIsContentHandlerRegistered, 
										 TBool& aIsOverriddenSettings );
    };

#endif // BROWSERLAUNCHER_H