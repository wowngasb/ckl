#include "util.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "base64.h"
#include "md5.h"

#define _toupper(x) ( (x) >= 0x61 && (x) <= 0x7a ? (x) - 0x20 : (x) )



const char *rand_base = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
const int rand_base_len = 62;

void rand_str(int len, char *output)
{
	int i;
	time_t t;

	if (len <= 0) {
		return;
	}

	/* 初始化随机数发生器 */
	srand((unsigned)time(&t));

	for (i = 0; i < len; ++i) {
		output[i] = rand_base[rand() % rand_base_len];
	}

	return;
}

void ByteToHexStr(unsigned char *indata, int inlen, char *outdata, int *outlen, int upper)
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

void ByteToHexStr_str(char *indata, char *outdata, int *outlen, int upper)
{
	ByteToHexStr(indata, (int)strlen(indata), outdata, outlen, upper);
	return;
}

void HexStrToByte(char *indata, int inlen, unsigned char *outdata, int *outlen)
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

void HexStrToByte_str(char *indata, unsigned char *outdata, int *outlen)
{
	HexStrToByte(indata, (int)strlen(indata), outdata, outlen);
	return;
}

void md5_str(char *indata, char *outdata, int *outlen, int upper) 
{
	MD5_CTX context;
	unsigned char digest[16] = { 0 };

	MD5Init(&context);
	MD5Update(&context, (unsigned char *)indata, (int)strlen(indata));
	MD5Final(&context, digest);

	ByteToHexStr(digest, 16, outdata, outlen, upper);
	return;
}

void md5_str2(char *indata1, char *indata2, char *outdata, int *outlen, int upper) {
	MD5_CTX context;
	unsigned char digest[16] = { 0 };

	MD5Init(&context);
	MD5Update(&context, (unsigned char *)indata1, (int)strlen(indata1));
	MD5Update(&context, (unsigned char *)indata2, (int)strlen(indata2));
	MD5Final(&context, digest);

	ByteToHexStr(digest, 16, outdata, outlen, upper);
	return;
}

void md5_str3(char *indata1, char *indata2, char *indata3, char *outdata, int *outlen, int upper) {
	MD5_CTX context;
	unsigned char digest[16] = { 0 };

	MD5Init(&context);
	MD5Update(&context, (unsigned char *)indata1, (int)strlen(indata1));
	MD5Update(&context, (unsigned char *)indata2, (int)strlen(indata2));
	MD5Update(&context, (unsigned char *)indata3, (int)strlen(indata3));
	MD5Final(&context, digest);

	ByteToHexStr(digest, 16, outdata, outlen, upper);
	return;
}

void md5_byte3(unsigned char *indata1, int inlen1, unsigned char *indata2, int inlen2, unsigned char *indata3, int inlen3, char *outdata, int *outlen, int upper)
{
	MD5_CTX context;
	unsigned char digest[16] = { 0 };

	MD5Init(&context);
	MD5Update(&context, indata1, inlen1);
	MD5Update(&context, indata2, inlen2);
	MD5Update(&context, indata3, inlen3);
	MD5Final(&context, digest);

	ByteToHexStr(digest, 16, outdata, outlen, upper);
	return;
}

void md5_str4(char *indata1, char *indata2, char *indata3, char *indata4, char *outdata, int *outlen, int upper) {
	MD5_CTX context;
	unsigned char digest[16] = { 0 };

	MD5Init(&context);
	MD5Update(&context, (unsigned char *)indata1, (int)strlen(indata1));
	MD5Update(&context, (unsigned char *)indata2, (int)strlen(indata2));
	MD5Update(&context, (unsigned char *)indata3, (int)strlen(indata3));
	MD5Update(&context, (unsigned char *)indata4, (int)strlen(indata4));
	MD5Final(&context, digest);

	ByteToHexStr(digest, 16, outdata, outlen, upper);
	return;
}

void decode(char* indata, int inlen, unsigned char* outdata, int *outlen, char* key, char* salt, int rnd_length, int chk_length)
{
	authcode(1, indata, inlen, outdata, outlen, key, 0, salt, rnd_length, chk_length);
	return;
}

