#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"

//小素数表
const static int PrimeTable[] = {
	3,    5,    7,    11,   13,   17,   19,   23,   29,   31,
	37,   41,   43,   47,   53,   59,   61,   67,   71,   73,
	79,   83,   89,   97,   101,  103,  107,  109,  113,  127,
	131,  137,  139,  149,  151,  157,  163,  167,  173,  179,
	181,  191,  193,  197,  199,  211,  223,  227,  229,  233,
	239,  241,  251,  257,  263,  269,  271,  277,  281,  283,
	293,  307,  311,  313,  317,  331,  337,  347,  349,  353,
	359,  367,  373,  379,  383,  389,  397,  401,  409,  419,
	421,  431,  433,  439,  443,  449,  457,  461,  463,  467,
	479,  487,  491,  499,  503,  509,  521,  523,  541,  547,
	557,  563,  569,  571,  577,  587,  593,  599,  601,  607,
	613,  617,  619,  631,  641,  643,  647,  653,  659,  661,
	673,  677,  683,  691,  701,  709,  719,  727,  733,  739,
	743,  751,  757,  761,  769,  773,  787,  797,  809,  811,
	821,  823,  827,  829,  839,  853,  857,  859,  863,  877,
	881,  883,  887,  907,  911,  919,  929,  937,  941,  947,
	953,  967,  971,  977,  983,  991,  997,  1009, 1013, 1019,
	1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087,
	1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153,
	1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223, 1229,
	1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297,
	1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381,
	1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453,
	1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523,
	1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597,
	1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663,
	1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741,
	1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823,
	1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901,
	1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993,
	1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053, 2063,
	2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129, 2131,
	2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213, 2221,
	2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287, 2293,
	2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 2371,
	2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423, 2437,
	2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531, 2539,
	2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617, 2621,
	2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687, 2689,
	2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741, 2749,
	2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 2833,
	2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903, 2909,
	2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999, 3001,
	3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079, 3083,
	3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181, 3187,
	3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257, 3259,
	3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331, 3343,
	3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413, 3433,
	3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511, 3517,
	3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571, 3581,
	3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637, 3643, 3659,
	3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727, 3733,
	3739, 3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821, 3823,
	3833, 3847, 3851, 3853, 3863, 3877, 3881, 3889, 3907, 3911,
	3917, 3919, 3923, 3929, 3931, 3943, 3947, 3967, 3989, 4001,
	4003, 4007, 4013, 4019, 4021, 4027, 4049, 4051, 4057, 4073,
	4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133, 4139, 4153,
	4157, 4159, 4177, 4201, 4211, 4217, 4219, 4229, 4231, 4241,
	4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297, 4327,
	4337, 4339, 4349, 4357, 4363, 4373, 4391, 4397, 4409, 4421,
	4423, 4441, 4447, 4451, 4457, 4463, 4481, 4483, 4493, 4507,
	4513, 4517, 4519, 4523, 4547, 4549, 4561, 4567, 4583, 4591,
	4597, 4603, 4621, 4637, 4639, 4643, 4649, 4651, 4657, 4663,
	4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751, 4759,
	4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831, 4861,
	4871, 4877, 4889, 4903, 4909, 4919, 4931, 4933, 4937, 4943,
	4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999, 5003, 5009,
	5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087, 5099,
	5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171, 5179, 5189,
	5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279, 5281,
	5297, 5303, 5309, 5323, 5333, 5347, 5351, 5381, 5387, 5393,
	5399, 5407, 5413, 5417, 5419, 5431, 5437, 5441, 5443, 5449,
	5471, 5477, 5479, 5483, 5501, 5503, 5507, 5519, 5521, 5527,
	5531, 5557, 5563, 5569, 5573, 5581, 5591, 5623, 5639, 5641,
	5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689, 5693, 5701,
	5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791, 5801,
	5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851, 5857, 5861,
	5867, 5869, 5879, 5881, 5897, 5903, 5923, 5927, 5939, 5953,
	5981, 5987, 6007, 6011, 6029, 6037, 6043, 6047, 6053, 6067,
	6073, 6079, 6089, 6091, 6101, 6113, 6121, 6131, 6133, 6143,
	6151, 6163, 6173, 6197, 6199, 6203, 6211, 6217, 6221, 6229,
	6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299, 6301, 6311,
	6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367, 6373,
	6379, 6389, 6397, 6421, 6427, 6449, 6451, 6469, 6473, 6481,
	6491, 6521, 6529, 6547, 6551, 6553, 6563, 6569, 6571, 6577,
	6581, 6599, 6607, 6619, 6637, 6653, 6659, 6661, 6673, 6679,
	6689, 6691, 6701, 6703, 6709, 6719, 6733, 6737, 6761, 6763,
	6779, 6781, 6791, 6793, 6803, 6823, 6827, 6829, 6833, 6841,
	6857, 6863, 6869, 6871, 6883, 6899, 6907, 6911, 6917, 6947,
	6949, 6959, 6961, 6967, 6971, 6977, 6983, 6991, 6997, 7001,
	7013, 7019, 7027, 7039, 7043, 7057, 7069, 7079, 7103, 7109,
	7121, 7127, 7129, 7151, 7159, 7177, 7187, 7193, 7207, 7211,
	7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283, 7297, 7307,
	7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393, 7411, 7417,
	7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499, 7507,
	7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561, 7573,
	7577, 7583, 7589, 7591, 7603, 7607, 7621, 7639, 7643, 7649,
	7669, 7673, 7681, 7687, 7691, 7699, 7703, 7717, 7723, 7727,
	7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823, 7829, 7841,
	7853, 7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919, 7927,
	7933, 7937, 7949, 7951, 7963, 7993, 8009, 8011, 8017, 8039,
	8053, 8059, 8069, 8081, 8087, 8089, 8093, 8101, 8111, 8117,
	8123, 8147, 8161, 8167, 8171, 8179, 8191, 8209, 8219, 8221,
	8231, 8233, 8237, 8243, 8263, 8269, 8273, 8287, 8291, 8293,
	8297, 8311, 8317, 8329, 8353, 8363, 8369, 8377, 8387, 8389,
	8419, 8423, 8429, 8431, 8443, 8447, 8461, 8467, 8501, 8513,
	8521, 8527, 8537, 8539, 8543, 8563, 8573, 8581, 8597, 8599,
	8609, 8623, 8627, 8629, 8641, 8647, 8663, 8669, 8677, 8681,
	8689, 8693, 8699, 8707, 8713, 8719, 8731, 8737, 8741, 8747,
	8753, 8761, 8779, 8783, 8803, 8807, 8819, 8821, 8831, 8837,
	8839, 8849, 8861, 8863, 8867, 8887, 8893, 8923, 8929, 8933,
	8941, 8951, 8963, 8969, 8971, 8999, 9001, 9007, 9011, 9013,
	9029, 9041, 9043, 9049, 9059, 9067, 9091, 9103, 9109, 9127,
	9133, 9137, 9151, 9157, 9161, 9173, 9181, 9187, 9199, 9203,
	9209, 9221, 9227, 9239, 9241, 9257, 9277, 9281, 9283, 9293,
	9311, 9319, 9323, 9337, 9341, 9343, 9349, 9371, 9377, 9391,
	9397, 9403, 9413, 9419, 9421, 9431, 9433, 9437, 9439, 9461,
	9463, 9467, 9473, 9479, 9491, 9497, 9511, 9521, 9533, 9539,
	9547, 9551, 9587, 9601, 9613, 9619, 9623, 9629, 9631, 9643,
	9649, 9661, 9677, 9679, 9689, 9697, 9719, 9721, 9733, 9739,
	9743, 9749, 9767, 9769, 9781, 9787, 9791, 9803, 9811, 9817,
	9829, 9833, 9839, 9851, 9857, 9859, 9871, 9883, 9887, 9901,
	9907, 9923, 9929, 9931, 9941, 9949, 9967, 9973, 10007,10009
};

