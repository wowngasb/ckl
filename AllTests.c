#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "md5.h"
#include "base64.h"
#include "util.h"
#include "bigint.h"

#include "CuTest.h"

void Test_ckl_encode_decode(CuTest *tc) {
	int retint = 0;
	unsigned char retstr[128] = { 0 };
	int tmpint = 0;
	unsigned char tmpstr[128] = { 0 };

	char * str = "1234567";
	char * key = "18fss45gre65q2%$@65e3276482";

	ckl_encode(str, (int)strlen(str), retstr, &retint, key, 0);
	retstr[retint] = '\0';
	CuAssertIntEquals(tc, 22, retint);

	ckl_decode(retstr, retint, tmpstr, &tmpint, key);
	tmpstr[tmpint] = '\0';

	CuAssertIntEquals(tc, 7, tmpint);

	// printf("ckl_encode len:%d ret:%s", retint, retstr);
	// printf("ckl_decode len:%d ret:%s", tmpint, tmpstr);
	// 	ckl_encode len:22 ret:U3J4YQN2GqeS8_aEaswTqk
	//  ckl_decode len : 7 ret : 1234567
	CuAssertStrEquals(tc, str, tmpstr);

	char * test[] = {
		"s",
		"se",
		"ser",
		"sers",
		"serse",
		"234",
		"43534",
		"adawe",
		"345345" ,
		"5345$#$5647" ,
		"fsxsr-OCx-g" ,
		"23423gd()mdQWQ# bnm565 3a4 azr3 rZRn3w4",
		"  "
	};
	int t_len;
	for (int i = 0; i< sizeof(test) / sizeof(test[0]); i++) {
		t_len = (int)strlen(test[i]);
		ckl_encode(test[i], t_len, retstr, &retint, key, 0);
		retstr[retint] = '\0';

		ckl_decode(retstr, retint, tmpstr, &tmpint, key);
		tmpstr[tmpint] = '\0';

		CuAssertIntEquals(tc, t_len, tmpint);
		CuAssertStrEquals(tc, test[i], tmpstr);
	}
}

void Test_ckl_decode_other(CuTest *tc) {
	int tmpint = 0;
	unsigned char tmpstr[128] = { 0 };

	char * testkey = "zT5hF$E24*(#dfS^Yq3&6A^6";

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
	char * test_ret[] = {
		"",
		"1",
		"12",
		"123",
		"1234" ,
		"1234" ,
		"1234" ,
		"1234",
		"1234"
	};
	for (int i = 0; i< sizeof(test) / sizeof(test[0]); i++) {
		ckl_decode(test[i], (int)strlen(test[i]), tmpstr, &tmpint, testkey);
		tmpstr[tmpint] = '\0';

		CuAssertIntEquals(tc, (int)strlen(test_ret[i]), tmpint);
		CuAssertStrEquals(tc, test_ret[i], tmpstr);
		// printf("ckl_decode test:%d len:%d ret:%s", i, tmpint, tmpstr);
		/*
		ckl_decode test:0 len:0 ret:
		ckl_decode test:1 len:1 ret:1
		ckl_decode test:2 len:2 ret:12
		ckl_decode test:3 len:3 ret:123
		ckl_decode test:4 len:4 ret:1234
		ckl_decode test:5 len:4 ret:1234
		ckl_decode test:6 len:4 ret:1234
		ckl_decode test:7 len:4 ret:1234
		ckl_decode test:8 len:4 ret:1234
		*/
	}
}

CuSuite* Suite_ckl_encode_decode() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, Test_ckl_encode_decode);
	SUITE_ADD_TEST(suite, Test_ckl_decode_other);
	return suite;
}

