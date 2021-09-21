#ifndef __SERVER_PAN__
#define __SERVER_PAN__

enum TMyPanic
	{
	EPanicNoMemory,				// 0
	EPanicBadDescriptor,		// 1 
	EPanicIllegalFunction,		// 2
	EPanicInvalidLicense,		// 3
	EPanicLicenseNotFound,		// 4
	EPanicInvalidUID,			// 5
	EPanicAlreadyWaiting,		// 6
    ESrvCreateServer,			// 7
    ECreateTrapCleanup			// 8
	};

#endif