void ckl_decode(char* indata, int inlen, unsigned char* outdata, int *outlen, char* key) {
	authcode(1, indata, inlen, outdata, outlen, key, 0, "salt", 2, 4);
	return;
}

void encode(char* indata, int inlen, unsigned char* outdata, int *outlen, char* key, int expiry, char* salt, int rnd_length, int chk_length)
{
	authcode(0, indata, inlen, outdata, outlen, key, expiry, salt, rnd_length, chk_length);
	return;
}

void ckl_encode(char* indata, int inlen, unsigned char* outdata, int *outlen, char* key, int expiry) {
	authcode(0, indata, inlen, outdata, outlen, key, expiry, "salt", 2, 4);
	return;
}


unsigned int byteToInt32WithLittleEndian(unsigned char byte0, unsigned char byte1, unsigned char byte2, unsigned char byte3)
{
	return (byte3 << 24) + (byte2 << 16) + (byte1 << 8) + byte0;
}

unsigned int hexToInt32WithLittleEndian(char *indata)
{
	int retint;
	unsigned char tmp[4] = { 0 };

	if (strlen(indata) > 8) {
		return 0;
	}
	HexStrToByte_str(indata, tmp, &retint);
	return byteToInt32WithLittleEndian(tmp[0], tmp[1], tmp[2], tmp[3]);
}

void int32ToByteWithLittleEndian(unsigned int ival, unsigned char *output)
{
	output[0] = (ival << 24) >> 24;
	output[1] = (ival << 16) >> 24;
	output[2] = (ival << 8) >> 24;
	output[3] = ival >> 24;
	return ;
}

void int32ToHexWithLittleEndian(unsigned int ival, char *output, int *outlen, int upper)
{
	unsigned char retstr[4] = { 0 };

	int32ToByteWithLittleEndian(ival, retstr);
	ByteToHexStr(retstr, 4, output, outlen, upper);
	return;
}