void Test_ckl_int32ToHexWithLittleEndian(CuTest *tc) {
	int t_int = 65535;
	int retint = 0;
	unsigned char retstr[128] = { 0 };

	int32ToHexWithLittleEndian(t_int, retstr, &retint, 1);
	retstr[retint] = '\0';

	CuAssertIntEquals(tc, 8, retint);
	CuAssertStrEquals(tc, "FFFF0000", retstr);
	// printf("int32ToHexWithLittleEndian len:%d ret:%s", retint, retstr);
	// int32ToHexWithLittleEndian len:8 ret:FFFF0000

	int r_int = hexToInt32WithLittleEndian(retstr);
	CuAssertIntEquals(tc, t_int, r_int);
	int test_arr[] = {
		0, 1, 2, 3, 4, 100, 300, 500, 1024, 2048, 6800, 8096, 65573, 655730
	};
	for (int i = 0; i < sizeof(test_arr) / sizeof(test_arr[0]); i++) {
		int32ToHexWithLittleEndian(test_arr[i], retstr, &retint, 1);
		retstr[retint] = '\0';
		int r_int = hexToInt32WithLittleEndian(retstr);
		CuAssertIntEquals(tc, test_arr[i], r_int);
	}
}

void Test_ckl_rand_str(CuTest *tc) {
	unsigned char retstr[128] = { 0 };
	int retint = 16;
	rand_str(retint, retstr);
	retstr[retint] = '\0';

	CuAssertIntEquals(tc, retint, strlen(retstr));
}

void Test_ckl_md5_str(CuTest *tc) {
	int retint = 0;
	unsigned char retstr[128] = { 0 };

	char * str = "1234567";

	md5_str((char *)str, retstr, &retint, 1);
	retstr[retint] = '\0';
	// printf("md5_str len:%d ret:%s", retint, retstr);
	// md5_str len : 32 ret : FCEA920F7412B5DA7BE0CF42B8C93759
	CuAssertIntEquals(tc, 32, retint);
	CuAssertStrEquals(tc, "FCEA920F7412B5DA7BE0CF42B8C93759", retstr);
}

void Test_ByteToHexStr(CuTest *tc) {
	MD5_CTX context;
	unsigned char digest[16] = { 0 };

	int retint = 0;
	unsigned char retstr[128] = { 0 };
	int tmpint = 0;
	unsigned char tmpstr[128] = { 0 };

	char * str = "1234567";

	MD5Init(&context);
	MD5Update(&context, (unsigned char *)str, (int)strlen(str));
	MD5Final(&context, digest);

	ByteToHexStr(digest, 16, retstr, &retint, 0);
	retstr[retint] = '\0';
	// printf("ByteToHexStr len:%d ret:%s", retint, retstr);
	// ByteToHexStr len:32 ret:fcea920f7412b5da7be0cf42b8c93759
	CuAssertIntEquals(tc, 32, retint);
	CuAssertStrEquals(tc, "fcea920f7412b5da7be0cf42b8c93759", retstr);

	HexStrToByte(retstr, retint, tmpstr, &tmpint);
	tmpstr[tmpint] = '\0';
	ByteToHexStr(tmpstr, tmpint, retstr, &retint, 0);
	retstr[retint] = '\0';
	// printf("ByteToHexStr2 len:%d ret:%s", retint, retstr);
	// ByteToHexStr2 len : 32 ret : fcea920f7412b5da7be0cf42b8c93759
	CuAssertIntEquals(tc, 32, retint);
	CuAssertStrEquals(tc, "fcea920f7412b5da7be0cf42b8c93759", retstr);

}