const static int PrimeTableLen = sizeof(PrimeTable) / sizeof(PrimeTable[0]);

const static char *CharTable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";


/*
* 辅助函数 结果存回原数据单元
*/

//大数 取负 结果存入 src
void CB_S_Neg(CBigInt *src) {
	if (src->m_nLength == 1 && src->m_ulValue[0] == 0) {
		src->m_signed = 0;
	}
	else {
		src->m_signed = src->m_signed == -1 ? 0 : -1;
	}
}

//大数 绝对值 结果存入 src
void CB_S_Abs(CBigInt *src) {
	src->m_signed = 0;
}

//计算两个大数的和  src + A 结果存入 src
void CB_S_Add(CBigInt *src, CBigInt *A) {
	CBigInt tmp;
	CB_Add(src, A, &tmp);
	CB_Mov(&tmp, src);
}

//大数与普通整数相加  src + A 结果存入 src
void CB_S_Addi(CBigInt *src, unsigned long A) {
	CBigInt tmp;
	CB_Addi(src, A, &tmp);
	CB_Mov(&tmp, src);
}

//计算两个大数的差  src - A 结果存入 src
void CB_S_Sub(CBigInt *src, CBigInt *A) {
	CBigInt tmp;
	CB_Sub(src, A, &tmp);
	CB_Mov(&tmp, src);
}

//计算两个大数的差  A - src 结果存入 src
void CB_S_Sub_Reverse(CBigInt *src, CBigInt *A) {
	CBigInt tmp;
	CB_Sub(A, src, &tmp);
	CB_Mov(&tmp, src);
}

//大数与普通整数相减  src - A 结果存入 src
void CB_S_Subi(CBigInt *src, unsigned long A) {
	CBigInt tmp;
	CB_Subi(src, A, &tmp);
	CB_Mov(&tmp, src);
}

//计算两个大数的积   src * A 结果存入 src
void CB_S_Mul(CBigInt *src, CBigInt *A) {
	CBigInt tmp;
	CB_Mul(src, A, &tmp);
	CB_Mov(&tmp, src);
}

//大数与普通整数相乘   src * A 结果存入 src
void CB_S_Muli(CBigInt *src, unsigned long A) {
	CBigInt tmp;
	CB_Muli(src, A, &tmp);
	CB_Mov(&tmp, src);
}

//计算两个大数的商   src / A 结果存入 src
void CB_S_Div(CBigInt *src, CBigInt *A) {
	CBigInt tmp;
	CB_Div(src, A, &tmp);
	CB_Mov(&tmp, src);
}

//计算两个大数的商   A / src 结果存入 src
void CB_S_Div_Reverse(CBigInt *src, CBigInt *A) {
	CBigInt tmp;
	CB_Div(A, src, &tmp);
	CB_Mov(&tmp, src);
}

