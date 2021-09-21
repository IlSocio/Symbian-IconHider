/*
* ==============================================================================
*  Name        : BrowserOverriddenSettings.h
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

#ifndef BROWSER_OVERRIDDEN_SETTINGS_H
#define BROWSER_OVERRIDDEN_SETTINGS_H

//  INCLUDES
#include <e32std.h>
#include <e32def.h>

// DATA TYPES
typedef enum
{
    EBrowserOverSettingsSmallScreen = 0,
    EBrowserOverSettingsAutoLoadImages,
    EBrowserOverSettingsFontSize,
    EBrowserOverSettingsFullScreen,
    EBrowserOverSettingsCustomAp,
    EBrowserOverSettingsUndefinied = KMaxTUint16
} TBrowserOverSettings;

typedef enum 
{
  EBrowserOverFontSizeLevelAllSmall = 0,
  EBrowserOverFontSizeLevelSmaller,
  EBrowserOverFontSizeLevelNormal,
  EBrowserOverFontSizeLevelLarger,    
  EBrowserOverFontSizeLevelAllLarge,
  EBrowserOverFontSizeLevelUndefined = KMaxTUint16
} TBrowserOverFontSizeLevel;

typedef enum 
{
  EBrowserOverFullScreenValueNormal = 0,
  EBrowserOverFullScreenValueSoftKeysOnly,
  EBrowserOverFullScreenValueFullScreen,
  EBrowserOverFullScreenValueUndefined = KMaxTUint16
} TBrowserOverFullScreenValue;


// CLASS DECLARATION

/**
*  This class stores values to pass to the browser in embedded mode
*
*/
class TBrowserOverriddenSettings
{
public :// Constructors and destructor 
   inline TBrowserOverriddenSettings();

public: // New functions
   /**
   * SetBrowserSetting sets the the settings which will be overriden
   * @param aBrowserSetting describes which type of setting to set
   * @param aValue the value to set
   * @return void
   */
   inline void SetBrowserSetting(TBrowserOverSettings aBrowserSetting, TUint aValue);
   /**
   * GetBrowserSetting gets the value of the setting
   * @param aBrowserSetting describes which type of setting to get
   * @return the value of the setting
   */
   inline TUint GetBrowserSetting(TBrowserOverSettings aBrowserSetting);
private:// Data
   //stores the value for the small screen setting can be 0 or 1
   //if not set then KMaxUint
   TUint iSmallScreen;
   //stores the value for the auto load image setting can be 0 or 1
   //if not set then KMaxUint
   TUint iAutoLoadImage;
   //stores the value for the font size setting can be 0,1,2,3 or 4
   TUint iFontSize;
   //stores the value for the small screen setting can be 0 or 1
   TUint iFullScreen;
   // Custom access point provided by the operator. If specified, contains an id of a read only
   // access point what is then set as default  and it cannot be changed manually. If not 
   // defined, operator menu works as before.
   // from operator menu( 0 - not specified, otherwise contains the access point id)
   TUint iCustomAp;
};

#include <BrowserOverriddenSettings.inl>

#endif  //BROWSER_OVERRIDDEN_SETTINGS_H