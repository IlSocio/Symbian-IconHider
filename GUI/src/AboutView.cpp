/*
========================================================================
 Name        : AboutView.cpp
 Author      : Marco Bellino
 Copyright   : FuzzyByte
 Description : 
========================================================================
*/

#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <akncontext.h>
#include <akntitle.h>
#include <stringloader.h>
#include <barsread.h>
#include <eikbtgpc.h>
#include <FuzzyByte\guiutils.h>

#include  <IconHider_20024264.rsg>
#include "Main.hrh"
#include "AboutView.h"
#include "AboutContainer.h"
#include "MessageContainer.h"

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CAboutView::CAboutView()
	{
	iAboutContainer = NULL;
	}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CAboutView::~CAboutView()
	{
	if ( iAboutContainer != NULL )
		{
		AppUi()->RemoveFromStack( iAboutContainer );
		delete iAboutContainer;
		iAboutContainer = NULL;
		}
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CAboutView
 */
CAboutView* CAboutView::NewL()
	{
	CAboutView* self = CAboutView::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CAboutView
 */
CAboutView* CAboutView::NewLC()
	{
	CAboutView* self = new ( ELeave ) CAboutView();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CAboutView::ConstructL()
	{
	BaseConstructL( R_ABOUT_VIEW );
	}

/**
 * @return The UID for this view
 */
TUid CAboutView::Id() const
	{
	return TUid::Uid( EAboutViewId );
	}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CAboutView::HandleCommandL( TInt aCommand )
	{
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
		default:
			break;
		}
			
	if ( !commandHandled ) 
		{	
		if ( aCommand == EAknSoftkeyExit )
			{
			AppUi()->HandleCommandL( EEikCmdExit );
			}	
		}
	}

/**
 *	Handles user actions during activation of the view, 
 *	such as initializing the content.
 */
void CAboutView::DoActivateL( 
		const TVwsViewId& /*aPrevViewId*/,
		TUid /*aCustomMessageId*/,
		const TDesC8& /*aCustomMessage*/ )
	{
	SetupStatusPaneL();
				
	if ( iAboutContainer == NULL )
		{
		iAboutContainer = CreateContainerL();
		iAboutContainer->SetMopParent( this );
		AppUi()->AddToStackL( iAboutContainer );
		} 
	}

/**
 */
void CAboutView::DoDeactivate()
	{
	CleanupStatusPane();
	
	if ( iAboutContainer != NULL )
		{
		AppUi()->RemoveFromStack( iAboutContainer );
		delete iAboutContainer;
		iAboutContainer = NULL;
		}
	}

/** 
 * Handle status pane size change for this view (override)
 */
void CAboutView::HandleStatusPaneSizeChange()
	{
	CAknView::HandleStatusPaneSizeChange();
	
	// this may fail, but we're not able to propagate exceptions here
	TVwsViewId view;
	AppUi()->GetActiveViewId( view );
	if ( view.iViewUid == Id() )
		{
		TInt result;
		TRAP( result, SetupStatusPaneL() );
		}
	}


void CAboutView::SetupStatusPaneL()
	{
	// reset the context pane
/*	TUid contextPaneUid = TUid::Uid( EEikStatusPaneUidContext );
	CEikStatusPaneBase::TPaneCapabilities subPaneContext = 
		StatusPane()->PaneCapabilities( contextPaneUid );
	if ( subPaneContext.IsPresent() && subPaneContext.IsAppOwned() )
		{
		CAknContextPane* context = static_cast< CAknContextPane* > ( 
			StatusPane()->ControlL( contextPaneUid ) );
		context->SetPictureToDefaultL();
		}*/
	
	/*// setup the title pane
	TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
	CEikStatusPaneBase::TPaneCapabilities subPaneTitle = 
		StatusPane()->PaneCapabilities( titlePaneUid );
	if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
		{
		CAknTitlePane* title = static_cast< CAknTitlePane* >( 
			StatusPane()->ControlL( titlePaneUid ) );
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_ABOUT_TITLE_RESOURCE );
		title->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}*/
	GuiUtils::StatusPaneDefaultContextPictureL( StatusPane() );
	GuiUtils::StatusPaneSetupTitleL( StatusPane(), R_ABOUT_TITLE_RESOURCE );
	}

void CAboutView::CleanupStatusPane()
	{
	}

/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CAboutContainer::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CAboutContainer
 */
CAboutContainer* CAboutView::CreateContainerL()
	{
	return CAboutContainer::NewL( ClientRect(), NULL, this );
	}

/*CMessageContainer* CAboutView::CreateContainerL()
	{
	return CMessageContainer::NewL( ClientRect(), this );
	}*/
