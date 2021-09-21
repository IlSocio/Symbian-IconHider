/*
========================================================================
 Name        : MainApplication.h
 Author      : Marco Bellino
 Copyright   : FuzzyByte
 Description : 
========================================================================
*/
#ifndef MainAPPLICATION_H
#define MainAPPLICATION_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknapp.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Constants]
const TUid KUidMainApplication = { 0x20024264 };
// ]]] end generated region [Generated Constants]

/**
 *
 * @class	CMainApplication MainApplication.h
 * @brief	A CAknApplication-derived class is required by the S60 application 
 *          framework. It is subclassed to create the application's document 
 *          object.
 */
class CMainApplication : public CAknApplication
	{
private:
	TUid AppDllUid() const;
	CApaDocument* CreateDocumentL();
	
	};
			
#endif // MainAPPLICATION_H		
