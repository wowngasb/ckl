/*****************************************************************
大数运算库源文件：BigInt.cpp
作者：fangle@monternet.com
版本：1.2 (2003.5.13)
说明：适用于MFC，1024位RSA运算
*****************************************************************/
#include "stdafx.h"
#include "BigInt.h"

//构造大数对象并初始化为零
CBigInt::CBigInt()
{
    m_nLength=1;
    for(int i=0;i<BI_MAXLEN;i++)m_ulValue[i]=0;
}

//解构大数对象
CBigInt::~CBigInt()
{
}

/****************************************************************************************
大数比较
调用方式：N.Cmp(A)
返回值：若N<A返回-1；若N=A返回0；若N>A返回1
****************************************************************************************/
int CBigInt::Cmp(CBigInt& A)
{
    if(m_nLength>A.m_nLength)return 1;
    if(m_nLength<A.m_nLength)return -1;
    for(int i=m_nLength-1;i>=0;i--)
    {
        if(m_ulValue[i]>A.m_ulValue[i])return 1;
        if(m_ulValue[i]<A.m_ulValue[i])return -1;
    }
    return 0;
}

/****************************************************************************************
大数赋值
调用方式：N.Mov(A)
返回值：无，N被赋值为A
****************************************************************************************/
void CBigInt::Mov(CBigInt& A)
{
    m_nLength=A.m_nLength;
    for(int i=0;i<BI_MAXLEN;i++)m_ulValue[i]=A.m_ulValue[i];
}

void CBigInt::Mov(unsigned __int64 A)
{
    if(A>0xffffffff)
    {
        m_nLength=2;
        m_ulValue[1]=(unsigned long)(A>>32);
        m_ulValue[0]=(unsigned long)A;
    }
    else
    {
        m_nLength=1;
        m_ulValue[0]=(unsigned long)A;
    }
    for(int i=m_nLength;i<BI_MAXLEN;i++)m_ulValue[i]=0;
}

/****************************************************************************************
大数相加
调用形式：N.Add(A)
返回值：N+A
****************************************************************************************/
CBigInt CBigInt::Add(CBigInt& A)
{
    CBigInt X;
    X.Mov(*this);
    unsigned carry=0;
    unsigned __int64 sum=0;
    if(X.m_nLength<A.m_nLength)X.m_nLength=A.m_nLength;
    for(unsigned i=0;i<X.m_nLength;i++)
    {
        sum=A.m_ulValue[i];
		sum=sum+X.m_ulValue[i]+carry;
        X.m_ulValue[i]=(unsigned long)sum;
        carry=(unsigned)(sum>>32);
    }
    X.m_ulValue[X.m_nLength]=carry;
    X.m_nLength+=carry;
    return X;
}

CBigInt CBigInt::Add(unsigned long A)
{
    CBigInt X;
    X.Mov(*this);
    unsigned __int64 sum;
    sum=X.m_ulValue[0];
	sum+=A;
    X.m_ulValue[0]=(unsigned long)sum;
    if(sum>0xffffffff)
    {
        unsigned i=1;
        while(X.m_ulValue[i]==0xffffffff){X.m_ulValue[i]=0;i++;}
        X.m_ulValue[i]++;
        if(X.m_nLength==i)X.m_nLength++;
    }
    return X;
}

/****************************************************************************************
大数相减
调用形式：N.Sub(A)
返回值：N-A
****************************************************************************************/
CBigInt CBigInt::Sub(CBigInt& A)
{
    CBigInt X;
    X.Mov(*this);
    if(X.Cmp(A)<=0){X.Mov(0);return X;}
    unsigned carry=0;
    unsigned __int64 num;
	unsigned i;
    for(i=0;i<m_nLength;i++)
    {
        if((m_ulValue[i]>A.m_ulValue[i])||((m_ulValue[i]==A.m_ulValue[i])&&(carry==0)))
        {
            X.m_ulValue[i]=m_ulValue[i]-carry-A.m_ulValue[i];
            carry=0;
        }
        else
        {
            num=0x100000000+m_ulValue[i];
            X.m_ulValue[i]=(unsigned long)(num-carry-A.m_ulValue[i]);
            carry=1;
        }
    }
    while(X.m_ulValue[X.m_nLength-1]==0)X.m_nLength--;
    return X;
}

