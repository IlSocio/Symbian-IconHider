
#ifndef DATA_CONSTS_20024264_H
#define DATA_CONSTS_20024264_H

#include <e32std.h>
#include <e32base.h>

const TUid KDataUid3={0x20024264}; 
const TUint KPSChangeConfig=1;


enum TStatusValue
	{
	EAntiHack0,
	EAlways,
	EAntiHack2,
	ENever,
	EOnSimChange,
	EAntiHack5,
	EAntiHack6,
	EAuto
	};


// #include <etel3rdparty.h>
// KPhoneSerialNumberSize 50 (imei)  --> 20
// KMaxTelNumberSize 100 (numero)    --> 25
// KIMSISize 15 (imsi)
// KNetworkLongNameSize 16 (oper)
// KNetworkCountryCodeSize 4 (country)

#define KMSG_LEN 38
#define KNUMBER_LEN 25		
#define KPASS_LEN 8
#define KIMEI_LEN 20
#define KIMSI_LEN 15
#define KOPER_LEN 16
#define KNAME_LEN 12
#define KNETWORK_LEN 8
#define KCOUNTRY_LEN 4

typedef TBuf<KMSG_LEN> TMessage;
typedef TBuf<KIMEI_LEN> TImei;
typedef TBuf<KIMSI_LEN> TImsi;
typedef TBuf<KNAME_LEN> TImsiName;
typedef TBuf<KOPER_LEN> TOperName;
typedef TBuf<KPASS_LEN>	 TPassCode;
typedef TBuf<KNUMBER_LEN> TNumber;
typedef TBuf<KNETWORK_LEN> TNetwork;
typedef TBuf<KCOUNTRY_LEN> TCountry;

#endif
