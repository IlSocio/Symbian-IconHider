//#define _LOGGING

#ifndef __T_SESSION_H__
#define __T_SESSION_H__

#include <e32base.h>
#include "server.pan"

//class CLicense;
class CMyServer;

void PanicClient(const RMessagePtr2& aMessage, TMyPanic TMyPanic);


#include <FuzzyByte\FileLogger.h>

class CMySession : public CSession2
	{
public:
	CMySession();
	void CreateL();
//	void DoNotifyChange(TConfigChangeEvent aChangeCode); // Notifica dal Server al Client
private:
	TUint ClientUID(const RMessagePtr2& aMessage);
	void SetPasswordL(const RMessage2& aMessage);
	void ImeiL(const RMessage2& aMessage);
	void SetEnabledL(const RMessage2& aMessage);
	void IsEnabledL(const RMessage2& aMessage);
	void PasswordL(const RMessage2& aMessage);
	void GetRegCodeL(const RMessage2& aMessage);
	void RegisterL(const RMessage2& aMessage);

	void GetProtectedListL(const RMessage2& aMessage);
	void AddProtectedUidL(const RMessage2& aMessage);
	void RemProtectedUidL(const RMessage2& aMessage);

	void GetVersionL(const RMessage2& aMessage);

	virtual ~CMySession();
	inline CMyServer& Server();
	void ServiceL(const RMessage2& aMessage);
	void ServiceError(const RMessage2& aMessage, TInt aError);

	inline TBool NotificationPending() const;
// Forse non serve piu' perche' c'e' ServiceError ad occuparsene...
//	void DispatchMessageL(const RMessage2& aMessage);
private:
	TInt iProcessedUid;
	__LOG_DECLARATION_MEMBER
	};


#endif