CBigInt CBigInt::Sub(unsigned long A)
{
    CBigInt X;
    X.Mov(*this);
    if(X.m_ulValue[0]>=A){X.m_ulValue[0]-=A;return X;}
    if(X.m_nLength==1){X.Mov(0);return X;}
    unsigned __int64 num=0x100000000+X.m_ulValue[0];
    X.m_ulValue[0]=(unsigned long)(num-A);
    int i=1;
    while(X.m_ulValue[i]==0){X.m_ulValue[i]=0xffffffff;i++;}
    X.m_ulValue[i]--;
    if(X.m_ulValue[i]==0)X.m_nLength--;
    return X;
}

/****************************************************************************************
大数相乘
调用形式：N.Mul(A)
返回值：N*A
****************************************************************************************/
CBigInt CBigInt::Mul(CBigInt& A)
{
    if(A.m_nLength==1)return Mul(A.m_ulValue[0]);
	CBigInt X;
	unsigned __int64 sum,mul=0,carry=0;
	unsigned i,j;
	X.m_nLength=m_nLength+A.m_nLength-1;
    for(i=0;i<X.m_nLength;i++)
	{
		sum=carry;
		carry=0;
		for(j=0;j<A.m_nLength;j++)
		{
            if(((i-j)>=0)&&((i-j)<m_nLength))
			{
				mul=m_ulValue[i-j];
				mul*=A.m_ulValue[j];
			    carry+=mul>>32;
				mul=mul&0xffffffff;
				sum+=mul;
			}
        }
		carry+=sum>>32;
		X.m_ulValue[i]=(unsigned long)sum;
	}
	if(carry){X.m_nLength++;X.m_ulValue[X.m_nLength-1]=(unsigned long)carry;}
    return X;
}

CBigInt CBigInt::Mul(unsigned long A)
{
    CBigInt X;
    unsigned __int64 mul;
    unsigned long carry=0;
    X.Mov(*this);
    for(unsigned i=0;i<m_nLength;i++)
    {
        mul=m_ulValue[i];
        mul=mul*A+carry;
        X.m_ulValue[i]=(unsigned long)mul;
        carry=(unsigned long)(mul>>32);
    }
    if(carry){X.m_nLength++;X.m_ulValue[X.m_nLength-1]=carry;}
    return X;
}

/****************************************************************************************
大数相除
调用形式：N.Div(A)
返回值：N/A
****************************************************************************************/
CBigInt CBigInt::Div(CBigInt& A)
{
    if(A.m_nLength==1)return Div(A.m_ulValue[0]);
    CBigInt X,Y,Z;
    unsigned i,len;
    unsigned __int64 num,div;
    Y.Mov(*this);
    while(Y.Cmp(A)>=0)
    {       
		div=Y.m_ulValue[Y.m_nLength-1];
		num=A.m_ulValue[A.m_nLength-1];
		len=Y.m_nLength-A.m_nLength;
		if((div==num)&&(len==0)){X.Mov(X.Add(1));break;}
		if((div<=num)&&len){len--;div=(div<<32)+Y.m_ulValue[Y.m_nLength-2];}
		div=div/(num+1);
		Z.Mov(div);
		if(len)
		{
			Z.m_nLength+=len;
			for(i=Z.m_nLength-1;i>=len;i--)Z.m_ulValue[i]=Z.m_ulValue[i-len];
			for(i=0;i<len;i++)Z.m_ulValue[i]=0;
		}
		X.Mov(X.Add(Z));
        Y.Mov(Y.Sub(A.Mul(Z)));
    }
    return X;
}

CBigInt CBigInt::Div(unsigned long A)
{
    CBigInt X;
    X.Mov(*this);
    if(X.m_nLength==1){X.m_ulValue[0]=X.m_ulValue[0]/A;return X;}
    unsigned __int64 div,mul;
    unsigned long carry=0;
    for(int i=X.m_nLength-1;i>=0;i--)
    {
        div=carry;
        div=(div<<32)+X.m_ulValue[i];
        X.m_ulValue[i]=(unsigned long)(div/A);
        mul=(div/A)*A;
        carry=(unsigned long)(div-mul);
    }
    if(X.m_ulValue[X.m_nLength-1]==0)X.m_nLength--;
    return X;
}

