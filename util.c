#include "util.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
	return;
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

		tmpbuf = (unsigned char *)malloc((inlen - rnd_length) * 3 / 4 + 3);
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
		return;
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
	unsigned char box[256] = { 0 };
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

	return;
}


const unsigned long ppp[] = { 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,
719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997,1009,1013,1019,1021,1031,1033,1039,1049,1051,1061,1063,1069,1087,1091,1093,1097,1103,1109,1117,1123,1129,1151,1153,1163,1171,1181,1187,1193,1201,1213,1217,1223,1229,1231,1237,1249,1259,1277,1279,1283,1289,1291,1297,1301,1303,1307,1319,1321,1327,1361,1367,1373,1381,1399,1409,1423,1427,1429,1433,1439,1447,1451,1453,1459,1471,1481,1483,1487,
1489,1493,1499,1511,1523,1531,1543,1549,1553,1559,1567,1571,1579,1583,1597,1601,1607,1609,1613,1619,1621,1627,1637,1657,1663,1667,1669,1693,1697,1699,1709,1721,1723,1733,1741,1747,1753,1759,1777,1783,1787,1789,1801,1811,1823,1831,1847,1861,1867,1871,1873,1877,1879,1889,1901,1907,1913,1931,1933,1949,1951,1973,1979,1987,1993,1997,1999,2003,2011,2017,2027,2029,2039,2053,2063,2069,2081,2083,2087,2089,2099,2111,2113,2129,2131,2137,2141,2143,2153,2161,2179,2203,2207,2213,2221,2237,2239,2243,2251,2267,2269,
2273,2281,2287,2293,2297,2309,2311,2333,2339,2341,2347,2351,2357,2371,2377,2381,2383,2389,2393,2399,2411,2417,2423,2437,2441,2447,2459,2467,2473,2477,2503,2521,2531,2539,2543,2549,2551,2557,2579,2591,2593,2609,2617,2621,2633,2647,2657,2659,2663,2671,2677,2683,2687,2689,2693,2699,2707,2711,2713,2719,2729,2731,2741,2749,2753,2767,2777,2789,2791,2797,2801,2803,2819,2833,2837,2843,2851,2857,2861,2879,2887,2897,2903,2909,2917,2927,2939,2953,2957,2963,2969,2971,2999,3001,3011,3019,3023,3037,3041,3049,3061,
3067,3079,3083,3089,3109,3119,3121,3137,3163,3167,3169,3181,3187,3191,3203,3209,3217,3221,3229,3251,3253,3257,3259,3271,3299,3301,3307,3313,3319,3323,3329,3331,3343,3347,3359,3361,3371,3373,3389,3391,3407,3413,3433,3449,3457,3461,3463,3467,3469,3491,3499,3511,3517,3527,3529,3533,3539,3541,3547,3557,3559,3571,3581,3583,3593,3607,3613,3617,3623,3631,3637,3643,3659,3671,3673,3677,3691,3697,3701,3709,3719,3727,3733,3739,3761,3767,3769,3779,3793,3797,3803,3821,3823,3833,3847,3851,3853,3863,3877,3881,3889,
3907,3911,3917,3919,3923,3929,3931,3943,3947,3967,3989,4001,4003,4007,4013,4019,4021,4027,4049,4051,4057,4073,4079,4091,4093,4099,4111,4127,4129,4133,4139,4153,4157,4159,4177,4201,4211,4217,4219,4229,4231,4241,4243,4253,4259,4261,4271,4273,4283,4289,4297,4327,4337,4339,4349,4357,4363,4373,4391,4397,4409,4421,4423,4441,4447,4451,4457,4463,4481,4483,4493,4507,4513,4517,4519,4523,4547,4549,4561,4567,4583,4591,4597,4603,4621,4637,4639,4643,4649,4651,4657,4663,4673,4679,4691,4703,4721,4723,4729,4733,4751,
4759,4783,4787,4789,4793,4799,4801,4813,4817,4831,4861,4871,4877,4889,4903,4909,4919,4931,4933,4937,4943,4951,4957,4967,4969,4973,4987,4993,4999,5003,5009,5011,5021,5023,5039,5051,5059,5077,5081,5087,5099,5101,5107,5113,5119,5147,5153,5167,5171,5179,5189,5197,5209,5227,5231,5233,5237,5261,5273,5279,5281,5297,5303,5309,5323,5333,5347,5351,5381,5387,5393,5399,5407,5413,5417,5419,5431,5437,5441,5443,5449,5471,5477,5479,5483,5501,5503,5507,5519,5521,5527,5531,5557,5563,5569,5573,5581,5591,5623,5639,5641,
5647,5651,5653,5657,5659,5669,5683,5689,5693,5701,5711,5717,5737,5741,5743,5749,5779,5783,5791,5801,5807,5813,5821,5827,5839,5843,5849,5851,5857,5861,5867,5869,5879,5881,5897,5903,5923,5927,5939,5953,5981,5987,6007,6011,6029,6037,6043,6047,6053,6067,6073,6079,6089,6091,6101,6113,6121,6131,6133,6143,6151,6163,6173,6197,6199,6203,6211,6217,6221,6229,6247,6257,6263,6269,6271,6277,6287,6299,6301,6311,6317,6323,6329,6337,6343,6353,6359,6361,6367,6373,6379,6389,6397,6421,6427,6449,6451,6469,6473,6481,6491,
6521,6529,6547,6551,6553,6563,6569,6571,6577,6581,6599,6607,6619,6637,6653,6659,6661,6673,6679,6689,6691,6701,6703,6709,6719,6733,6737,6761,6763,6779,6781,6791,6793,6803,6823,6827,6829,6833,6841,6857,6863,6869,6871,6883,6899,6907,6911,6917,6947,6949,6959,6961,6967,6971,6977,6983,6991,6997,7001,7013,7019,7027,7039,7043,7057,7069,7079,7103,7109,7121,7127,7129,7151,7159,7177,7187,7193,7207,7211,7213,7219,7229,7237,7243,7247,7253,7283,7297,7307,7309,7321,7331,7333,7349,7351,7369,7393,7411,7417,7433,7451,
7457,7459,7477,7481,7487,7489,7499,7507,7517,7523,7529,7537,7541,7547,7549,7559,7561,7573,7577,7583,7589,7591,7603,7607,7621,7639,7643,7649,7669,7673,7681,7687,7691,7699,7703,7717,7723,7727,7741,7753,7757,7759,7789,7793,7817,7823,7829,7841,7853,7867,7873,7877,7879,7883,7901,7907,7919,7927,7933,7937,7949,7951,7963,7993,8009,8011,8017,8039,8053,8059,8069,8081,8087,8089,8093,8101,8111,8117,8123,8147,8161,8167,8171,8179,8191,8209,8219,8221,8231,8233,8237,8243,8263,8269,8273,8287,8291,8293,8297,8311,8317,
8329,8353,8363,8369,8377,8387,8389,8419,8423,8429,8431,8443,8447,8461,8467,8501,8513,8521,8527,8537,8539,8543,8563,8573,8581,8597,8599,8609,8623,8627,8629,8641,8647,8663,8669,8677,8681,8689,8693,8699,8707,8713,8719,8731,8737,8741,8747,8753,8761,8779,8783,8803,8807,8819,8821,8831,8837,8839,8849,8861,8863,8867,8887,8893,8923,8929,8933,8941,8951,8963,8969,8971,8999,9001,9007,9011,9013,9029,9041,9043,9049,9059,9067,9091,9103,9109,9127,9133,9137,9151,9157,9161,9173,9181,9187,9199,9203,9209,9221,9227,9239,
9241,9257,9277,9281,9283,9293,9311,9319,9323,9337,9341,9343,9349,9371,9377,9391,9397,9403,9413,9419,9421,9431,9433,9437,9439,9461,9463,9467,9473,9479,9491,9497,9511,9521,9533,9539,9547,9551,9587,9601,9613,9619,9623,9629,9631,9643,9649,9661,9677,9679,9689,9697,9719,9721,9733,9739,9743,9749,9767,9769,9781,9787,9791,9803,9811,9817,9829,9833,9839,9851,9857,9859,9871,9883,9887,9901,9907,9923,9929,9931,9941,9949,9967,9973 };