//大数与普通整数相除   src / A 结果存入 src
void CB_S_Divi(CBigInt *src, unsigned long A) {
	CBigInt tmp;
	CB_Divi(src, A, &tmp);
	CB_Mov(&tmp, src);
}

//计算两个大数相除的余数   src % A 结果存入 src
void CB_S_Mod(CBigInt *src, CBigInt *A) {
	CBigInt tmp;
	CB_Mod(src, A, &tmp);
	CB_Mov(&tmp, src);
}

//计算两个大数相除的余数   A % src 结果存入 src
void CB_S_Mod_Reverse(CBigInt *src, CBigInt *A) {
	CBigInt tmp;
	CB_Mod(A, src, &tmp);
	CB_Mov(&tmp, src);
}

//计算大数算术平方根   Sqrt(src) 结果存入 src
void CB_S_Sqrt(CBigInt *src) {
	CBigInt tmp;
	CB_Sqrt(src, &tmp);
	CB_Mov(&tmp, src);
}

//计算大数算术平方根   Power(src, n) 结果存入 src
void CB_S_Power(CBigInt *src, unsigned int n) {
	CBigInt tmp;
	CB_Power(src, n, &tmp);
	CB_Mov(&tmp, src);
}

void CB_S_ModInv(CBigInt *src, CBigInt *A) {
	CBigInt tmp;
	CB_ModInv(src, A, &tmp);
	CB_Mov(&tmp, src);
}

void CB_S_ModMul(CBigInt *src, CBigInt *A, CBigInt *B) {
	CBigInt tmp;
	CB_ModMul(src, A, B, &tmp);
	CB_Mov(&tmp, src);
}

void CB_S_MonPro(CBigInt *src, CBigInt *A, CBigInt *B, unsigned long n) {
	CBigInt tmp;
	CB_MonPro(src, A, B, n, &tmp);
	CB_Mov(&tmp, src);
}

void CB_S_ModExp(CBigInt *src, CBigInt *A, CBigInt *B) {
	CBigInt tmp;
	CB_ModExp(src, A, B, &tmp);
	CB_Mov(&tmp, src);
}

/****************************************************************************************
大数 取负
调用方式：CB_Neg(src, dst)
返回值：无  src  取负  结果存入 dst
****************************************************************************************/
void CB_Neg(CBigInt *src, CBigInt *dst) {
	CB_Mov(src, dst);
	if (src->m_nLength == 1 && src->m_ulValue[0] == 0) {
		dst->m_signed = 0;
	}
	else {
		dst->m_signed = src->m_signed == -1 ? 0 : -1;
	}
}

/****************************************************************************************
大数 绝对值
调用方式：CB_Abs(src, dst)
返回值：无  src的绝对值  结果存入 dst
****************************************************************************************/
void CB_Abs(CBigInt *src, CBigInt *dst) {
	CB_Mov(src, dst);
	dst->m_signed = 0;
}

/****************************************************************************************
大数比较
调用方式：CB_Cmp(src, A)
返回值：int 若 src<A 返回 -1  若 src=A 返回 0  若 src>A 返回 1
****************************************************************************************/
int CB_Cmp(CBigInt *src, CBigInt *A) {
	if (src->m_signed == -1 && A->m_signed != -1) {
		return -1;
	}
	if (src->m_signed != -1 && A->m_signed == -1) {
		return 1;
	}

	int opposite = 0;
	if (src->m_signed == -1 && A->m_signed == -1) {
		opposite = 1;
	}

	if (src->m_nLength > A->m_nLength) {
		return opposite ? -1 : 1;
	}
	if (src->m_nLength < A->m_nLength) {
		return opposite ? 1 : -1;
	}
	for (int i = src->m_nLength - 1; i >= 0; i--) {
		if (src->m_ulValue[i] > A->m_ulValue[i]) {
			return opposite ? -1 : 1;
		}
		if (src->m_ulValue[i] < A->m_ulValue[i]) {
			return opposite ? 1 : -1;
		}
	}
	return 0;
}

int CB_Cmpi(CBigInt *src, unsigned long A) {
	if (src->m_signed == -1) {
		return -1;
	}

	if (src->m_nLength == 1) {
		return src->m_ulValue[0] == A ? 0 : (src->m_ulValue[0] > A ? 1 : -1);
	}
	else {
		return 1;
	}
}

/****************************************************************************************
大数赋值
调用方式：CB_Mov(src, dst)
返回值：无，dst 被赋值为 src
****************************************************************************************/
void CB_Mov(CBigInt *src, CBigInt *dst) {
	dst->m_nLength = src->m_nLength;
	dst->m_signed = src->m_signed;
	memcpy(dst->m_ulValue, src->m_ulValue, sizeof(dst->m_ulValue));
}

void CB_Movi(unsigned __int64 A, CBigInt *dst) {
	memset(dst->m_ulValue, 0, sizeof(dst->m_ulValue));
	if (A > 0xffffffff){
		dst->m_nLength = 2;
		dst->m_ulValue[1] = (unsigned long)(A >> 32);
		dst->m_ulValue[0] = (unsigned long)A;
	} else {
		dst->m_nLength = 1;
		dst->m_ulValue[0] = (unsigned long)A;
	}
	dst->m_signed = 0;
}

