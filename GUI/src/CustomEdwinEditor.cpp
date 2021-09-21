/*
 * ============================================================================
 *  Name        : CCustomEdwinEditor from CustomEdwinEditor.cpp
 *  Part of     : SkebbyGui
 *  Created     : 07.01.2009 by BITWEEN S.R.L.
 *  Description : CCustomEdwinEditor implementation.
 *  Version     : 1.0
 *  Copyright   : BITWEEN S.R.L.
 * ============================================================================
 */

// INCLUDE FILES 
#include "CustomEdwinEditor.h"          // CCustomEdwinEditor

#include <eikenv.h>                     // iEikonEnv
#include <aknutils.h>                   // LatinPlain12
#include <AknsDrawUtils.h>
#include <AknsSkinInstance.h>

// ================= MEMBER FUNCTIONS ========================================
//
// ---------------------------------------------------------------------------
// CCustomEdwinDrawer::CCustomEdwinDrawer()
// Default C++ Constructor.
// ---------------------------------------------------------------------------
//
CCustomEdwinDrawer::CCustomEdwinDrawer( const MLafEnv & aEnv,
        const CCoeControl& aControl ) : CLafEdwinCustomDrawBase( aEnv, aControl )
    {
    }



void CCustomEdwinDrawer::DrawLineGraphics(const TParam& aParam,const TLineInfo& aLineInfo) const
	{
	// TODO: Al piu' usalo per rilevare la grandezza delle righe... non sembra andare bene perche' per la prima riga restutisce 77 per le altre +35
	// TODO: Vedi da dove si puo' ricavare quel +35... Forse si riesce a farlo spuntare fuori da altro...
	TBuf<50> buf;
	buf = _L("**** DrawLineGraphics ");
	buf.AppendNum( aLineInfo.iInnerRect.iBr.iY );
	RDebug::Print(buf);
/*	// disegna una linea fino a width - x 	
    TRect baseRect = iControl.Rect();
	TPoint start( aLineInfo.iInnerRect.iTl.iX, aLineInfo.iInnerRect.iTl.iY );
	TPoint end( baseRect.iBr.iX - aLineInfo.iInnerRect.iTl.iX,  aLineInfo.iInnerRect.iTl.iY);
    aParam.iGc.SetPenColor( KRgbBlack );
    aParam.iGc.DrawLine( start, end );
    
	TPoint start2( aLineInfo.iInnerRect.iTl.iX, aLineInfo.iInnerRect.iBr.iY  + 20);
	TPoint end2( 100,  aLineInfo.iInnerRect.iBr.iY + 20);
    aParam.iGc.SetPenColor( KRgbBlack );
    aParam.iGc.DrawLine( start2, end2 );*/
    }

// ---------------------------------------------------------------------------
// CCustomEdwinDrawer::DrawBackground()
// Draws background of the text control.
// ---------------------------------------------------------------------------
//
void CCustomEdwinDrawer::DrawBackground( const TParam& aParam,
        const TRgb& aBackground, TRect& aDrawn ) const
    {
    // Tutte le righe vengono disegnate qui...
	RDebug::Print(_L("**** DrawBackground"));
/*	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
   	MAknsControlContext* context = AknsDrawUtils::ControlContext( &iControl );
//	MAknsControlContext* context = AknsDrawUtils::ControlContext( prov );

   	CWindowGc& gc = iControl.SystemGc();
	// Determina l'area da disegnare...
   	AknsDrawUtils::Background(skin, context, &iControl, gc, iControl.Rect());
*/    	
    	/*    CLafEdwinCustomDrawBase::DrawBackground( aParam, aBackground, aDrawn );

    aParam.iGc.SetPenColor( TRgb( 206, 207, 255 ) );

    TRect baseRect = iControl.Rect();
    
    const CFont* font = LatinPlain12();
    TInt h = font->HeightInPixels() + font->AscentInPixels() - 2;
    for ( TInt i = 0; i < 6; i++ )
        {
        TInt y = baseRect.iTl.iY + ( i + 1 ) * h - 4 + iCount;
        aParam.iGc.DrawLine( TPoint( 1, y ), TPoint( baseRect.Width() - 2, y ) );
        }*/
    }



// ---------------------------------------------------------------------------
// CCustomEdwinEditor::CreateCustomDrawL()
// Creates custom drawer.
// ---------------------------------------------------------------------------
//
/*CLafEdwinCustomDrawBase* CCustomEdwinEditor::CreateCustomDrawL()
    {
    return new ( ELeave ) CCustomEdwinDrawer( iEikonEnv->LafEnv(), *this );
    }*/

CCustomEdwinEditor::CCustomEdwinEditor() : CEikGlobalTextEditor()
	{
	}

/*CCustomEdwinEditor* CCustomEdwinEditor::NewL()
	{	
	CCustomEdwinEditor* self = new (ELeave) CCustomEdwinEditor();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}*/

/*void CCustomEdwinEditor::InitBackL()
	{
	iBack = CAknsBasicBackgroundControlContext::NewL(KAknsIIDQsnBgScreen ,iControl.Rect(), EFalse);//KAknsIIDQsnBgScreenIdle
	}*/

    	
    	
// End of File
