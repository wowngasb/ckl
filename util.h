#ifndef CKL_UTIL_H  
#define CKL_UTIL_H

#if __cplusplus
extern "C" {
#endif

	void ByteToHexStr(unsigned char *indata, int inlen, char *outdata, int *outlen, int upper);

	void ByteToHexStr_str(char *indata, char *outdata, int *outlen, int upper);

	void HexStrToByte(char *indata, int inlen, unsigned char *outdata, int *outlen);

	void HexStrToByte_str(char *indata, unsigned char *outdata, int *outlen);

	void md5_str(char *indata, char *outdata, int *outlen, int upper);

	void md5_str2(char *indata1, char *indata2, char *outdata, int *outlen, int upper);

	void md5_str3(char *indata1, char *indata2, char *indata3, char *outdata, int *outlen, int upper);

	void md5_byte3(unsigned char *indata1, int inlen1, unsigned char *indata2, int inlen2, unsigned char *indata3, int inlen3, char *outdata, int *outlen, int upper);

	void md5_str4(char *indata1, char *indata2, char *indata3, char *indata4, char *outdata, int *outlen, int upper);

	void decode(char* indata, int inlen, unsigned char* outdata, int *outlen, char* key, char* salt, int rnd_length, int chk_length);

	void ckl_decode(char* indata, int inlen, unsigned char* outdata, int *outlen, char* key);

	void encode(char* indata, int inlen, unsigned char* outdata, int *outlen, char* key, int expiry, char* salt, int rnd_length, int chk_length);

	void ckl_encode(char* indata, int inlen, unsigned char* outdata, int *outlen, char* key, int expiry);

	void rand_str(int len, char *output);

	unsigned int byteToInt32WithLittleEndian(unsigned char byte0, unsigned char byte1, unsigned char byte2, unsigned char byte3);

	unsigned int hexToInt32WithLittleEndian(char *indata);

	void int32ToByteWithLittleEndian(unsigned int ival, unsigned char *output);

	void int32ToHexWithLittleEndian(unsigned int ival, char *output, int *outlen, int upper);



	void authcode(int cmd, char* indata, int inlen, unsigned char* outdata, int *outlen, char* _key, unsigned int expiry, char* salt, int rnd_length, int chk_length);

	void encodeByXor(unsigned char* buf, int buflen, unsigned char* key, int keylen);

	unsigned long otfpn(unsigned long n, int k);
	unsigned long otklfindp(unsigned long n);
	unsigned long fpn(unsigned long n, int k);
	unsigned long klfindp(unsigned long n);
	unsigned long findp(unsigned long n);
	unsigned long findpEx(unsigned long n);

	int isPrime(unsigned long n);
	int klPrime(unsigned long n);

#if __cplusplus
}
#endif

#endif /* CKL_UTIL_H */