#include "util.h"

#include <string.h>

#include "base64.h"
#include "md5.h"

#define _toupper(x) ( (x) >= 0x61 && (x) <= 0x7a ? (x) - 0x20 : (x) )


void ByteToHexStr(const unsigned char *indata, int inlen, char *outdata, int *outlen, int upper)
{
	short i;
	unsigned char highByte, lowByte;
	unsigned char offset = upper ? 0x07 : 0x27;

	if (!indata || inlen <= 0 || !outdata || !outlen) {
		return;
	}

	for (i = 0; i < inlen; i++)
	{
		highByte = indata[i] >> 4;
		lowByte = indata[i] & 0x0f;

		highByte += 0x30;
		outdata[i * 2] = highByte > 0x39 ? highByte + offset : highByte;

		lowByte += 0x30;
		outdata[i * 2 + 1] = lowByte > 0x39 ? lowByte + offset : lowByte;
	}

	*outlen = inlen * 2;
	return;
}

void ByteToHexStr_str(const char *indata, char *outdata, int *outlen, int upper)
{
	ByteToHexStr(indata, strlen(indata), outdata, outlen, upper);
	return;
}

void HexStrToByte(const char *indata, int inlen, unsigned char *outdata, int *outlen)
{
	short i;
	unsigned char highByte, lowByte;

	if (!indata || inlen <= 0 || inlen % 2 != 0 || !outdata || !outlen) {
		return;
	}

	for (i = 0; i < inlen; i += 2)
	{
		highByte = _toupper(indata[i]);
		lowByte = _toupper(indata[i + 1]);

		highByte -= highByte > 0x39 ? 0x37 : 0x30;
		lowByte -= lowByte > 0x39 ? 0x37 : 0x30;

		outdata[i / 2] = (highByte << 4) | lowByte;
	}

	*outlen = inlen / 2;
	return;
}

void HexStrToByte_str(const char *indata, unsigned char *outdata, int *outlen)
{
	HexStrToByte(indata, strlen(indata), outdata, outlen);
	return;
}

void md5_str(const char *indata, char *outdata, int *outlen, int upper) 
{
	MD5_CTX context;
	unsigned char digest[16] = { 0 };

	MD5Init(&context);
	MD5Update(&context, indata, strlen(indata));
	MD5Final(&context, digest);

	ByteToHexStr(digest, 16, outdata, outlen, upper);
	return;
}