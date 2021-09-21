/*
 * ============================================================================
 *  Name        : CMessageContainer from MessageContainer.h
 *  Part of     : SkebbyGui
 *  Created     : 12.12.2008 by BITWEEN S.R.L.
 *  Description : Declares container control for application.
 *  Version     : 1.0
 *  Copyright   : BITWEEN S.R.L.
 * ============================================================================
 */

#ifndef MESSAGECONTAINER_H
#define MESSAGECONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include <eikedwob.h>                   // MEikEdwinObserver
#include <coecobs.h>                    // MCoeControlObserver
#include <eikedwin.h>
#include <eikgted.h> 

// LIBS
// cone.lib, form.lib (CTextLayout), gdi.lib (TFontSyle)

// CAPS
// None

// FORWARD DECLARATIONS
class CEikLabel;
class CEikEdwin;
class CAknsBasicBackgroundControlContext;


// CLASS DECLARATION
/**
 * CMessageContainer container control class.
 * Provides access to draw functions, displays data on screen.
 *
 */
class CMessageContainer: public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor

    	static CMessageContainer* CMessageContainer::NewL( 
    			const TRect& aRect, MEikCommandObserver* aCommandObserver, const CCoeControl* aParent = NULL, TBool aReadOnly = EFalse );

    	static CMessageContainer* CMessageContainer::NewLC( 
    			const TRect& aRect, MEikCommandObserver* aCommandObserver, const CCoeControl* aParent = NULL, TBool aReadOnly = EFalse );

    	void ConstructL(
    			const TRect& aRect, MEikCommandObserver* aCommandObserver, const CCoeControl* aParent);
		
        /**
         * NewL()
         * Two-phased constructor.
         * @param aRect Frame rectangle for container.
         * @param aReadOnly ETrue for read only container
         * @return Pointer to the created instance of CMessageContainer.
         */
//        static CMessageContainer* NewL( const TRect& aRect, TBool aReadOnly);

        /**
         * NewLC()
         * Two-phased constructor.
         * @param aRect Frame rectangle for container.
         * @param aReadOnly ETrue for read only container
         * @return Pointer to the created instance of CMessageContainer.
         */
//        static CMessageContainer* NewLC( const TRect& aRect, TBool aReadOnly);

        /**
         * ~CMessageContainer()
         * Destructor.
         */
        ~CMessageContainer();

    private: // From CCoeControl

        /**
         * SizeChanged()
         * Responds to size changes.
         */
        void SizeChanged();

        /**
         * CountComponentControls()
         * Gets the number of controls contained in a compound control.
         * @return The number of component controls contained by this control.
         */
        TInt CountComponentControls() const;

        /**
         * ComponentControl()
         * Gets the specified component of a compound control.
         * @param aIndex The index of the control to get.
         * @return The component control with an index of aIndex.
         */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
         * Draw()
         * Draws data to the screen.
         * @param aRect The region of the control to be redrawn.
         */
        void Draw( const TRect& aRect ) const;

        /**
         * OfferKeyEventL()
         * Handles key events.
         * @param aId The key event.
         * @param aType The type of key event.
         * @return Whether or not the key event was used by this control.
         */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                TEventCode aType );

    public: // MCoeControlObserver
        
        /**
         * HandleControlEventL()
         * Handles CCoeControl event.
         * @param aControl CCoeControl object where event occured.
         * @param aEventType Type of event occured.   
         */
        void HandleControlEventL( CCoeControl* aControl, TCoeEvent aEventType );
        
    private: // Control functions
        
        /**
         * CreateLabelL()
         * Creates to/from label control.
         */
        void CreateLabelL();
        
        /**
         * CreateNumberEditorL()
         * Creates number editor control.
                     */
        void CreateNumberEditorL();
        
        /**
         * CreateTextEditorL()
         * Creates text editor control.
         */
        void CreateTextEditorL();
        
        /**
         * CursorAtTop()
         * Determines if cursor is on the top line now.
         * @param aEditor Editor to check.
         * @return ETrue if cursor on the top line.
         */
        TBool CursorAtTop( const CEikEdwin& aEditor ) const;
        
        /**
         * CursorAtBottom()
         * Determines if cursor is on the bottom line now.
         * @param aEditor Editor to check.
         * @return ETrue if cursor on the bottom line.
         */
        // TBool CursorAtBottom( const CEikEdwin& aEditor ) const;
        
    public: // New functions
    	void GetRecipientsL(CDesCArray& array);
    	
        /**
         * InsertRecepientsL()
         * Inserts recepients.
         * @param aRecepients Recepients string.
         */
        void InsertRecipientsL( const TDesC& aRecipients );
        
        /**
         * SetTextL()
         * Sets text.
         * @param aText Text string.
         */
        void SetTextL( const TDesC& aText );
        
        /**
         * TextLC()
         * Retrieves the text from the text editor.
         * @return POinter to the editor's text.
         */
        HBufC* TextLC();
        
        /**
         * TextLength()
         * Returns length of text.
         * @return Count of symbols in text field.
         */ 
        TInt TextLengthL() const;
        
        /**
         * SetReadOnlyL()
         * Sets editors in the read only state.
         * @param aState ETrue if read only.
         */
        void SetReadOnlyL( TBool aState = ETrue );
        
        /**
         * SetTextLimit()
         * Sets up new text limit for text editor.
         * @param aNewLimit Limit in symbols.
         */
        void SetTextLimit( const TInt aNewLimit );
        
        /**
         * IsReadOnly()
         * Returns container status.
         * @return ETrue if container is read only.
         */ 
        TBool IsReadOnly();
        
        /**
         * IsChangedL()
         * Returns whether container was changed or not.
         * @return ETrue if container was changed.
         */
        TBool IsChangedL();
        
    private: // Constructors
    	void NumEdDeleteCurrentParagraphL();
    	void NumEdDeleteParagraphL();
    	TInt NumEdCreateNewParagraphL(const TDesC& aRecepients, TBool isUserInput);
    	TBool NumEdCursorAtBottom();
    	TInt NumEdGetLineNumberAmount();
    	void NumEdSizeChanged();
    	TBool NumEdIsUserInputParagraph(TInt parPos);
    	TInt NumEdCreateNewUserInputParagraphL();
    	void NumEdMoveCursorOnNearParL(TInt relParPos);
    	void NumEdInsertTextAndMoveCursorL(const TDesC& aText);
    	void NumEdInsertTextAndMoveCursorL(const TChar& aChar);

        /**
         * CMessageContainer()
         * Default C++ constructor.
         * @param aReadOnly ETrue for read only container.
         */
        CMessageContainer( TBool aReadOnly );

        /**
         * ConstructL()
         * Default EPOC constructor.
         * @param aRect Frame rectangle for container.
         */
//        void ConstructL( const TRect& aRect );
        
     
    private: // enum
        
        enum TFocusedControl
            {
            EFocusedNumberEditor,
            EFocusedTextEditor
            };
        
    private: // data
        
        CEikLabel*                      iToFromLabel;
        CEikGlobalTextEditor*			iNumberEditor;
        CEikGlobalTextEditor*           iTextEditor;
        
        TFocusedControl                 iCurrentFocus;
        TBool                           iReadOnly;
        TBool                           iChanged;
        
        RArray<TBool>					iParagraphTypes;
        CAknsBasicBackgroundControlContext* iSkinContext;
        CAknsBasicBackgroundControlContext* iSkinContext2;
    };

#endif // CREATEMESSAGECONTAINER_H

// End of File
