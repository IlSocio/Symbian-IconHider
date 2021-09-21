/*
========================================================================
 Name        : MainDocument.cpp
 Author      : Marco Bellino
 Copyright   : FuzzyByte
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated User Includes]
#include "MainDocument.h"
#include "MainAppUi.h"
// ]]] end generated region [Generated User Includes]

/**
 * @brief Constructs the document class for the application.
 * @param anApplication the application instance
 */
CMainDocument::CMainDocument( CEikApplication& anApplication )
	: CAknDocument( anApplication )
	{
	}

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here.  
 */ 
void CMainDocument::ConstructL()
	{
	}
	
/**
 * Symbian OS two-phase constructor.
 *
 * Creates an instance of CMainDocument, constructs it, and
 * returns it.
 *
 * @param aApp the application instance
 * @return the new CMainDocument
 */
CMainDocument* CMainDocument::NewL( CEikApplication& aApp )
	{
	CMainDocument* self = new ( ELeave ) CMainDocument( aApp );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * @brief Creates the application UI object for this document.
 * @return the new instance
 */	
CEikAppUi* CMainDocument::CreateAppUiL()
	{
	return new ( ELeave ) CMainAppUi;
	}
				
