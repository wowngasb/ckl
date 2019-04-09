#ifndef CKL_BIGINT_H
#define CKL_BIGINT_H

#include "stdint.h"

#define BI_MAXLEN 80  //  80 ���� 40
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
	uint16_t m_nLength;  //��¼0x10000000���Ƶ�λ����0-80֮�䣬�൱��2���Ƶ�  0-2560(80 * 32) λ
	int16_t m_signed;  // ���ݷ���  -1 ��ʾ����  0 ��ʾ����
	uint32_t m_ulValue[BI_MAXLEN];   //��¼ÿһλ�ġ����֡�
} CBigInt;

// ����Ϊ1   m_signed Ϊ0  ��ʾ ���� 0   ��ʱӦ���������ݶ�Ϊ 0

#if __cplusplus
extern "C" {
#endif

int CB_IsZero(CBigInt *src);

int CB_IsPositive(CBigInt *src);

int CB_IsNegative(CBigInt *src);

//������� ȡ��  ������� dst
void CB_Neg(CBigInt *src, CBigInt *dst);

//��������ľ���ֵ  ������� dst
void CB_Abs(CBigInt *src, CBigInt *dst);

//�Ƚ�����������С src A �Ƚ�
int CB_Cmp(CBigInt *src, CBigInt *A);

int CB_Cmpi(CBigInt *src, int32_t A);
int CB_Cmpu(CBigInt *src, uint32_t A);

//��������ֵΪ��һ������  A -> dst
void CB_Mov(CBigInt *src, CBigInt *dst);

//��������ֵΪ�������ܹ������κ����γ��������  A -> dst
void CB_Movu(uint64_t A, CBigInt *dst);
void CB_Movi(int64_t A, CBigInt *dst);

//�������������ĺ�  src + A ������� dst
void CB_Add(CBigInt *src, CBigInt *A, CBigInt *dst);

//��������ͨ�������  src + A ������� dst
void CB_Addi(CBigInt *src, int32_t A, CBigInt *dst);
void CB_Addu(CBigInt *src, uint32_t A, CBigInt *dst);

//�������������Ĳ�  src - A ������� dst
void CB_Sub(CBigInt *src, CBigInt *A, CBigInt *dst);

//��������ͨ�������  src - A ������� dst
void CB_Subi(CBigInt *src, int32_t A, CBigInt *dst);
void CB_Subu(CBigInt *src, uint32_t A, CBigInt *dst);

//�������������Ļ�   src * A ������� dst
void CB_Mul(CBigInt *src, CBigInt *A, CBigInt *dst);

//��������ͨ�������   src * A ������� dst
void CB_Muli(CBigInt *src, int32_t A, CBigInt *dst);
void CB_Mulu(CBigInt *src, uint32_t A, CBigInt *dst);

//����������������   src / A ������� dst
void CB_Div(CBigInt *src, CBigInt *A, CBigInt *dst);

//��������ͨ�������   src / A ������� dst
void CB_Divi(CBigInt *src, int32_t A, CBigInt *dst);
void CB_Divu(CBigInt *src, uint32_t A, CBigInt *dst);

//���������������������   src % A ������� dst
void CB_Mod(CBigInt *src, CBigInt *A, CBigInt *dst);

//������������ ���Լ��   gcd(src, A) ������� dst
void CB_Gcd(CBigInt *src, CBigInt *A, CBigInt *dst);

//������������ ��С������   lcm(src, A) ������� dst
void CB_Lcm(CBigInt *src, CBigInt *A, CBigInt *dst);

//��չŷ������㷨  ���� src*x + A*y = gcd(src, A)�е� x��y��������
void CB_ExtGcd(CBigInt *src, CBigInt *A, CBigInt *x, CBigInt *y);

//��������ͨ���������ģ   src % A ���ؽ��
int32_t CB_Modi(CBigInt *src, int32_t A);
uint32_t CB_Modu(CBigInt *src, uint32_t A);

//�����������ƽ����   Sqrt(src) ������� dst
void CB_Sqrt(CBigInt *src, CBigInt *dst);

//�����������ƽ����   Power(src, n, dst) ������� dst
void CB_Power(CBigInt *src, unsigned int n, CBigInt *dst);

/* 
 * �������� ������ԭ���ݵ�Ԫ
 */

 //���� ȡ�� ������� src
void CB_S_Neg(CBigInt *src);

//���� ����ֵ ������� src
void CB_S_Abs(CBigInt *src);


//�������������ĺ�  src + A ������� src
void CB_S_Add(CBigInt *src, CBigInt *A);

//���غ�����֧�ִ�������ͨ�������  src + A ������� src
void CB_S_Addi(CBigInt *src, int32_t A);
void CB_S_Addu(CBigInt *src, uint32_t A);

//�������������Ĳ�  src - A ������� src
void CB_S_Sub(CBigInt *src, CBigInt *A);

//�������������Ĳ�  A - src ������� src
void CB_S_Sub_Reverse(CBigInt *src, CBigInt *A);

//��������ͨ�������  src - A ������� src
void CB_S_Subi(CBigInt *src, int32_t A);
void CB_S_Subu(CBigInt *src, uint32_t A);

//�������������Ļ�   src * A ������� src
void CB_S_Mul(CBigInt *src, CBigInt *A);

//��������ͨ�������   src * A ������� src
void CB_S_Muli(CBigInt *src, int32_t A);
void CB_S_Mulu(CBigInt *src, uint32_t A);

//����������������   src / A ������� src
void CB_S_Div(CBigInt *src, CBigInt *A);

//����������������   A / src ������� src
void CB_S_Div_Reverse(CBigInt *src, CBigInt *A);

//��������ͨ�������   src / A ������� src
void CB_S_Divi(CBigInt *src, int32_t A);
void CB_S_Divu(CBigInt *src, uint32_t A);

//���������������������   src % A ������� src
void CB_S_Mod(CBigInt *src, CBigInt *A);

//��������������������� Reverse   A % src ������� src
void CB_S_Mod_Reverse(CBigInt *src, CBigInt *A);

//�����������ƽ����   Sqrt(src) ������� src
void CB_S_Sqrt(CBigInt *src);

//�����������ƽ��   Power(src, n) ������� src
void CB_S_Power(CBigInt *src, unsigned int n);

/*****************************************************************
�������
Get�����ַ�����10���ƻ�16���Ƹ�ʽ���뵽����
Put����������10���ƻ�16���Ƹ�ʽ������ַ���
*****************************************************************/

void CB_Get(unsigned char *input, unsigned int len, unsigned int radix, CBigInt *dst);

void CB_Put(CBigInt *src, unsigned char *output, unsigned int len, unsigned int radix);

/*****************************************************************
RSA�������
ModMul�����������㷨��ģ��
ModInv��ŷ������㷨��ģ��
MonPro���ɸ������㷨��ģ��
ModExp���ɸ������㷨��ģ��
TestPrime�����������㷨������������
FindPrime������ָ�����ȵ����������
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