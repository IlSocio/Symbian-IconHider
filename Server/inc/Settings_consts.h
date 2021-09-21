#ifndef SETTINGS_CONSTS_H
#define SETTINGS_CONSTS_H

#include <e32std.h>
#include <e32base.h>

const TUid KServerUid3={0x2005E763}; 
const TUint KPSChangeConfig=1;

_LIT(KSETTINGS_VERSION, "0.2");
_LIT(KGUI_VERSION, "1.03.0000");

#define KPASS_LEN 8
#define KIMEI_LEN 20
#define KNAME_LEN 12

typedef TBuf<KNAME_LEN> TProfileName;
typedef TBuf<KIMEI_LEN> TImei;
typedef TBuf<KPASS_LEN>	TPassCode;

#endif