void authcode(int cmd, char* indata, int inlen, unsigned char* outdata, int *outlen, char* _key, unsigned int expiry, char* salt, int rnd_length, int chk_length)
{
	unsigned char *tmpbuf;
	int tmpint, cryptlen, retlen;
	unsigned int expiry_t;
	char key[33] = { 0 };
	char keya[33] = { 0 };
	char keyb[33] = { 0 };
	char keyc[33] = { 0 };
	char crypt[65] = { 0 };
	char checksum[33] = { 0 };
	char tmp_sum[33] = { 0 };
	unsigned int timestamp = (unsigned int)time(NULL);
	int pre_len = 4 + chk_length;

	rnd_length = rnd_length > 0 ? rnd_length : 0;
	expiry = expiry > 0 ? expiry : 0;
	chk_length = chk_length > 4 ? (chk_length < 16 ? chk_length : 16) : 4;

	md5_str3(salt, _key, "origin key", key, &tmpint, 0);
	key[tmpint] = '\0';
	
	// 密匙a会参与加解密
	tmpint = 16;
	memcpy(keya, key, tmpint);
	keya[tmpint] = '\0';
	md5_str3(salt, keya, "key a for crypt", keya, &tmpint, 0);
	keya[tmpint] = '\0';

	// 密匙b会用来做数据完整性验证
	tmpint = 16;
	memcpy(keyb, key + 16, tmpint);
	keyb[tmpint] = '\0';
	md5_str3(salt, keyb, "key b for check sum", keyb, &tmpint, 0);
	keyb[tmpint] = '\0';

	if (cmd) {
		if (rnd_length > 0) {
			// 密匙c用于变化生成的密文
			memcpy(keyc, indata, rnd_length);
			keyc[rnd_length] = '\0';
		}
		// 参与运算的密匙
		tmpint = 32;
		memcpy(crypt, keya, tmpint);
		crypt[tmpint] = '\0';
		md5_str4(salt, keya, keyc, "merge key a and key c", crypt + 32, &tmpint, 0);
		crypt[32 + tmpint] = '\0';
		cryptlen = 32 + tmpint;

		tmpbuf = (unsigned char *)malloc( (inlen - rnd_length) * 3 / 4 + 3 );
		// 解码，会从第 $keyc_length Byte开始，因为密文前 $keyc_length Byte保存 动态密匙

		base64_decode(indata + rnd_length, inlen - rnd_length, tmpbuf, &retlen, 1);
		
		encodeByXor(tmpbuf, retlen, crypt, cryptlen);
		
		// 验证数据有效性
		expiry_t = retlen >= 4 ? byteToInt32WithLittleEndian(tmpbuf[0], tmpbuf[1], tmpbuf[2], tmpbuf[3]) : 0;
		
		if (retlen >= pre_len) {
			ByteToHexStr(tmpbuf + 4, chk_length, checksum, &tmpint, 0);
			checksum[tmpint] = '\0';
			md5_byte3(salt, (int)strlen(salt), tmpbuf + pre_len, retlen - pre_len, keyb, (int)strlen(keyb), tmp_sum, &tmpint, 0);
			tmp_sum[2 * chk_length] = '\0';
			if ((expiry_t == 0 || expiry_t > timestamp) && strcmp(checksum, tmp_sum) == 0) {
				*outlen = retlen - pre_len;
				memcpy(outdata, tmpbuf + pre_len, retlen - pre_len);
			}
		}
		free(tmpbuf);
		return ;
	}
	else {
		if (rnd_length > 0) {
			rand_str(rnd_length, keyc);
			keyc[rnd_length] = '\0';
		}
		md5_byte3(salt, (int)strlen(salt), indata, inlen, keyb, (int)strlen(keyb), checksum, &tmpint, 0);
		checksum[2 * chk_length] = '\0';
		expiry_t = expiry > 0 ? expiry + timestamp : 0;

		// 参与运算的密匙
		tmpint = 32;
		memcpy(crypt, keya, tmpint);
		crypt[tmpint] = '\0';
		md5_str4(salt, keya, keyc, "merge key a and key c", crypt + 32, &tmpint, 0);
		crypt[32 + tmpint] = '\0';
		cryptlen = 32 + tmpint;

		tmpbuf = (unsigned char *)malloc(inlen + pre_len);

		// 加密，原数据补充附加信息，共 8byte  前 4 Byte 用来保存时间戳，后 chk_length Byte 用来保存 $checksum 解密时验证数据完整性
		int32ToByteWithLittleEndian(expiry_t, tmpbuf);
		HexStrToByte_str(checksum, tmpbuf + 4, &tmpint);
		memcpy(tmpbuf + pre_len, indata, inlen);

		encodeByXor(tmpbuf, inlen + pre_len, crypt, cryptlen);
		
		memcpy(outdata, keyc, rnd_length);
		base64_encode(tmpbuf, inlen + pre_len, outdata + rnd_length, &retlen, 1);
		
		*outlen = retlen + rnd_length;
		free(tmpbuf);
		return;
	}
}

void encodeByXor(unsigned char* buf, int buflen, unsigned char* key, int keylen)
{
	int i, j, k;
	unsigned char tmpc;
	unsigned char box[256] = {0};
	unsigned char boxk[256] = { 0 };
	// 产生密匙簿
	for (i = 0; i < 256; ++i) {
		boxk[i] = key[i % keylen];
		box[i] = (unsigned char)i;
	}

	for (j = i = 0; i < 256; ++i) {
		j = (i + j + box[i] + box[j] + boxk[i] + boxk[j]) % 256;
		tmpc = box[i];
		box[i] = box[j];
		box[j] = tmpc;
	}

	for (k = j = i = 0; i < buflen; ++i) {
		k = (k + 1) % 256;
		j = (j + box[k]) % 256;
		tmpc = box[k];
		box[k] = box[j];
		box[j] = tmpc;

		buf[i] = buf[i] ^ box[(box[k] + box[j]) % 256];
	}

	return ;
}

