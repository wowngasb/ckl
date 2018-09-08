// ckl.c : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <string.h>

#include "md5.h"
#include "base64.h"
#include "util.h"

int main()
{	
	MD5_CTX context;
	int i;
	unsigned char digest[16] = { 0 };

	int retint = 0;
	unsigned char retstr[128] = { 0 };
	int tmpint = 0;
	unsigned char tmpstr[128] = { 0 };

	char * str = "1234567";
	char * key = "18fss45gre65q2%$@65e3276482";

	ckl_encode(str, strlen(str), retstr, &retint, key, 0);
	retstr[retint] = '\0';
	printf("ckl_encode len:%d ret:%s", retint, retstr);
	printf("\n");

	ckl_decode(retstr, retint, tmpstr, &tmpint, key);
	tmpstr[tmpint] = '\0';
	printf("ckl_decode len:%d ret:%s", tmpint, tmpstr);
	printf("\n");


	char * testkey = "zT5hF$E24*(#dfS^Yq3&6A^6";
	"m1lFv6XGYEAe9z";
	char * test[] = {
		"CDveD0kGQkX6c",
		"m1lFv6XGYEAe9z",
		"lJT_J__LJePfudOg",
		"dzWJrtwrqORxqOUJ4",
		"7r8Kqp3Fs3k5Dk3ZSp" ,
		"q58an2EvsFuIuSevy2" ,
		"qQDKgHWSuJFg-OCx-g" ,
		"coJbrAR2BcNFy8IWkA",
		"ewVITfifalugspmnN6"
	};
	for (i=0; i< sizeof(test) / 4; i++)
	{
		ckl_decode(test[i], strlen(test[i]), tmpstr, &tmpint, testkey);
		tmpstr[tmpint] = '\0';
		printf("ckl_decode test:%d len:%d ret:%s", i, tmpint, tmpstr);
		printf("\n");
	}

	int32ToHexWithLittleEndian(65535, retstr, &retint, 1);
	retstr[retint] = '\0';
	printf("int32ToHexWithLittleEndian len:%d ret:%s", retint, retstr);
	printf("\n");

	retint = 16;
	rand_str(retint, retstr);
	retstr[retint] = '\0';
	printf("rand_str len:%d ret:%s", retint, retstr);
	printf("\n");


	md5_str((char *)str, retstr, &retint, 1);
	retstr[retint] = '\0';
	printf("md5_str len:%d ret:%s", retint, retstr);
	printf("\n");


	MD5Init(&context);
	MD5Update(&context, (unsigned char *)str, strlen(str));
	MD5Final(&context, digest);

	/* 打印MD5值 */
	printf("md5: ");
	for (i = 0; i < 16; i++)
	{
		printf("%02x", digest[i]);
	}
	printf("\n");
	ByteToHexStr(digest, 16, retstr, &retint, 0);
	retstr[retint] = '\0';
	printf("ByteToHexStr len:%d ret:%s", retint, retstr);
	printf("\n");

	HexStrToByte(retstr, retint, tmpstr, &tmpint);
	tmpstr[tmpint] = '\0';
	ByteToHexStr(tmpstr, tmpint, retstr, &retint, 1);
	retstr[retint] = '\0';
	printf("ByteToHexStr2 len:%d ret:%s", retint, retstr);
	printf("\n");

	base64_encode(tmpstr, tmpint, retstr, &retint, 0);
	retstr[retint] = '\0';
	printf("base64_encode len:%d ret:%s", retint, retstr);
	printf("\n");

	base64_decode(retstr, retint, tmpstr, &tmpint, 0);
	tmpstr[tmpint] = '\0';
	ByteToHexStr(tmpstr, tmpint, retstr, &retint, 0);
	retstr[retint] = '\0';
	printf("ByteToHexStr base64_decode len:%d ret:%s", retint, retstr);
	printf("\n");

	base64_encode(tmpstr, tmpint, retstr, &retint, 1);
	retstr[retint] = '\0';
	printf("safe_base64_encode len:%d ret:%s", retint, retstr);
	printf("\n");

	base64_decode(retstr, retint, tmpstr, &tmpint, 1);
	tmpstr[tmpint] = '\0';
	ByteToHexStr(tmpstr, tmpint, retstr, &retint, 1);
	retstr[retint] = '\0';
	printf("ByteToHexStr safe_base64_decode len:%d ret:%s", retint, retstr);
	printf("\n");

	return 0;
}