/****************************************************************************************
大数相加
调用形式：CB_Add(src, A, dst)
返回值：无，dst 被赋值为 src+A
****************************************************************************************/
void CB_Add(CBigInt *src, CBigInt *A, CBigInt *dst)
{
	if (src->m_signed == -1 && A->m_signed != -1) {
		CBigInt _src;
		CB_Abs(src, &_src);
		CB_Sub(A, &_src, dst);
		return;
	}
	if (src->m_signed != -1 && A->m_signed == -1) {
		CBigInt _A;
		CB_Abs(A, &_A);
		CB_Sub(src, &_A, dst);
		return;
	}
	if (src->m_signed == -1 && A->m_signed == -1) {
		CBigInt _src, _A;
		CB_Abs(src, &_src);
		CB_Abs(A, &_A);
		CB_Add(&_src, &_A, dst);
		CB_S_Neg(dst);
		return;
	}

	if (A->m_nLength == 1) {
		CB_Addi(src, A->m_ulValue[0], dst);
		return;
	}

	CB_Mov(src, dst);
	unsigned int carry = 0;
	unsigned __int64 sum = 0;
	if (dst->m_nLength < A->m_nLength) {
		dst->m_nLength = A->m_nLength;
	}
	for (int i = 0; i < dst->m_nLength; i++) {
		sum = A->m_ulValue[i];
		sum = sum + dst->m_ulValue[i] + carry;
		dst->m_ulValue[i] = (unsigned long)sum;
		carry = (unsigned int)(sum >> 32);
	}
	dst->m_ulValue[dst->m_nLength] = carry;
	dst->m_nLength += carry;
}

void CB_Addi(CBigInt *src, unsigned long A, CBigInt *dst) {
	if (src->m_signed == -1) {
		CBigInt _src;
		CB_Abs(src, &_src);
		CB_Subi(&_src, A, dst);
		CB_S_Neg(dst);
		return;
	}

	CB_Mov(src, dst);
	unsigned __int64 sum;
	sum = dst->m_ulValue[0];
	sum += A;
	dst->m_ulValue[0] = (unsigned long)sum;
	if (sum > 0xffffffff) {
		unsigned int i = 1;
		while (dst->m_ulValue[i] == 0xffffffff) { 
			dst->m_ulValue[i] = 0;
			i++;
		}
		dst->m_ulValue[i]++;
		if (dst->m_nLength == i) {
			dst->m_nLength++;
		}
	}
}

/****************************************************************************************
大数相减
调用形式：CB_Sub(src, A, dst)
返回值：无，dst 被赋值为 src-A
****************************************************************************************/
void CB_Sub(CBigInt *src, CBigInt *A, CBigInt *dst) {
	if (A->m_nLength == 1) {
		 CB_Subi(src, A->m_ulValue[0], dst);
		 return;
	}

	CB_Mov(src, dst);
	if (CB_Cmp(dst, A) <= 0) {
		CB_Movi(0, dst);
		return ;
	}

	unsigned int carry = 0;
	unsigned __int64 num;
	for (int i = 0; i < src->m_nLength; i++) {
		if (src->m_ulValue[i] > A->m_ulValue[i] || (src->m_ulValue[i] == A->m_ulValue[i] && carry == 0))
		{
			dst->m_ulValue[i] = src->m_ulValue[i] - carry - A->m_ulValue[i];
			carry = 0;
		} else{
			num = 0x100000000 + src->m_ulValue[i];
			dst->m_ulValue[i] = (unsigned long)(num - carry - A->m_ulValue[i]);
			carry = 1;
		}
	}
	while (dst->m_ulValue[dst->m_nLength - 1] == 0) {
		dst->m_nLength--;
	}
}

void CB_Subi(CBigInt *src, unsigned long A, CBigInt *dst) {
	CB_Mov(src, dst);
	if (dst->m_ulValue[0] >= A) {
		dst->m_ulValue[0] -= A;
		return;
	}
	if (dst->m_nLength == 1) { 
		CB_Movi(0, dst);
		return ; 
	}
	unsigned __int64 num = 0x100000000 + dst->m_ulValue[0];
	dst->m_ulValue[0] = (unsigned long)(num - A);
	int i = 1;
	while (dst->m_ulValue[i] == 0) { 
		dst->m_ulValue[i] = 0xffffffff; 
		i++; 
	}
	dst->m_ulValue[i]--;
	if (dst->m_ulValue[i] == 0) {
		dst->m_nLength--;
	}
}

/****************************************************************************************
大数相乘
调用形式：CB_Mul(src, A, dst)
返回值：无，dst 被赋值为 src*A
****************************************************************************************/
void CB_Mul(CBigInt *src, CBigInt *A, CBigInt *dst) {
	if (A->m_nLength == 1) {
		 CB_Muli(src, A->m_ulValue[0], dst);
		 return;
	}

	CB_Movi(0, dst);
	unsigned __int64 sum, mul = 0, carry = 0;
	dst->m_nLength = src->m_nLength + A->m_nLength - 1;
	for (int i = 0; i < dst->m_nLength; i++) {
		sum = carry;
		carry = 0;
		for (int j = 0; j < A->m_nLength; j++) {
			if ((i - j) >= 0 && (i - j) < src->m_nLength) {
				mul = src->m_ulValue[i - j];
				mul *= A->m_ulValue[j];
				carry += mul >> 32;
				mul = mul & 0xffffffff;
				sum += mul;
			}
		}
		carry += sum >> 32;
		dst->m_ulValue[i] = (unsigned long)sum;
	}
	if (carry) { 
		dst->m_nLength++; 
		dst->m_ulValue[dst->m_nLength - 1] = (unsigned long)carry; 
	}
}

