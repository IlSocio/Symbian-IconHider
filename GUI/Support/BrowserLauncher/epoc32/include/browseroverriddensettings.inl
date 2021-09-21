/*
* ==============================================================================
*  Name        : BrowserOverriddenSettings.inl
*  Part of     : Browser / BrowserLauncher
*  Description : enables other applications to launch browser in embedded mode
*  Version     : 1.0
*
*  Copyright (c) 2002 Nokia Corporation.
*  This material, including documentation and any related 
*  computer programs, is protected by copyright controlled by 
*  Nokia Corporation. All rights are reserved. Copying, 
*  including reproducing, storing, adapting or translating, any 
*  or all of this material requires the prior written consent of 
*  Nokia Corporation. This material also contains confidential 
*  information which may not be disclosed to others without the 
*  prior written consent of Nokia Corporation.
* ==============================================================================
*/

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TBrowserOverriddenSettings::TBrowserOverriddenSettings
// constructor sets the values to undefinied
// -----------------------------------------------------------------------------
inline TBrowserOverriddenSettings::TBrowserOverriddenSettings()
    {
         iSmallScreen = KMaxTUint;
         iAutoLoadImage = KMaxTUint;
         iFontSize = EBrowserOverFontSizeLevelUndefined;
         iFullScreen = EBrowserOverFullScreenValueUndefined;
         iCustomAp = 0;
    };

// -----------------------------------------------------------------------------
// TBrowserOverriddenSettings::SetBrowserSetting
// SetBrowserSetting sets the the settings which will be overriden
// Returns: void
// -----------------------------------------------------------------------------
inline void TBrowserOverriddenSettings::SetBrowserSetting(TBrowserOverSettings aBrowserSetting, TUint aValue)
   {
        switch( aBrowserSetting )
            {
            case EBrowserOverSettingsSmallScreen :
                {
                iSmallScreen = aValue;
                break;
                }
            case EBrowserOverSettingsAutoLoadImages :
                {
                iAutoLoadImage = aValue;
                break;
                }
            case EBrowserOverSettingsFontSize :
                {
                iFontSize = aValue;
                break;
                }
            case EBrowserOverSettingsFullScreen :
                {
                iFullScreen = aValue;
                break;
                }
            case EBrowserOverSettingsCustomAp :
                {
                iCustomAp = aValue;
                break;
                }
            default :
                {
                return;
                }
            }
   };

// -----------------------------------------------------------------------------
// TBrowserOverriddenSettings::GetBrowserSetting
// Gets the values of a specified type of setting
// Returns: the value of the specified setting
// -----------------------------------------------------------------------------
inline TUint TBrowserOverriddenSettings::GetBrowserSetting(TBrowserOverSettings aBrowserSetting)
   {
        switch( aBrowserSetting )
            {
            case EBrowserOverSettingsSmallScreen :
                {
                return iSmallScreen;
                break;
                }
            case EBrowserOverSettingsAutoLoadImages :
                {
                return iAutoLoadImage;
                break;
                }
            case EBrowserOverSettingsFontSize :
                {
                return iFontSize;
                break;
                }
            case EBrowserOverSettingsFullScreen :
                {
                return iFullScreen;
                break;
                }
            case EBrowserOverSettingsCustomAp :
                {
                return iCustomAp;
                break;
                }
            default :
                {
	            return KMaxTUint16;
                }
        }
   } 	