/****************************************************************************************
大数求模
调用形式：N.Mod(A)
返回值：N%A
****************************************************************************************/
CBigInt CBigInt::Mod(CBigInt& A)
{
    CBigInt X,Y;
	unsigned __int64 div,num;
    unsigned long carry=0;
	unsigned i,len;
	int n;
	n=Cmp(A);
	if(n<0)return A;
	if(n=0)return X;
    X.Mov(*this);
    while(1)
    {
		div=X.m_ulValue[X.m_nLength-1];
		num=A.m_ulValue[A.m_nLength-1];
		len=X.m_nLength-A.m_nLength;
		if((div<=num)&&len){len--;div=(div<<32)+X.m_ulValue[X.m_nLength-2];}
		div=div/(num+1);
		Y.Mov(div);
		Y.Mov(A.Mul(Y));
		if(len)
		{
			Y.m_nLength+=len;
			for(i=Y.m_nLength-1;i>=len;i--)Y.m_ulValue[i]=Y.m_ulValue[i-len];
			for(i=0;i<len;i++)Y.m_ulValue[i]=0;
		}
        X.Mov(X.Sub(Y));
		n=X.Cmp(A);
		if(n==0){X.Mov(0);return X;}
		if(n<0)return X;
    }
}

unsigned long CBigInt::Mod(unsigned long A)
{
    if(m_nLength==1)return(m_ulValue[0]%A);
    unsigned __int64 div;
    unsigned long carry=0;
    for(int i=m_nLength-1;i>=0;i--)
    {
        div=m_ulValue[i];
		div+=carry*0x100000000;
        carry=(unsigned long)(div%A);
    }
    return carry;
}

/****************************************************************************************
大数开方
调用形式：N.Sqr()
返回值：N的算术平方根
****************************************************************************************/
CBigInt CBigInt::Sqrt()
{
    CBigInt X,M,N;
	unsigned long m,n;
	n=m_ulValue[m_nLength-1];
	n=(unsigned long)sqrt((double)n);
	m=n+1;
	if(m_nLength==1){X.Mov(n);return X;}
	N.m_nLength=m_nLength/2;
	M.m_nLength=N.m_nLength;
	if(m_nLength&1)
	{
		M.m_nLength++;
		N.m_nLength++;
		M.m_ulValue[M.m_nLength-1]=m;
		N.m_ulValue[N.m_nLength-1]=n;
	}
	else 
	{
		M.m_ulValue[M.m_nLength-1]=(m<<16);
		N.m_ulValue[N.m_nLength-1]=(n<<16);
	}
	X.Mov(M.Add(N));
	X.Mov(X.Div(2));
	while(1)
	{
		if(Cmp(X.Mul(X))<0)M.Mov(X);
        else N.Mov(X);
		X.Mov(M.Sub(N));
		if((X.m_ulValue[0]==1)&&(X.m_nLength==1))return N;
  	    X.Mov(M.Add(N));
	    X.Mov(X.Div(2));
	}
}

/****************************************************************************************
从字符串按2进制到36进制格式输入到大数
调用格式：N.Get(str,sys)
返回值：N被赋值为相应大数
****************************************************************************************/
void CBigInt::Get(CString& str, unsigned int system)
{
    int len=str.GetLength(),k;
    Mov(0);
    for(int i=0;i<len;i++)
    {
       Mov(Mul(system));
       if((str[i]>='0')&&(str[i]<='9'))k=str[i]-48;
       else if((str[i]>='A')&&(str[i]<='Z'))k=str[i]-55;
       else if((str[i]>='a')&&(str[i]<='z'))k=str[i]-87;
       else k=0;
       Mov(Add(k));
    }
}

