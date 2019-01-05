#ifndef CKL_BIGINT_H
#define CKL_BIGINT_H


#define BI_MAXLEN 80  //  80 ���� 40
#define DEC 10
#define HEX 16

typedef struct
{
	int m_nLength;  //��¼0x10000000���Ƶ�λ����0-80֮�䣬�൱��2���Ƶ� 0-2560(80 * 32) λ
	unsigned long m_ulValue[BI_MAXLEN];   //��¼ÿһλ�ġ����֡�
} CBigInt;

#if __cplusplus
extern "C" {
#endif


//�Ƚ�����������С src A �Ƚ�
int CB_Cmp(CBigInt *src, CBigInt *A);

//��������ֵΪ��һ������  A -> dst
void CB_Mov(CBigInt *A, CBigInt *dst);

//��������ֵΪ�������ܹ������κ����γ��������  A -> dst
void CB_Movi(unsigned __int64 A, CBigInt *dst);

//�������������ĺ�  src + A ������� dst
void CB_Add(CBigInt *src, CBigInt *A, CBigInt *dst);

//��������ͨ�������  src + A ������� dst
void CB_Addi(CBigInt *src, unsigned long A, CBigInt *dst);

//�������������Ĳ�  src - A ������� dst
void CB_Sub(CBigInt *src, CBigInt *A, CBigInt *dst);

//��������ͨ�������  src - A ������� dst
void CB_Subi(CBigInt *src, unsigned long A, CBigInt *dst);

//�������������Ļ�   src * A ������� dst
void CB_Mul(CBigInt *src, CBigInt *A, CBigInt *dst);

//��������ͨ�������   src * A ������� dst
void CB_Muli(CBigInt *src, unsigned long A, CBigInt *dst);

//����������������   src / A ������� dst
void CB_Div(CBigInt *src, CBigInt *A, CBigInt *dst);

//��������ͨ�������   src / A ������� dst
void CB_Divi(CBigInt *src, unsigned long A, CBigInt *dst);

//���������������������   src % A ������� dst
void CB_Mod(CBigInt *src, CBigInt *A, CBigInt *dst);

//��������ͨ���������ģ   src % A ���ؽ��
unsigned long CB_Modi(CBigInt *src, unsigned long A);

//�����������ƽ����   Sqrt(src) ������� dst
void CB_Sqrt(CBigInt *src, CBigInt *dst);

//�����������ƽ����   Power(src, n, dst) ������� dst
void CB_Power(CBigInt *src, unsigned int n, CBigInt *dst);

/* 
 * �������� ������ԭ���ݵ�Ԫ
 */

//�������������ĺ�  src + A ������� src
void CB_S_Add(CBigInt *src, CBigInt *A);

//���غ�����֧�ִ�������ͨ�������  src + A ������� src
void CB_S_Addi(CBigInt *src, unsigned long A);

//�������������Ĳ�  src - A ������� src
void CB_S_Sub(CBigInt *src, CBigInt *A);

//�������������Ĳ�  A - src ������� src
void CB_S_Sub_Reverse(CBigInt *src, CBigInt *A);

//��������ͨ�������  src - A ������� src
void CB_S_Subi(CBigInt *src, unsigned long A);

//�������������Ļ�   src * A ������� src
void CB_S_Mul(CBigInt *src, CBigInt *A);

//��������ͨ�������   src * A ������� src
void CB_S_Muli(CBigInt *src, unsigned long A);

//����������������   src / A ������� src
void CB_S_Div(CBigInt *src, CBigInt *A);

//����������������   A / src ������� src
void CB_S_Div_Reverse(CBigInt *src, CBigInt *A);

//��������ͨ�������   src / A ������� src
void CB_S_Divi(CBigInt *src, unsigned long A);

//���������������������   src % A ������� src
void CB_S_Mod(CBigInt *src, CBigInt *A);

//���������������������   A % src ������� src
void CB_S_Mod_Reverse(CBigInt *src, CBigInt *A);

//�����������ƽ����   Sqrt(src) ������� src
void CB_S_Sqrt(CBigInt *src);

//�����������ƽ����   Power(src, n) ������� src
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

#if __cplusplus
}
#endif

#endif /* CKL_BIGINT_H */