void CB_Muli(CBigInt *src, unsigned long A, CBigInt *dst) {
	CB_Mov(src, dst);
	unsigned __int64 mul;
	unsigned long carry = 0;
	for (int i = 0; i < src->m_nLength; i++) {
		mul = src->m_ulValue[i];
		mul = mul * A + carry;
		dst->m_ulValue[i] = (unsigned long)mul;
		carry = (unsigned long)(mul >> 32);
	}
	if (carry) { 
		dst->m_nLength++; 
		dst->m_ulValue[dst->m_nLength - 1] = carry; 
	}
}

/****************************************************************************************
大数相除
调用形式：CB_Div(src, A, dst)
返回值：无，dst 被赋值为 src / A
****************************************************************************************/
void CB_Div(CBigInt *src, CBigInt *A, CBigInt *dst) {
	if (A->m_nLength == 1) {
		 CB_Divi(src, A->m_ulValue[0], dst);
		 return;
	}

	CB_Movi(0, dst);
	CBigInt Y, Z;
	CB_Mov(src, &Y);
	CB_Movi(0, &Z);
	unsigned i, len;
	unsigned __int64 num, div;
	while (CB_Cmp(&Y, A) >= 0) {
		div = Y.m_ulValue[Y.m_nLength - 1];
		num = A->m_ulValue[A->m_nLength - 1];
		len = Y.m_nLength - A->m_nLength;
		if (div == num && len == 0) { 
		    CB_S_Addi(dst, 1);
			return; 
		}
		if (div <= num && len) { 
			len--; 
			div = (div << 32) + Y.m_ulValue[Y.m_nLength - 2]; 
		}
		div = div / (num + 1);
		CB_Movi(div, &Z);
		if (len) {
			Z.m_nLength += len;
			for (i = Z.m_nLength - 1; i >= len; i--) {
				Z.m_ulValue[i] = Z.m_ulValue[i - len];
			}
			for (i = 0; i < len; i++) {
				Z.m_ulValue[i] = 0;
			}
		}
		CB_S_Add(dst, &Z);
		CB_S_Mul(&Z, A);
		CB_S_Sub(&Y, &Z);
	}
}

void CB_Divi(CBigInt *src, unsigned long A, CBigInt *dst) {
	CB_Mov(src, dst);
	if (dst->m_nLength == 1) { 
		dst->m_ulValue[0] = dst->m_ulValue[0] / A; 
		return ; 
	}
	unsigned __int64 div, mul;
	unsigned long carry = 0;
	for (int i = dst->m_nLength - 1; i >= 0; i--) {
		div = carry;
		div = (div << 32) + dst->m_ulValue[i];
		dst->m_ulValue[i] = (unsigned long)(div / A);
		mul = (div / A)*A;
		carry = (unsigned long)(div - mul);
	}
	if (dst->m_ulValue[dst->m_nLength - 1] == 0) {
		dst->m_nLength--;
	}
}

/****************************************************************************************
大数求模
调用形式：CB_Mod(src, A, dst)
返回值：无，dst 被赋值为 src % A
****************************************************************************************/
void CB_Mod(CBigInt *src, CBigInt *A, CBigInt *dst) {
	int n = CB_Cmp(src, A);
	if (n < 0) {
		CB_Mov(src, dst);
		return ;
	}
	if (n = 0) {
		CB_Movi(0, dst);
		return ;
	}
	CB_Mov(src, dst);
	CBigInt Y;
	unsigned __int64 div, num;
	unsigned long carry = 0;
	unsigned i, len;
	while (1) {
		div = dst->m_ulValue[dst->m_nLength - 1];
		num = A->m_ulValue[A->m_nLength - 1];
		len = dst->m_nLength - A->m_nLength;  // 一定大于等于0
		if (div <= num) { 
			if (dst->m_nLength > A->m_nLength) {
				len--;
				div = (div << 32) + dst->m_ulValue[dst->m_nLength - 2];
			}
			if (dst->m_nLength == A->m_nLength && dst->m_nLength > 1) {
				div = (div << 32) + dst->m_ulValue[dst->m_nLength - 2];
				num = (num << 32) + A->m_ulValue[A->m_nLength - 2];
			}
		}
		div = div / (num + 1);
		CB_Movi(div, &Y);
		CB_S_Mul(&Y, A);
		if (len) {
			Y.m_nLength += len;
			for (i = Y.m_nLength - 1; i >= len; i--) {
				Y.m_ulValue[i] = Y.m_ulValue[i - len];
			}
			for (i = 0; i < len; i++) {
				Y.m_ulValue[i] = 0;
			}
		}

		CB_S_Sub(dst, &Y);
		n = CB_Cmp(dst, A);
		if (n == 0) { 
			CB_Movi(0, dst);
			return ; 
		}
		if (n < 0) {
			return ;
		}
	}
}

unsigned long CB_Modi(CBigInt *src, unsigned long A){
	if (src->m_nLength == 1) {
		return src->m_ulValue[0] % A;
	}
	unsigned __int64 div;
	unsigned long carry = 0;
	for (int i = src->m_nLength - 1; i >= 0; i--) {
		div = src->m_ulValue[i];
		div += carry * 0x100000000;
		carry = (unsigned long)(div % A);
	}
	return carry;
}