/****************************************************************************************
将大数按2进制到36进制格式输出为字符串
调用格式：N.Put(str,sys)
返回值：无，参数str被赋值为N的sys进制字符串
****************************************************************************************/
void CBigInt::Put(CString& str, unsigned int system)
{
    if((m_nLength==1)&&(m_ulValue[0]==0)){str="0";return;}
	str="";
    int a;
    char ch;
    CBigInt X;
    X.Mov(*this);
    while(X.m_ulValue[X.m_nLength-1]>0)
    {
        a=X.Mod(system);
        ch=CharTable[a];
        str.Insert(0,ch);
        X.Mov(X.Div(system));
    }
}

/****************************************************************************************
求模逆，即解同余方程NX%A=1，亦即解不定方程NX-AY=1的最小整数解
调用方式：N.ModInv(A)
返回值：X,满足：NX%A=1
****************************************************************************************/
CBigInt CBigInt::ModInv(CBigInt& A)
{
	CBigInt M,E,X,Y,I,J;
    int x,y;
	M.Mov(A);
	E.Mov(*this);
	X.Mov(0);
	Y.Mov(1);
	x=y=1;
	while((E.m_nLength!=1)||(E.m_ulValue[0]!=0))
	{
		I.Mov(M.Div(E));
		J.Mov(M.Mod(E));
		M.Mov(E);
		E.Mov(J);
		J.Mov(Y);
		Y.Mov(Y.Mul(I));
		if(x==y)
		{
		    if(X.Cmp(Y)>=0)Y.Mov(X.Sub(Y));
			else{Y.Mov(Y.Sub(X));y=0;}
		}
		else{Y.Mov(X.Add(Y));x=1-x;y=1-y;}
		X.Mov(J);
	}
	if(x==0)X.Mov(A.Sub(X));
	return X;
}

/****************************************************************************************
求模乘
调用方式：N.ModMul(A,B)
返回值：X=N*A%B
****************************************************************************************/
CBigInt CBigInt::ModMul(CBigInt& A, CBigInt& B)
{
	int i,j;
	CBigInt X;
	X.Mov(A.Mul(m_ulValue[m_nLength-1]));
	X.Mov(X.Mod(B));
    for(i=m_nLength-2;i>=0;i--)
	{          
		for(j=X.m_nLength;j>0;j--)X.m_ulValue[j]=X.m_ulValue[j-1];
		X.m_ulValue[0]=0;
		X.m_nLength++;
		X.Mov(X.Add(A.Mul(m_ulValue[i])));
		X.Mov(X.Mod(B));
	}
    return X;
}

/****************************************************************************************
求蒙哥马利模乘
调用方式：N.MonPro(A,B,n)，（2**(k-1)<B<2**k，R=2**k，R*R'%B=1，n*B[0]%0x100000000=-1） 
返回值：X=N*A*R'%B
****************************************************************************************/
CBigInt CBigInt::MonPro(CBigInt& A, CBigInt& B, unsigned long n)
{
	CBigInt X;
	unsigned long T[BI_MAXLEN*2];
	unsigned i,j,k;
	unsigned long m,carry;
	unsigned __int64 sum;
	for(i=0;i<BI_MAXLEN*2;i++)T[i]=0;
	k=B.m_nLength;
	for(i=0;i<k;i++)
	{
		carry=0;
		for(j=0;j<k;j++)
		{
			sum=A.m_ulValue[i];
			sum=sum*m_ulValue[j]+T[i+j]+carry;
			T[i+j]=(unsigned long)sum;
			carry=(unsigned long)(sum>>32);
		}
		T[i+k]=carry;
	}
	for(i=0;i<k;i++)
	{
		carry=0;
		m=T[i]*n;
		for(j=0;j<k;j++)
		{
			sum=B.m_ulValue[j];
			sum=sum*m+T[i+j]+carry;
			T[i+j]=(unsigned long)sum;
			carry=(unsigned long)(sum>>32);
		}
		for(j=i+k;j<k*2;j++)
		{
			sum=T[j];
			sum+=carry;
			T[j]=(unsigned long)sum;
			carry=(unsigned long)(sum>>32);
			if(carry==0)break;
		}
	}
	T[k*2]=carry;
	X.m_nLength=k+1;
	for(i=0;i<=k;i++)X.m_ulValue[i]=T[i+k];
	while(X.m_ulValue[X.m_nLength-1]==0)X.m_nLength--;
	if(X.Cmp(B)>=0)X.Mov(X.Sub(B));
	return X;
}

