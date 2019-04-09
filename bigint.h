#ifndef CKL_BIGINT_H
#define CKL_BIGINT_H

#include "stdint.h"

#define BI_MAXLEN 80  //  80 或者 40
#define DEC 10
#define HEX 16

#define AUTO_FIX_ZERO(dst) do{ \
	while (dst->m_nLength > 1 && dst->m_ulValue[dst->m_nLength - 1] == 0) { \
		dst->m_nLength--; \
	} \
	if (dst->m_nLength == 1 && dst->m_ulValue[0] == 0) { dst->m_signed = 0; } \
} while(0);


typedef struct
{
	uint16_t m_nLength;  //记录0x10000000进制的位数，0-80之间，相当于2进制的  0-2560(80 * 32) 位
	int16_t m_signed;  // 数据符号  -1 表示负数  0 表示正数
	uint32_t m_ulValue[BI_MAXLEN];   //记录每一位的“数字”
} CBigInt;

// 长度为1   m_signed 为0  表示 数字 0   此时应该所有数据都为 0

#if __cplusplus
extern "C" {
#endif

int CB_IsZero(CBigInt *src);

int CB_IsPositive(CBigInt *src);

int CB_IsNegative(CBigInt *src);

//计算大数 取负  结果存入 dst
void CB_Neg(CBigInt *src, CBigInt *dst);

//计算大数的绝对值  结果存入 dst
void CB_Abs(CBigInt *src, CBigInt *dst);

//比较两个大数大小 src A 比较
int CB_Cmp(CBigInt *src, CBigInt *A);

int CB_Cmpi(CBigInt *src, int32_t A);
int CB_Cmpu(CBigInt *src, uint32_t A);

//将大数赋值为另一个大数  A -> dst
void CB_Mov(CBigInt *src, CBigInt *dst);

//将大数赋值为编译器能够理解的任何整形常数或变量  A -> dst
void CB_Movu(uint64_t A, CBigInt *dst);
void CB_Movi(int64_t A, CBigInt *dst);

//计算两个大数的和  src + A 结果存入 dst
void CB_Add(CBigInt *src, CBigInt *A, CBigInt *dst);

//大数与普通整数相加  src + A 结果存入 dst
void CB_Addi(CBigInt *src, int32_t A, CBigInt *dst);
void CB_Addu(CBigInt *src, uint32_t A, CBigInt *dst);

//计算两个大数的差  src - A 结果存入 dst
void CB_Sub(CBigInt *src, CBigInt *A, CBigInt *dst);

//大数与普通整数相减  src - A 结果存入 dst
void CB_Subi(CBigInt *src, int32_t A, CBigInt *dst);
void CB_Subu(CBigInt *src, uint32_t A, CBigInt *dst);

//计算两个大数的积   src * A 结果存入 dst
void CB_Mul(CBigInt *src, CBigInt *A, CBigInt *dst);

//大数与普通整数相乘   src * A 结果存入 dst
void CB_Muli(CBigInt *src, int32_t A, CBigInt *dst);
void CB_Mulu(CBigInt *src, uint32_t A, CBigInt *dst);

//计算两个大数的商   src / A 结果存入 dst
void CB_Div(CBigInt *src, CBigInt *A, CBigInt *dst);

//大数与普通整数相除   src / A 结果存入 dst
void CB_Divi(CBigInt *src, int32_t A, CBigInt *dst);
void CB_Divu(CBigInt *src, uint32_t A, CBigInt *dst);

//计算两个大数相除的余数   src % A 结果存入 dst
void CB_Mod(CBigInt *src, CBigInt *A, CBigInt *dst);

//计算两个大数 最大公约数   gcd(src, A) 结果存入 dst
void CB_Gcd(CBigInt *src, CBigInt *A, CBigInt *dst);

//计算两个大数 最小公倍数   lcm(src, A) 结果存入 dst
void CB_Lcm(CBigInt *src, CBigInt *A, CBigInt *dst);

//扩展欧几里的算法  计算 src*x + A*y = gcd(src, A)中的 x与y的整数解
void CB_ExtGcd(CBigInt *src, CBigInt *A, CBigInt *x, CBigInt *y);

//大数与普通整数相除求模   src % A 返回结果
int32_t CB_Modi(CBigInt *src, int32_t A);
uint32_t CB_Modu(CBigInt *src, uint32_t A);

//计算大数算术平方根   Sqrt(src) 结果存入 dst
void CB_Sqrt(CBigInt *src, CBigInt *dst);

//计算大数算术平方根   Power(src, n, dst) 结果存入 dst
void CB_Power(CBigInt *src, unsigned int n, CBigInt *dst);

/* 
 * 辅助函数 结果存回原数据单元
 */

 //大数 取负 结果存入 src
void CB_S_Neg(CBigInt *src);

//大数 绝对值 结果存入 src
void CB_S_Abs(CBigInt *src);


//计算两个大数的和  src + A 结果存入 src
void CB_S_Add(CBigInt *src, CBigInt *A);

//重载函数以支持大数与普通整数相加  src + A 结果存入 src
void CB_S_Addi(CBigInt *src, int32_t A);
void CB_S_Addu(CBigInt *src, uint32_t A);

//计算两个大数的差  src - A 结果存入 src
void CB_S_Sub(CBigInt *src, CBigInt *A);

//计算两个大数的差  A - src 结果存入 src
void CB_S_Sub_Reverse(CBigInt *src, CBigInt *A);

//大数与普通整数相减  src - A 结果存入 src
void CB_S_Subi(CBigInt *src, int32_t A);
void CB_S_Subu(CBigInt *src, uint32_t A);

//计算两个大数的积   src * A 结果存入 src
void CB_S_Mul(CBigInt *src, CBigInt *A);

//大数与普通整数相乘   src * A 结果存入 src
void CB_S_Muli(CBigInt *src, int32_t A);
void CB_S_Mulu(CBigInt *src, uint32_t A);

//计算两个大数的商   src / A 结果存入 src
void CB_S_Div(CBigInt *src, CBigInt *A);

//计算两个大数的商   A / src 结果存入 src
void CB_S_Div_Reverse(CBigInt *src, CBigInt *A);

//大数与普通整数相除   src / A 结果存入 src
void CB_S_Divi(CBigInt *src, int32_t A);
void CB_S_Divu(CBigInt *src, uint32_t A);

//计算两个大数相除的余数   src % A 结果存入 src
void CB_S_Mod(CBigInt *src, CBigInt *A);

//计算两个大数相除的余数 Reverse   A % src 结果存入 src
void CB_S_Mod_Reverse(CBigInt *src, CBigInt *A);

//计算大数算术平方根   Sqrt(src) 结果存入 src
void CB_S_Sqrt(CBigInt *src);

//计算大数算术平方   Power(src, n) 结果存入 src
void CB_S_Power(CBigInt *src, unsigned int n);

/*****************************************************************
输入输出
Get，从字符串按10进制或16进制格式输入到大数
Put，将大数按10进制或16进制格式输出到字符串
*****************************************************************/

void CB_Get(unsigned char *input, unsigned int len, unsigned int radix, CBigInt *dst);

void CB_Put(CBigInt *src, unsigned char *output, unsigned int len, unsigned int radix);

/*****************************************************************
RSA相关运算
ModMul，布莱克雷算法求模乘
ModInv，欧几里德算法求模逆
MonPro，蒙哥马利算法求模乘
ModExp，蒙哥马利算法求模幂
TestPrime，拉宾米勒算法进行素数测试
FindPrime，产生指定长度的随机大素数
*****************************************************************/
void CB_ModInv(CBigInt *src, CBigInt *A, CBigInt *dst);
void CB_ModMul(CBigInt *src, CBigInt *A, CBigInt *B, CBigInt *dst);
void CB_MonPro(CBigInt *src, CBigInt *A, CBigInt *B, unsigned long n, CBigInt *dst);
void CB_ModExp(CBigInt *src, CBigInt *A, CBigInt *B, CBigInt *dst);
int CB_TestPrime(CBigInt *src);
void CB_FindPrime(int bits, CBigInt *dst);

void CB_RandInit(int len, CBigInt *dst, int mask);

#if __cplusplus
}
#endif

#endif /* CKL_BIGINT_H */