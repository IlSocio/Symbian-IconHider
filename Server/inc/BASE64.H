
#ifndef __BASE64_H
#define __BASE64_H

#include <e32def.h>
#include <e32base.h>


const TInt KErrInvalidSource=(-1);
const TUint8 KErrInvalidCharacter=65;

enum TBase64Panic
	{
	EBase64Overflow
	};

class CBase64CodecBase : public CBase
	{
public:
	TUint Base64Char(TText aChar) const;
	TInt Decode(const TDesC& aSource, TDes& aResult);
	void Reset();
	TInt Encode(const TDesC& aSource, TDes& aResult) const;
private:
	TUint iQuart[4];
	TInt iQuartCount;
	};

#endif