void Test_base64(CuTest *tc) {
	unsigned char digest[16] = { 0 };

	int retint = 0;
	unsigned char retstr[128] = { 0 };
	int tmpint = 0;
	unsigned char tmpstr[128] = { 0 };

	char * str = "1234567";

	md5_str((char *)str, retstr, &retint, 1);
	retstr[retint] = '\0';

	HexStrToByte(retstr, retint, tmpstr, &tmpint);
	tmpstr[tmpint] = '\0';

	base64_encode(tmpstr, tmpint, retstr, &retint, 0);
	retstr[retint] = '\0';
	// printf("base64_encode len:%d ret:%s", retint, retstr);
	// ase64_encode len:24 ret:/OqSD3QStdp74M9CuMk3WQ==
	CuAssertIntEquals(tc, 24, retint);
	CuAssertStrEquals(tc, "/OqSD3QStdp74M9CuMk3WQ==", retstr);

	base64_decode(retstr, retint, tmpstr, &tmpint, 0);
	tmpstr[tmpint] = '\0';
	ByteToHexStr(tmpstr, tmpint, retstr, &retint, 0);
	retstr[retint] = '\0';
	// printf("ByteToHexStr base64_decode len:%d ret:%s", retint, retstr);
	// 	ByteToHexStr base64_decode len:32 ret:fcea920f7412b5da7be0cf42b8c93759
	CuAssertIntEquals(tc, 32, retint);
	CuAssertStrEquals(tc, "fcea920f7412b5da7be0cf42b8c93759", retstr);

	base64_encode(tmpstr, tmpint, retstr, &retint, 1);
	retstr[retint] = '\0';
	// printf("safe_base64_encode len:%d ret:%s", retint, retstr);
	// safe_base64_encode len:22 ret:_OqSD3QStdp74M9CuMk3WQ
	CuAssertIntEquals(tc, 22, retint);
	CuAssertStrEquals(tc, "_OqSD3QStdp74M9CuMk3WQ", retstr);

	base64_decode(retstr, retint, tmpstr, &tmpint, 1);
	tmpstr[tmpint] = '\0';
	ByteToHexStr(tmpstr, tmpint, retstr, &retint, 1);
	retstr[retint] = '\0';
	// printf("ByteToHexStr safe_base64_decode len:%d ret:%s", retint, retstr);
	// ByteToHexStr safe_base64_decode len:32 ret:FCEA920F7412B5DA7BE0CF42B8C93759
	CuAssertIntEquals(tc, 32, retint);
	CuAssertStrEquals(tc, "FCEA920F7412B5DA7BE0CF42B8C93759", retstr);
}

CuSuite* Suite_ckl_other() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, Test_ckl_int32ToHexWithLittleEndian);
	SUITE_ADD_TEST(suite, Test_ckl_rand_str);
	SUITE_ADD_TEST(suite, Test_ckl_md5_str);
	SUITE_ADD_TEST(suite, Test_ByteToHexStr);
	SUITE_ADD_TEST(suite, Test_base64);
	return suite;
}

const static char* bigint_str_list[] = {
	"0",
	"123",
	"123456",
	"123456789",
	"1234567890123",
	"1234567890123456",
	"12312345678901234561234567890123456",
	"1234561234567890125678901234561234567890123456",
	"123456789123456789012345901212345678901234561234567890123456",
	"12345678901231234561234567890123456156789012345612345678901234561234567890123456",
	"123456789012345612345678901234564567890123456123456789012345612345678901234561234567890123456",
	"123123456789012345612345678901234561234901234561234567890123456123456789012345612345678901234561234567890123456",
	"12345567890123456123456789012345623456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456",
	"123456789123456789012345612345678901234561890123456123456789012345612345678234561234567890123456123456789012345612345678901234561234567890123456",
	"123456789012312345678901234561234567890123456123456789012345612345678989012345612345678901234561234567890123456123456789012345612345678901234561234567890123456",
	"1234567890123451234567890123456123456789012345612345678901234561201234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234566",
};

const static bigint_str_list_len = sizeof(bigint_str_list) / sizeof(bigint_str_list[0]);

void Test_ckl_bigint_get_put(CuTest *tc) {
	CBigInt A, B, C;
	char *A_ = "14256756745435234521231231231231234353464";
	char *B_ = "24256756745435234521231231231231234353464";
	char *C_ = "342567567454352345212312312312312343534641";

	CB_Get(A_, (int)strlen(A_), 10, &A);
	CB_Get(B_, (int)strlen(B_), 10, &B);
	CB_Get(C_, (int)strlen(C_), 10, &C);

	int retint = 0;
	unsigned char retstr[2048] = { 0 };

	CB_Put(&A, retstr, 2048, 10);
	CuAssertStrEquals(tc, A_, retstr);
	CB_Put(&B, retstr, 2048, 10);
	CuAssertStrEquals(tc, B_, retstr);
	CB_Put(&C, retstr, 2048, 10);
	CuAssertStrEquals(tc, C_, retstr);

	for (int i = 0; i < bigint_str_list_len; i++) {
		CB_Get(bigint_str_list[i], (int)strlen(bigint_str_list[i]), 10, &A);
		CB_Put(&A, retstr, 2048, 10);
		CuAssertStrEquals(tc, bigint_str_list[i], retstr);
	}
}