const int pppn = sizeof(ppp) / sizeof(ppp[0]);
#define pppm  ppp[pppn - 1]


unsigned long otfpn(unsigned long n, int k)
{
	if (n < ppp[k])  return 0;
	if (n < ppp[k] * ppp[k])  return 1;
	unsigned long xpo, xpn;
	int nn;
	unsigned long pp2 = ppp[k] * ppp[k];
	xpo = n / ppp[k];
	if (k > 50 && n < 65537 * ppp[50] && xpo < pp2)
	{
		int low = 0;
		int high = pppn;
		int mid;
		int kp = 0;
		int lp = 0;
		while (low < high  && lp != kp + 1)
		{
			mid = low + ((high - low) / 2);
			kp = low;
			lp = high;
			if (ppp[mid] == xpo)
			{
				kp = mid;
				break;
			}
			if (xpo < ppp[mid])
				high = mid;
			else
				low = mid;
		}
		return (long)kp - (long)(k - 2);
	}
	xpn = xpo - xpo / 2;
	if (k > 1)
	{
		for (nn = 1; nn < k; nn++)
			xpn = xpn - fpn(xpo, nn);
	}
	return xpn;
}

unsigned long fpn(unsigned long n, int k)
{
	if (n < ppp[k])  return 0;
	if (n < ppp[k] * ppp[k])  return 1;
	unsigned long xpo, xpn;
	int nn;
	xpo = n / ppp[k];
	xpn = xpo - xpo / 2;
	if (k > 1)
	{
		for (nn = 1; nn < k; nn++)
			xpn = xpn - fpn(xpo, nn);
	}
	return xpn;
}