/****************************************************************************************
求模幂
调用方式：N.ModExp(A,B)
返回值：X=N**A%B
****************************************************************************************/
CBigInt CBigInt::ModExp(CBigInt& A, CBigInt& B)
{
	CBigInt X,Y;
	int i,k;
	unsigned long n;
	k=A.m_nLength*32-32;
	n=A.m_ulValue[A.m_nLength-1];
	while(n){n=n>>1;k++;}
	Y.m_nLength=2;
    Y.m_ulValue[1]=1;
	X.Mov(B.m_ulValue[0]);
	X.Mov(X.ModInv(Y));
	X.Mov(Y.Sub(X));
	n=X.m_ulValue[0];
	Y.Mov(0);
	Y.m_nLength=B.m_nLength+1;
	Y.m_ulValue[Y.m_nLength-1]=1;
	X.Mov(Y.Sub(B));
    Y.Mov(ModMul(X,B));
	for(i=k-1;i>=0;i--)
	{
        X.Mov(X.MonPro(X,B,n));
		if((A.m_ulValue[i>>5]>>(i&31))&1)X.Mov(X.MonPro(Y,B,n));
	}
    Y.Mov(1);
	X.Mov(X.MonPro(Y,B,n));
    return X;
}

/****************************************************************************************
测试素数
调用方式：N.TestPrime()
返回值：若N为素数，返回0，否则返回最小质因数，若质因数不可知，返回1
****************************************************************************************/
int CBigInt::TestPrime()
{
    unsigned i,pass;
	if((m_ulValue[0]&1)==0)return 2;
    for(i=0;i<1230;i++){if(Mod(PrimeTable[i])==0)return PrimeTable[i];}
	if((m_nLength==1)&&(m_ulValue[0]<100180081))return 0;
    CBigInt S,A,I,K;
    K.Mov(*this);
	K.m_ulValue[0]--;
    for(i=0;i<5;i++)
    {
        pass=0;
        A.Mov(rand());
		S.Mov(K);
        while((S.m_ulValue[0]&1)==0)
		{
            S.Mov(S.Div(2));
			I.Mov(A.ModExp(S,*this));
			if(I.Cmp(K)==0){pass=1;break;}
		}
		if((I.m_nLength==1)&&(I.m_ulValue[0]==1))pass=1;
		if(pass==0)return 1;
	}
    return 0;
}

/****************************************************************************************
产生随机素数
调用方法：N.FindPrime(bits)
返回值：N被赋值为一个bits位（0x100000000进制长度）的素数
****************************************************************************************/
void CBigInt::FindPrime(int bits)
{
    unsigned i;
    m_nLength=bits;
	for(i=1;i<m_nLength;i++)m_ulValue[i]=rand()*0x10000+rand();
	m_ulValue[m_nLength-1]=m_ulValue[m_nLength-1]|0x80000000;
begin:
    m_ulValue[0]=rand()*0x10000+rand();
	m_ulValue[0]=m_ulValue[0]|3;
    for(i=0;i<500;i++){if(Mod(PrimeTable[i])==0)goto begin;}
    CBigInt S,A;
    S.Mov(*this);
	S.m_ulValue[0]--;
	for(i=0;i<S.m_nLength;i++)
	{
	    S.m_ulValue[i]=S.m_ulValue[i]>>1;
	    if(S.m_ulValue[i+1]&1)S.m_ulValue[i]=S.m_ulValue[i]|0x80000000;
	}
	if(S.m_ulValue[S.m_nLength-1]==0)S.m_nLength--;
    for(i=0;i<5;i++)
	{
        A.Mov(rand());
		A.Mov(A.ModExp(S,*this));
		A.m_ulValue[0]++;
	    if(((A.m_nLength!=1)||(A.m_ulValue[0]!=2))&&(Cmp(A)!=0))goto begin;
	}
}