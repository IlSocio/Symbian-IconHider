Browser Laucher API

Installation to S60 3rd Ed. Maintenance Release.

Copy this archive to EPOCROOT and unzip. All files listed below will be 
installed.

API is used through these headers:
epoc32\include\browserlauncher.h
epoc32\include\browseroverriddensettings.h
epoc32\include\browseroverriddensettings.inl
epoc32\include\downloadedcontenthandler.h

Libraries related to this API:
epoc32\release\armv5\lib\browserlauncher.dso
epoc32\release\armv5\lib\browserlauncher.lib
epoc32\release\armv5\lib\browserlauncher{000a0000}.dso
epoc32\release\armv5\lib\browserlauncher{000a0000}.lib

API documentation can be found in PartnerAPIDoc\BrowserLauncher\index.html. 
README.txt can be found in PartnerAPIDoc\BrowserLauncher\README.txt.

Please notice the following issues with this API:
1. Nokia is not giving any binary compatibility promise for this API; neither between platform versions or within a platform version.
2. You must test your application with each device that the application can be installed on. This is needed to ensure the best possible user experience. To prevent application installation on other than tested devices, define the Product ID(s) in the .pkg file. More information can be found in the document "S60 Platform: Identification Codes" available on the Forum Nokia Web site.

Copyright (c) 2001-2007 Nokia Corporation. All rights reserved. Nokia and Nokia Connecting People are registered trademarks of Nokia Corporation.