/*
 * ============================================================================
 *  Name        : CCustomEdwinEditor from CustomEdwinEditor.h
 *  Part of     : SkebbyGui
 *  Created     : 07.01.2009 by BITWEEN S.R.L.
 *  Description : CCustomEdwinEditor declaration.
 *  Version     : 1.0
 *  Copyright   : BITWEEN S.R.L.
 * ============================================================================
 */

#ifndef CUSTOMEDWINEDITOR_H
#define CUSTOMEDWINEDITOR_H

// INCLUDES 
#include <eikedwin.h>                   // CEikEdwin
#include <EIKGTED.H>
#include <AKNSBASICBACKGROUNDCONTROLCONTEXT.H>

// LIBS
// eikcoctl.lib (CLafEdwinCustomDrawBase), uiklaf.lib (CLafEdwinCustomDrawBase)

// CAPS
// None

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * CCustomEdwinDrawer class.
 * CCustomEdwinDrawer declaration.
 * 
 */
class CCustomEdwinDrawer: public CLafEdwinCustomDrawBase
    {
    public: // Constructor
        
        /**
         * CCustomEdwinDrawer()
         * Default C++ Constructor.
         */
        CCustomEdwinDrawer( const MLafEnv & aEnv, const CCoeControl & aControl );

    public: // From CLafEdwinCustomDrawBase
        
        /**
         * DrawBackground()
         * Draws background of the text control.
         * @param aParam Contains the drawing parameters.
         * @param aBackground The default background colour.
         * @param aDrawn Must return the rectangle you actually draw.
         */
        void DrawBackground( const TParam& aParam,
                const TRgb& aBackground, TRect& aDrawn ) const;
        
    	void DrawLineGraphics(const TParam& aParam,const TLineInfo& aLineInfo) const;

    public:
    	CAknsBasicBackgroundControlContext* iBack;
    };

/**
 * CCustomEdwinEditor class.
 * CCustomEdwinEditor declaration.
 * 
 */
class CCustomEdwinEditor: public CEikGlobalTextEditor
    {
    public: // From CEikEdwin

    	CCustomEdwinEditor();
//    	static CCustomEdwinEditor* NewL();
    protected:
//    	void ContructL();
        /**
         * CreateCustomDrawL()
         * Creates custom drawer.
         * @return Pointer to the created custom drawer.
         */
        //CLafEdwinCustomDrawBase* CreateCustomDrawL();
    	
    private:
    	CAknsBasicBackgroundControlContext* iBack;
    };

#endif // CUSTOMEDWINEDITOR_H

// End of File