void Test_ckl_bigint_cmp(CuTest *tc) {
	CBigInt A, B, C;
	CB_Movi(1234, &A);
	CB_Movi(5678, &B);
	CB_Movi(91011, &C);

	CuAssertIntEquals(tc, 0, CB_Cmp(&A, &A));
	CuAssertIntEquals(tc, -1, CB_Cmp(&A, &B));
	CuAssertIntEquals(tc, 1, CB_Cmp(&B, &A));
	CuAssertIntEquals(tc, -1, CB_Cmp(&A, &C));
	CuAssertIntEquals(tc, 1, CB_Cmp(&C, &B));

	char *A_ = "14256756745435234521231231231231234353464";
	char *B_ = "24256756745435234521231231231231234353464";
	char *C_ = "342567567454352345212312312312312343534641";

	CB_Get(A_, (int)strlen(A_), 10, &A);
	CB_Get(B_, (int)strlen(B_), 10, &B);
	CB_Get(C_, (int)strlen(C_), 10, &C);

	CuAssertIntEquals(tc, 0, CB_Cmp(&A, &A));
	CuAssertIntEquals(tc, -1, CB_Cmp(&A, &B));
	CuAssertIntEquals(tc, 1, CB_Cmp(&B, &A));
	CuAssertIntEquals(tc, -1, CB_Cmp(&A, &C));
	CuAssertIntEquals(tc, 1, CB_Cmp(&C, &B));
}

void Test_ckl_bigint_add(CuTest *tc) {
	CBigInt A, B, C, S, R;

	char *A_ = "14256756745435234521231231231231234353464";
	char *B_ = "24256756745435234521231231231231234353464";
	char *C_ = "342567567454352345212312312312312343534641";
	CB_Get(A_, (int)strlen(A_), 10, &A);
	CB_Get(B_, (int)strlen(B_), 10, &B);
	CB_Get(C_, (int)strlen(C_), 10, &C);

	CB_Add(&A, &B, &S);
	char *R1_ = "38513513490870469042462462462462468706928";
	CB_Get(R1_, (int)strlen(R1_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));

	CB_Add(&A, &C, &S);
	char *R2_ = "356824324199787579733543543543543577888105";
	CB_Get(R2_, (int)strlen(R2_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));

	CB_Add(&B, &C, &S);
	char *R3_ = "366824324199787579733543543543543577888105";
	CB_Get(R3_, (int)strlen(R3_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));
}

void Test_ckl_bigint_sub(CuTest *tc) {
	CBigInt A, B, C, S, R;

	char *A_ = "914256756745435234521231231231231234353464";
	char *B_ = "782425675674543523452123123123123123435346";
	char *C_ = "342567567454352345212312312312312343534641";
	CB_Get(A_, (int)strlen(A_), 10, &A);
	CB_Get(B_, (int)strlen(B_), 10, &B);
	CB_Get(C_, (int)strlen(C_), 10, &C);

	CB_Sub(&A, &B, &S);
	char *R1_ = "131831081070891711069108108108108110918118";
	CB_Get(R1_, (int)strlen(R1_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));

	CB_Sub(&A, &C, &S);
	char *R2_ = "571689189291082889308918918918918890818823";
	CB_Get(R2_, (int)strlen(R2_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));

	CB_Sub(&B, &C, &S);
	char *R3_ = "439858108220191178239810810810810779900705";
	CB_Get(R3_, (int)strlen(R3_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));
}

void Test_ckl_bigint_mul(CuTest *tc) {
	CBigInt A, B, C, X, Y, Z;
	int retint = 0;
	unsigned char retstr[2048] = { 0 };

	char *X_ = "231231231221231231233123435464";
	char *Y_ = "142567567454352345123243534364";
	CB_Get(X_, (int)strlen(X_), 10, &X);
	CB_Get(Y_, (int)strlen(Y_), 10, &Y);
	CB_Mul(&X, &Y, &Z);

	for (int i = 0; i < bigint_str_list_len; i++) {
		CB_Get(bigint_str_list[i], (int)strlen(bigint_str_list[i]), 10, &A);
		CB_Muli(&A, 2, &B);
		CB_Add(&A, &A, &C);

		CuAssertIntEquals(tc, 0, CB_Cmp(&B, &C));

		CB_Muli(&A, 3, &B);
		CB_Add(&A, &A, &C);
		CB_S_Add(&C, &A);

		CuAssertIntEquals(tc, 0, CB_Cmp(&B, &C));

		CB_Mul(&A, &Z, &B);
		CB_Mul(&A, &X, &C);
		CB_S_Mul(&C, &Y);
		CuAssertIntEquals(tc, 0, CB_Cmp(&B, &C));
	}
}

