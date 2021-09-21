//#define _LOGGING

#ifndef __T_SERVER_H__
#define __T_SERVER_H__

#include "clientserver.h"
#include "Server.pan"
#include "client.h"
#include "Data.h"

#include <FuzzyByte\timeouttimer.h>
#include <FuzzyByte\FileLogger.h>
#include <FuzzyByte\phone.h>

#include <e32base.h>
#include <f32file.h>
#include <ES_SOCK.H>



class CMyServer : public CServer2, public MTimeOutNotifier
	{
public:
	static CMyServer* NewLC();
	virtual ~CMyServer();
	static TInt EntryPoint(TAny* aStarted);
	void AddSession();
	void DropSession();
	void NotifyChange(TAny* aSource, const TConfigChangeEvent aChangeCode);
	void SaveSettingsL();
	void TimerExpiredL(TAny* src);
private:
	CMyServer(TInt aPriority);
	void ConstructL();
	CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;
	static void ThreadMainL();

public:
	RFs iFs;
	CPhone* iPhone;
	CData* iSettings;
	TImei iImei;
	TInt iSessionCount;
	TBool hasChanged;

private:
	CTimeOutTimer* iShutdown;
	__LOG_DECLARATION_MEMBER
	};

#endif