/****************************************************************************************
最大公约数
调用形式：CB_Gcd(src, A, dst)
返回值：无，dst 被赋值为 gcd(src, A)
****************************************************************************************/
void CB_Gcd(CBigInt *src, CBigInt *A, CBigInt *dst) {
	int n = CB_Cmp(src, A);
	if (n < 0) {
		CB_Gcd(A, src, dst);
		return;
	}
	CBigInt tmp, A_;
	CB_Mov(A, &A_);
	CB_Mov(src, dst);
	while (1) {
		if (CB_Cmpi(&A_, 0) == 0) {
			break;
		}
		CB_Mod(dst, &A_, &tmp);
		CB_Mov(&A_, dst);
		CB_Mov(&tmp, &A_);
	}
}


/****************************************************************************************
最小公倍数
调用形式：CB_Lcm(src, A, dst)
返回值：无，dst 被赋值为 lcm(src, A)
****************************************************************************************/
void CB_Lcm(CBigInt *src, CBigInt *A, CBigInt *dst) {
	CBigInt mul, gcd;
	CB_Gcd(src, A, &gcd);
	CB_Mul(src, A, &mul);
	CB_Div(&mul, &gcd, dst);
}

/****************************************************************************************
扩展欧几里的算法
调用形式：CB_ExtGcd(src, A, x, y)
返回值：无，计算 src*x + A*y = gcd(src, A)中的 x与y的整数解
****************************************************************************************/
void CB_ExtGcd(CBigInt *src, CBigInt *A, CBigInt *x, CBigInt *y) {
	int n = CB_Cmp(src, A);
	if (n < 0) {
		CB_ExtGcd(A, src, y, x);
		return;
	}

	if (CB_Cmpi(A, 0) == 0) {
		CB_Movi(1, x);
		CB_Movi(0, y);
		return;
	}
	else {
		CBigInt x1, y1, tmp;
		CB_Mod(src, A, &tmp);
		CB_ExtGcd(A, &tmp, &x1, &y1);
		
		CB_Div(src, A, &tmp);
		CB_S_Mul(&tmp, &y1);

		CB_Mov(&y1, x);
		CB_Sub(&x1, &tmp, y);
		return;
	}
}

/****************************************************************************************
大数开方
调用形式：CB_Sqrt(src, dst)
返回值：无，dst 被赋值为 src 的算术平方根
****************************************************************************************/
void CB_Sqrt(CBigInt *src, CBigInt *dst) {
	unsigned long m, n;
	n = src->m_ulValue[src->m_nLength - 1];
	n = (unsigned long)sqrt((double)n);
	if (src->m_nLength == 1) {
		CB_Movi(n, dst);
		return;
	}
	CBigInt M, N, tmp;
	CB_Movi(0, &M);
	CB_Movi(0, &N);
	m = n + 1;
	N.m_nLength = src->m_nLength / 2;
	M.m_nLength = N.m_nLength;
	if (src->m_nLength & 1) {
		M.m_nLength++;
		N.m_nLength++;
		M.m_ulValue[M.m_nLength - 1] = m;
		N.m_ulValue[N.m_nLength - 1] = n;
	}
	else {
		M.m_ulValue[M.m_nLength - 1] = (m << 16);
		N.m_ulValue[N.m_nLength - 1] = (n << 16);
	}
	CB_Add(&M, &N, dst);
	CB_S_Divi(dst, 2);
	while (1) {
		CB_Mul(dst, dst, &tmp);
		if (CB_Cmp(src, &tmp) < 0) {
			CB_Mov(dst, &M);
		}
		else {
			CB_Mov(dst, &N);
		}
		CB_Sub(&M, &N, dst);
		if (dst->m_ulValue[0] == 1 && dst->m_nLength == 1) {
			CB_Mov(&N, dst);
			return;
		}
		CB_Add(&M, &N, dst);
		CB_S_Divi(dst, 2);
	}
}


/****************************************************************************************
大数开方
调用形式：CB_Power(src, n, dst)
返回值：无，dst 被赋值为 src 的 n 次方
****************************************************************************************/
void CB_Power(CBigInt *src, unsigned int n, CBigInt *dst) {
	if (n == 0) {
		CB_Movi(1, dst);
		return;
	}
	CB_Mov(src, dst);
	if (n == 1) {
		return;
	}
	int mod = n % 2;
	CBigInt tmp;
	n = n - mod;
	while (n > 1) {
		CB_Mul(dst, dst, &tmp);
		CB_Mov(&tmp, dst);
		n = n / 2;
	}
	if (mod) {
		CB_S_Mul(dst, src);
	}
}

/****************************************************************************************
从字符串按2进制到36进制格式输入到大数
调用格式：CB_Get(input, len, radix, dst)
返回值：N被赋值为相应大数
****************************************************************************************/
void CB_Get(unsigned char *input, unsigned int len, unsigned int radix, CBigInt *dst) {
	int k;
	CB_Movi(0, dst);
	for (unsigned int i = 0; i < len; i++) {
    	CB_S_Muli(dst, radix);
		if (input[i] >= '0' && input[i] <= '9') {
			k = input[i] - 48;
		}else if ((input[i] >= 'A') && (input[i] <= 'Z')) {
			k = input[i] - 55;
		} else if ((input[i] >= 'a') && (input[i] <= 'z')) {
			k = input[i] - 87;
		} else {
			k = 0;
		}
		CB_S_Addi(dst, k);
	}
}

