#ifndef CKL_UTIL_H  
#define CKL_UTIL_H

#if __cplusplus
extern "C" {
#endif

	enum AuthCodeActionEnum
	{
		AuthCode_ENCODE = 0,
		AuthCode_DECODE,
	};

	void ByteToHexStr(const unsigned char *indata, int inlen, char *outdata, int *outlen, int upper);

	void ByteToHexStr_str(const char *indata, char *outdata, int *outlen, int upper);

	void HexStrToByte(const char *indata, int inlen, unsigned char *outdata, int *outlen);

	void HexStrToByte_str(const char *indata, unsigned char *outdata, int *outlen);

	void md5_str(const char *indata, char *outdata, int *outlen, int upper);

#if __cplusplus
}
#endif

#endif /* CKL_UTIL_H */