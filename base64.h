#ifndef BASE64_H
#define BASE64_H

#if __cplusplus
extern "C" {
#endif

	void base64_encode(const unsigned char *indata, int inlen, char *outdata, int *outlen, int safe);

	void base64_encode_str(const char *indata, char *outdata, int *outlen, int safe);

	void base64_decode(const char *indata, int inlen, unsigned char *outdata, int *outlen, int safe);

	void base64_decode_str(const char *indata, int inlen, unsigned char *outdata, int *outlen, int safe);


#if __cplusplus
}
#endif

#endif /* BASE64_H */