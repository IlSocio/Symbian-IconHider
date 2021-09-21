/*
========================================================================
 Name        : MainDocument.h
 Author      : Marco Bellino
 Copyright   : FuzzyByte
 Description : 
========================================================================
*/
#ifndef MainDOCUMENT_H
#define MainDOCUMENT_H

#include <akndoc.h>
		
class CEikAppUi;

/**
* @class	CMainDocument MainDocument.h
* @brief	A CAknDocument-derived class is required by the S60 application 
*           framework. It is responsible for creating the AppUi object. 
*/
class CMainDocument : public CAknDocument
	{
public: 
	// constructor
	static CMainDocument* NewL( CEikApplication& aApp );

private: 
	// constructors
	CMainDocument( CEikApplication& aApp );
	void ConstructL();
	
public: 
	// from base class CEikDocument
	CEikAppUi* CreateAppUiL();
	};
#endif // MainDOCUMENT_H