/****************************************************************************************
将大数按2进制到36进制格式输出为字符串
调用格式：CB_Put(src, output, radix)
返回值：无，参数str被赋值为N的sys进制字符串
****************************************************************************************/
void CB_Put(CBigInt *src, unsigned char *output, unsigned int len, unsigned int radix) {
	if (src->m_nLength == 1 && src->m_ulValue[0] == 0) { 
	    if(len < 2){
    	    return ;
	    }
		output[0] = '0';
		output[1] = '\0';
		return;
	}
	int a;
	unsigned int idx = 0;
	char ch;
	CBigInt tmp;
	CB_Mov(src, &tmp);
	while (tmp.m_ulValue[tmp.m_nLength - 1] > 0)
	{
		a = CB_Modi(&tmp, radix);
		ch = CharTable[a];
	    if(idx < len - 1){
    	    output[idx] = ch;
	    }
		CB_S_Divi(&tmp, radix);
		idx += 1;
	}
    if(idx < len - 1){
	    output[idx] = '\0';
    }
	a = (idx + 1) / 2;
	for (int i = 0; i < a; i++) {
		ch = output[i];
		output[i] = output[idx - 1 - i];
		output[idx - 1 - i] = ch;
	}
}

/****************************************************************************************
求模逆，即解同余方程NX%A=1，亦即解不定方程NX-AY=1的最小整数解
调用方式：CB_ModInv(src, A, dst)
返回值：无 dst,满足：(src * dst) % A=1
****************************************************************************************/
void CB_ModInv(CBigInt *src, CBigInt *A, CBigInt *dst) {
	CBigInt M, E, Y, I, J;
	int x, y;
	CB_Mov(A, &M);
	CB_Mov(src, &E);
	CB_Movi(0, dst);
	CB_Movi(1, &Y);
	x = y = 1;
	while (E.m_nLength != 1 || E.m_ulValue[0] != 0) {
		CB_Div(&M, &E, &I);
		CB_Mod(&M, &E, &J);
		CB_Mov(&E, &M);
		CB_Mov(&J, &E);
		CB_Mov(&Y, &J);
		CB_S_Mul(&Y, &I);
		if (x == y) {
			if (CB_Cmp(dst, &Y) >= 0) {
				CB_S_Sub_Reverse(&Y, dst);
			} else {
				CB_S_Sub(&Y, dst);
				y = 0; 
			}
		} else { 
			CB_S_Add(&Y, dst);
			x = 1 - x; 
			y = 1 - y; 
		}
		CB_Mov(&J, dst);
	}
	if (x == 0) {
		CB_S_Sub_Reverse(dst, A);
	}
}

/****************************************************************************************
求模乘
调用方式：CB_ModMul(src, A, B, dst)
返回值：无 dst=src * A % B
****************************************************************************************/
void CB_ModMul(CBigInt *src, CBigInt *A, CBigInt *B, CBigInt *dst) {
	unsigned long n = src->m_ulValue[src->m_nLength - 1];
	CB_Muli(A, n, dst);
	int i, j;
	CBigInt tmp;
	CB_S_Mod(dst, B);
	for (i = src->m_nLength - 2; i >= 0; i--) {
		for (j = dst->m_nLength; j > 0; j--) {
			dst->m_ulValue[j] = dst->m_ulValue[j - 1];
		}
		dst->m_ulValue[0] = 0;
		dst->m_nLength++;
		CB_Muli(A, src->m_ulValue[i], &tmp);
		CB_S_Add(dst, &tmp);
		CB_S_Mod(dst, B);
	}
}

/****************************************************************************************
求蒙哥马利模乘
调用方式：CB_MonPro(src, A, B, n, dst)，（2**(k-1) < B < 2**k，R=2**k，R*R'%B=1，n*B[0]%0x100000000=-1）
返回值：无 dst=src*A*R'%B
****************************************************************************************/
void CB_MonPro(CBigInt *src, CBigInt *A, CBigInt *B, unsigned long n, CBigInt *dst) {
	unsigned long T[BI_MAXLEN * 2];
	unsigned int i, j, k, idx;
	unsigned long m, carry;
	unsigned __int64 sum;
	CB_Movi(0, dst);

	memset(T, 0, sizeof(T));
	k = B->m_nLength;
	for (i = 0; i < k; i++) {
		carry = 0;
		for (j = 0; j < k; j++) {
			idx = i + j;
			sum = (unsigned __int64)(A->m_ulValue[i]) * src->m_ulValue[j] + T[idx] + carry;
			T[idx] = (unsigned long)sum;
			carry = (unsigned long)(sum >> 32);
		}
		T[i + k] = carry;
	}
	for (i = 0; i < k; i++) {
		carry = 0;
		m = T[i] * n;
		for (j = 0; j < k; j++) {
			idx = i + j;
			sum = (unsigned __int64)(B->m_ulValue[j]) * m + T[idx] + carry;
			T[idx] = (unsigned long)sum;
			carry = (unsigned long)(sum >> 32);
		}
		for (j = i + k; j < k * 2; j++) {
			sum = T[j];
			sum += carry;
			T[j] = (unsigned long)sum;
			carry = (unsigned long)(sum >> 32);
			if (carry == 0) {
				break;
			}
		}
	}
	T[k * 2] = carry;
	dst->m_nLength = k + 1;
	for (i = 0; i <= k; i++) {
		dst->m_ulValue[i] = T[i + k];
	}
	while (dst->m_ulValue[dst->m_nLength - 1] == 0) {
		dst->m_nLength--;
	}
	if (CB_Cmp(dst, B) >= 0) {
		CB_S_Sub(dst, B);
	}
}