unsigned long klfindp(unsigned long n)
{
	int low = 0;
	int high = pppn;
	int mid, k;
	int kp = 0;
	unsigned long lp = 0;
	unsigned long pnum;
	if (n <= pppm)
	{
		while (low < high && lp != kp + 1)
		{
			mid = low + ((high - low) / 2);
			kp = low;
			lp = high;
			if (ppp[mid] == n)  return (long)mid + 1;
			if (n < ppp[mid])
				high = mid;
			else
				low = mid;
		}
		return (kp > 0) ? lp : 0;
	}
	else
	{
		pnum = (n + 1) / 2;
		unsigned long sqrtn = (unsigned long)(sqrt((double)n));

		lp = klfindp(sqrtn);

		for (k = 1; k <= lp; k++)
			pnum = pnum - fpn(n, k) + 1;
		return pnum;
	}
}

unsigned long findp(unsigned long n) {
	unsigned long ret = n >= 2 ? 1 : 0;
	for (unsigned long i = 3; i <= n; i += 2) {
		if (klPrime(i)) {
			ret += 1;
		}
	}
	return ret;
}

unsigned long findpEx(unsigned long n) {
	if (n < 2) return 0;

	char exist = 1;
	char not_exist = 0;

	unsigned long count = n - 1;      //假设全部是素数

	char* A = malloc(sizeof(char)*(n + 1));
	memset(A, exist, sizeof(char)*(n + 1));
	int k = (int)(sqrt((double)n));

	for (int p = 2; p <= k; p++)
	{
		if (A[p] == exist)
		{
			int j = p * p;
			while (j <= n)
			{
				if (A[j] == exist)     //只有没被去除，才做去除操作。避免重复统计
				{
					A[j] = not_exist;
					count--;           //减少1个
				}
				j += p;
			}
		}
	}

	free(A);
	return count;

}

int klPrime(unsigned long n)
{
	if (n % 2 == 0 || n <= 1) {
		return 0;
	}

	int i, k, j;
	k = (int)(sqrt((double)n));
	for (i = 0; i < pppn; i++)
	{
		//  pp=ppp[i];
		if (n % ppp[i] == 0)   return 0;
		if (k < ppp[i])   return 1;
	}
	for (j = pppm; j <= k; j += 2)
	{
		if (n % j == 0)
			return 0;
	}
	return 1;
}

int isPrime(unsigned long n)
{
	if (n % 2 == 0 || n <= 1) {
		return 0;
	}
	int i, k;
	k = (int)(sqrt((double)n));
	for (i = 3; i <= k; i += 2)
	{
		if (n % i == 0)
			return 0;
	}
	return 1;
}