void Test_ckl_bigint_mul2(CuTest *tc) {
	CBigInt A, B, C, S, R;

	char *A_ = "914256756745435234521231231231231234353464";
	char *B_ = "782425675674543523452123123123123123435346";
	char *C_ = "342567567454352345212312312312312343534641";
	CB_Get(A_, (int)strlen(A_), 10, &A);
	CB_Get(B_, (int)strlen(B_), 10, &B);
	CB_Get(C_, (int)strlen(C_), 10, &C);

	CB_Mul(&A, &B, &S);
	char *R1_ = "715337960636563940574034165385623354370778401396613281226909285848406966498915138544";
	CB_Get(R1_, (int)strlen(R1_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));

	CB_Mul(&A, &C, &S);
	char *R2_ = "313194713186989288199503625649422280943032024188354685535543955321177130372122346424";
	CB_Get(R2_, (int)strlen(R2_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));

	CB_Mul(&B, &C, &S);
	char *R3_ = "268033660429656399361658170372750057476739996391808486812616310607192788112274820786";
	CB_Get(R3_, (int)strlen(R3_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));
}

void Test_ckl_bigint_div(CuTest *tc) {
	CBigInt A, B, C, X, Y, Z;

	char *X_ = "231231231221231231233123435464";
	char *Y_ = "142567567454352345123243534364";
	CB_Get(X_, (int)strlen(X_), 10, &X);
	CB_Get(Y_, (int)strlen(Y_), 10, &Y);
	CB_Mul(&X, &Y, &Z);

	for (int i = 0; i < bigint_str_list_len; i++) {
		CB_Get(bigint_str_list[i], (int)strlen(bigint_str_list[i]), 10, &A);

		CB_Mul(&A, &Z, &B);
		CB_Div(&B, &X, &C);
		CB_S_Div(&C, &Y);
		CuAssertIntEquals(tc, 0, CB_Cmp(&A, &C));
	}
}

void Test_ckl_bigint_div2(CuTest *tc) {
	CBigInt A, B, C, S, R;

	char *A_ = "715337960636563940574034165385623354370778401396613281226909285848406966498915138544";
	char *B_ = "782425675674543523453123123435346";
	char *C_ = "342567567454352343534641";
	CB_Get(A_, (int)strlen(A_), 10, &A);
	CB_Get(B_, (int)strlen(B_), 10, &B);
	CB_Get(C_, (int)strlen(C_), 10, &C);

	CB_Div(&A, &B, &S);
	char *R1_ = "914256756745435234520062740637656180745796783793647";
	CB_Get(R1_, (int)strlen(R1_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));

	CB_Div(&A, &C, &S);
	char *R2_ = "2088166039629200527261405558028711359332057856671964538814012";
	CB_Get(R2_, (int)strlen(R2_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));

	CB_Div(&B, &C, &S);
	char *R3_ = "2284003945";
	CB_Get(R3_, (int)strlen(R3_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));
}

void Test_ckl_bigint_mod(CuTest *tc) {
	CBigInt A, B, C, S, R;

	char *A_ = "715337960636563940574034165385623354370778401396613281226909285848406966498915138544";
	char *B_ = "782425675674543523453123123435346";
	char *C_ = "342567567454352343534641";
	CB_Get(A_, (int)strlen(A_), 10, &A);
	CB_Get(B_, (int)strlen(B_), 10, &B);
	CB_Get(C_, (int)strlen(C_), 10, &C);

	CB_Mod(&A, &B, &S);
	char *R1_ = "422652519357991133203661905091682";
	CB_Get(R1_, (int)strlen(R1_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));

	CB_Mod(&A, &C, &S);
	char *R2_ = "114327586380072736948852";
	CB_Get(R2_, (int)strlen(R2_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));

	CB_Mod(&B, &C, &S);
	char *R3_ = "179749163400007835276601";
	CB_Get(R3_, (int)strlen(R3_), 10, &R);

	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));
}

