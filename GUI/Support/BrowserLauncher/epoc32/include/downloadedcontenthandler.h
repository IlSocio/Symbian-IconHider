/*
* ==============================================================================
*  Name        : DownloadedContentHandler.h
*  Part of     : Browser UI / BrowserLauncher
*  Interface   : Browser UI, BrowserLauncher, DownloadedContentHandler
*  Description : Downloaded content handler interface.
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

#ifndef DOWNLOADEDCONTENTHANDLER_H
#define DOWNLOADEDCONTENTHANDLER_H

#include <e32std.h>
#include <apmstd.h>

// FORWARD DECLARATIONS

class RFile;
class CAiwGenericParamList;

// CLASS DECLARATION

/**
*  Downloaded content handler interface. Clients should implement 
*  it if they want to handle content that has been downloaded by 
*  the BrowserLauncher.
*/
class MDownloadedContentHandler
    {
    public:

        /**
        * Handle downloaded content that has been saved to a file, 
        * and return ETrue if it was handled successfully.
        * @param aFileName File name with path.
        * @param aParamList Generic parameter list.
        * @param aContinue Output parameter: It must be ETrue if the caller wants 
        *                  Browser not to terminate after the content is handled.
        * @return ETrue if the implementer has processed the content. If it is 
        *                  EFalse, BrowserLauncher opens that content in an 
        *                  appropriate viewer application.
        */
        virtual TBool HandleContentL( const TDesC& aFileName, 
                                      const CAiwGenericParamList& aParamList, 
                                      TBool& aContinue ) = 0;

        // reserved for future use
        virtual TAny* DownloadedContentHandlerReserved1( TAny* /*aAnyParam*/ ) = 0;
    };

#endif // DOWNLOADEDCONTENTHANDLER_H
