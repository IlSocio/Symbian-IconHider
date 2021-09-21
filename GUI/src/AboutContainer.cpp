/*
========================================================================
 Name        : AboutContainer.cpp
 Author      : Marco Bellino
 Copyright   : FuzzyByte
 Description : 
========================================================================
*/
#include <aknview.h>	// iAvkonViewAppUi

#include <aknviewappui.h>
#include <eikappui.h>
#include "AboutContainer.h"
#include "Main.hrh"

/**
 * First phase of Symbian two-phase construction. Should not 
 * contain any code that could leave.
 */
CAboutContainer::CAboutContainer()
	{
	}

/** 
 * Destroy child controls.
 */
CAboutContainer::~CAboutContainer()
	{
	}
				
/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CAboutContainer
 */
CAboutContainer* CAboutContainer::NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CAboutContainer* self = CAboutContainer::NewLC( 
			aRect, 
			aParent, 
			aCommandObserver );
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is left on cleanup stack.
 * @param aRect The rectangle for this window
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return new instance of CAboutContainer
 */
CAboutContainer* CAboutContainer::NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CAboutContainer* self = new ( ELeave ) CAboutContainer();
	CleanupStack::PushL( self );
	self->ConstructL( aRect, aParent, aCommandObserver );
	return self;
	}
			
/**
 * Construct the control (second phase).
 *  Creates a window to contain the controls and activates it.
 * @param aRect bounding rectangle
 * @param aCommandObserver command observer
 * @param aParent owning parent, or NULL
 */ 
void CAboutContainer::ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	if ( aParent == NULL )
	    {
		CreateWindowL();
	    }
	else
	    {
	    SetContainerWindowL( *aParent );
	    }
	iFocusControl = NULL;
	iCommandObserver = aCommandObserver;
	InitializeControlsL();
	SetRect( aRect );
	ActivateL();
	}
			
/**
* Return the number of controls in the container (override)
* @return count
*/
TInt CAboutContainer::CountComponentControls() const
	{
	return ( int ) ELastControl;
	}
				
/**
* Get the control with the given index (override)
* @param aIndex Control index [0...n) (limited by #CountComponentControls)
* @return Pointer to control
*/
CCoeControl* CAboutContainer::ComponentControl( TInt aIndex ) const
	{
	switch ( aIndex )
		{
		}
	return NULL;
	}
				
/**
 *	Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */				
void CAboutContainer::SizeChanged()
	{
	CCoeControl::SizeChanged();
	LayoutControls();
	}
				
/**
 * Layout components as specified in the UI Designer
 */
void CAboutContainer::LayoutControls()
	{
	}

/**
 *	Handle key events.
 */				
TKeyResponse CAboutContainer::OfferKeyEventL( 
		const TKeyEvent& aKeyEvent, 
		TEventCode aType )
	{
	if ( iFocusControl != NULL
		&& iFocusControl->OfferKeyEventL( aKeyEvent, aType ) == EKeyWasConsumed )
		{
		return EKeyWasConsumed;
		}
	return CCoeControl::OfferKeyEventL( aKeyEvent, aType );
	}
				
/**
 *	Initialize each control upon creation.
 */				
void CAboutContainer::InitializeControlsL()
	{	
	}

/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CAboutContainer::HandleResourceChange( TInt aType )
	{
	CCoeControl::HandleResourceChange( aType );
	SetRect( iAvkonViewAppUi->View( TUid::Uid( EAboutViewId ) )->ClientRect() );
	}
				
/**
 *	Draw container contents.
 */				
void CAboutContainer::Draw( const TRect& aRect ) const
	{
	CWindowGc& gc = SystemGc();
	gc.Clear( aRect );
	}
				