void Test_ckl_bigint_sqrt(CuTest *tc) {
	CBigInt A, B, C;

	for (int i = 0; i < bigint_str_list_len; i++) {
		CB_Get(bigint_str_list[i], (int)strlen(bigint_str_list[i]), 10, &A);

		CB_Mul(&A, &A, &B);
		CB_Sqrt(&B, &C);
		CB_S_Sqrt(&B);
		CuAssertIntEquals(tc, 0, CB_Cmp(&A, &C));
		CuAssertIntEquals(tc, 0, CB_Cmp(&A, &B));

		CB_S_Power(&A, 2);

		CB_Mul(&A, &A, &B);
		CB_Sqrt(&B, &C);
		CB_S_Sqrt(&B);
		CuAssertIntEquals(tc, 0, CB_Cmp(&A, &C));
		CuAssertIntEquals(tc, 0, CB_Cmp(&A, &B));
	}
}

void Test_ckl_bigint_ModExp(CuTest *tc) {
	CBigInt A, B, C, S, R;

	char *A_ = "7153379606909285848406966498915138544";
	char *B_ = "7";
	char *C_ = "3425634641";
	CB_Get(A_, (int)strlen(A_), 10, &A);
	CB_Get(B_, (int)strlen(B_), 10, &B);
	CB_Get(C_, (int)strlen(C_), 10, &C);

	CB_ModExp(&A, &B, &C, &S);
	char *R1_ = "2358361836";
	CB_Get(R1_, (int)strlen(R1_), 10, &R);
	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));

	char *B2_ = "70";
	CB_Get(B2_, (int)strlen(B2_), 10, &B);

	CB_ModExp(&A, &B, &C, &S);

	int retint = 0;
	unsigned char retstr[2048] = { 0 };

	char *R2_ = "1724523087";
	CB_Get(R2_, (int)strlen(R2_), 10, &R);
	CuAssertIntEquals(tc, 0, CB_Cmp(&S, &R));
}

void Test_ckl_bigint_prime(CuTest *tc) {
	CBigInt A;
	char *A_ = "2147483647";
	CB_Get(A_, (int)strlen(A_), 10, &A);
	CuAssertIntEquals(tc, 0, CB_TestPrime(&A));

	CB_S_Power(&A, 2);
	CuAssertIntEquals(tc, 1, CB_TestPrime(&A));

	CB_FindPrime(1024 / 32, &A);
	CuAssertIntEquals(tc, 0, CB_TestPrime(&A));

	CB_FindPrime(1099 / 32, &A);
	CuAssertIntEquals(tc, 0, CB_TestPrime(&A));

	CB_FindPrime(1199 / 32, &A);
	CuAssertIntEquals(tc, 0, CB_TestPrime(&A));

	CB_FindPrime(1299 / 32, &A);
	CuAssertIntEquals(tc, 0, CB_TestPrime(&A));
}

CuSuite* Suite_ckl_bigint() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, Test_ckl_bigint_get_put);
	SUITE_ADD_TEST(suite, Test_ckl_bigint_cmp);
	SUITE_ADD_TEST(suite, Test_ckl_bigint_add);
	SUITE_ADD_TEST(suite, Test_ckl_bigint_sub);
	SUITE_ADD_TEST(suite, Test_ckl_bigint_mul);
	SUITE_ADD_TEST(suite, Test_ckl_bigint_mul2);
	SUITE_ADD_TEST(suite, Test_ckl_bigint_div);
	SUITE_ADD_TEST(suite, Test_ckl_bigint_div2);
	SUITE_ADD_TEST(suite, Test_ckl_bigint_mod);
	SUITE_ADD_TEST(suite, Test_ckl_bigint_sqrt);
	SUITE_ADD_TEST(suite, Test_ckl_bigint_ModExp);
	SUITE_ADD_TEST(suite, Test_ckl_bigint_prime);
	return suite;
}

void RunAllTests(void) {
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, Suite_ckl_encode_decode());
	CuSuiteAddSuite(suite, Suite_ckl_other());


	CuSuiteAddSuite(suite, Suite_ckl_bigint());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	RunAllTests();
	system("PAUSE");
}