/****************************************************************************************
求模幂
调用方式：CB_ModExp(src, A, B, dst)
返回值：无 dst=src**A%B
****************************************************************************************/
void CB_ModExp(CBigInt *src, CBigInt *A, CBigInt *B, CBigInt *dst) {
	CBigInt Y;
	int i, k;
	unsigned long n;
	CB_Movi(0, &Y);

	k = A->m_nLength * 32 - 32;
	n = A->m_ulValue[A->m_nLength - 1];
	while (n) { 
		n = n >> 1; 
		k++; 
	}
	Y.m_nLength = 2;
	Y.m_ulValue[1] = 1;
	CB_Movi(B->m_ulValue[0], dst);
	CB_S_ModInv(dst, &Y);
	CB_S_Sub_Reverse(dst, &Y);
	n = dst->m_ulValue[0];
	CB_Movi(0, &Y);
	Y.m_nLength = B->m_nLength + 1;
	Y.m_ulValue[Y.m_nLength - 1] = 1;
	CB_Sub(&Y, B, dst);
	CB_ModMul(src, dst, B, &Y);
	for (i = k - 1; i >= 0; i--) {
		CB_S_MonPro(dst, dst, B, n);
		if (A->m_ulValue[i >> 5] >> (i & 31) & 1) {
			CB_S_MonPro(dst, &Y, B, n);
		}
	}
	CB_Movi(1, &Y);
	CB_S_MonPro(dst, &Y, B, n);
}

/****************************************************************************************
测试素数
调用方式：CB_TestPrime(src)
返回值：若N为素数，返回0，否则返回最小质因数，若质因数不可知，返回1
****************************************************************************************/
int CB_TestPrime(CBigInt *src) {
	unsigned int PrimeTableMax = PrimeTable[PrimeTableLen - 1] * PrimeTable[PrimeTableLen - 1];

	int i, pass;
	if ((src->m_ulValue[0] & 1) == 0) {
		return 2;
	}
	for (i = 0; i < PrimeTableLen; i++) { 
		if (CB_Modi(src, PrimeTable[i]) == 0) {
			return PrimeTable[i];
		}
	}
	if (src->m_nLength == 1 && src->m_ulValue[0] < PrimeTableMax) {
		return 0;
	}
	CBigInt S, A, I, K;
	CB_Mov(src, &K);
	K.m_ulValue[0]--;
	for (i = 0; i < 5; i++) {
		pass = 0;
		CB_Movi(rand(), &A);
		CB_Mov(&K, &S);
		while ((S.m_ulValue[0] & 1) == 0) {
			CB_S_Divi(&S, 2);
			CB_ModExp(&A, &S, src, &I);
			if (CB_Cmp(&I, &K) == 0) { 
				pass = 1; 
				break; 
			}
		}
		if (I.m_nLength == 1 && I.m_ulValue[0] == 1) {
			pass = 1;
		}
		if (pass == 0) {
			return 1;
		}
	}
	return 0;
}

/****************************************************************************************
产生随机素数
调用方法：CB_RandInit(len, dst, mask)
返回值：无 dst 被赋值为一个len位（0x100000000进制长度）的随机数
****************************************************************************************/
void CB_RandInit(int len, CBigInt *dst, int mask) {
	int i;
	CB_Movi(0, dst);
	dst->m_nLength = len;
	for (i = 1; i < dst->m_nLength; i++) {
		dst->m_ulValue[i] = rand() * 0x10000 + rand();
	}
	dst->m_ulValue[dst->m_nLength - 1] = dst->m_ulValue[dst->m_nLength - 1] | 0x80000000;
	dst->m_ulValue[0] = rand() * 0x10000 + rand();
	if (mask > 0) {
		dst->m_ulValue[0] = dst->m_ulValue[0] | mask;
	}
}

/****************************************************************************************
产生随机素数
调用方法：CB_FindPrime(len, dst)
返回值：无 dst 被赋值为一个len位（0x100000000进制长度）的素数
****************************************************************************************/
void CB_FindPrime(int len, CBigInt *dst) {
	int i;
	CB_Movi(0, dst);
	dst->m_nLength = len;
	for (i = 1; i < dst->m_nLength; i++) {
		dst->m_ulValue[i] = rand() * 0x10000 + rand();
	}
	dst->m_ulValue[dst->m_nLength - 1] = dst->m_ulValue[dst->m_nLength - 1] | 0x80000000;
begin:
	dst->m_ulValue[0] = rand() * 0x10000 + rand();
	dst->m_ulValue[0] = dst->m_ulValue[0] | 3;
	for (i = 0; i < PrimeTableLen; i++) { 
		if (CB_Modi(dst, PrimeTable[i]) == 0) {
			goto begin;
		}
	}
	CBigInt S, A;
	CB_Mov(dst, &S);
	S.m_ulValue[0]--;
	for (i = 0; i < S.m_nLength; i++) {
		S.m_ulValue[i] = S.m_ulValue[i] >> 1;
		if (S.m_ulValue[i + 1] & 1) {
			S.m_ulValue[i] = S.m_ulValue[i] | 0x80000000;
		}
	}
	if (S.m_ulValue[S.m_nLength - 1] == 0) {
		S.m_nLength--;
	}

	for (i = 0; i < 5; i++) {
		CB_Movi(rand(), &A);
		CB_S_ModExp(&A, &S, dst);
		A.m_ulValue[0]++;
		if ((A.m_nLength != 1 || A.m_ulValue[0] != 2) && CB_Cmp(dst, &A) != 0) {
			goto begin;
		}
	}
}