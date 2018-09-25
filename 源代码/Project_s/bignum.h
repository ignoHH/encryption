#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdlib.h>
#include <string>
#include <math.h>
#include <iostream>
#define max_len 70
using namespace std;

class BIGNUM
{
public:
    unsigned length;
    unsigned long value[max_len];
    int flag;
    BIGNUM()
    {
        length=1;
        for(int i=0;i<max_len;i++)value[i]=0;
        flag = 1;
    }
    ~BIGNUM(){}
    friend BIGNUM operator +( const BIGNUM& num_1, const BIGNUM& num_2 );
    friend BIGNUM operator -( const BIGNUM& num_1, const BIGNUM& num_2 );
    friend BIGNUM operator *( const BIGNUM& num_1, const BIGNUM& num_2 );
    friend BIGNUM operator /( const BIGNUM& num_1, const BIGNUM& num_2 );
    friend BIGNUM operator %( const BIGNUM& num_1, const BIGNUM& num_2 );
    BIGNUM &operator =(const BIGNUM& num_2 );

    friend BIGNUM operator +( const BIGNUM& num_1, const unsigned long num_2 );
    friend BIGNUM operator -( const BIGNUM& num_1, const unsigned long num_2 );
    friend BIGNUM operator *( const BIGNUM& num_1, const unsigned long num_2 );
    friend BIGNUM operator /( const BIGNUM& num_1, const unsigned long num_2 );
    friend unsigned long operator %( const BIGNUM& num_1, const unsigned long num_2 );
    BIGNUM &operator =( const unsigned __int64 num_2 ); //2^64

    friend unsigned long operator !=( const BIGNUM& num_1, const BIGNUM& num_2 );
    friend unsigned long operator ==( const BIGNUM& num_1, const BIGNUM& num_2 );
    friend unsigned long operator >=( const BIGNUM& num_1, const BIGNUM& num_2 );
    friend unsigned long operator <=( const BIGNUM& num_1, const BIGNUM& num_2 );
    friend unsigned long operator > ( const BIGNUM& num_1, const BIGNUM& num_2 );
    friend unsigned long operator < ( const BIGNUM& num_1, const BIGNUM& num_2 );

    friend unsigned long operator !=( const BIGNUM& num_1, const unsigned long num_2 );
    friend unsigned long operator ==( const BIGNUM& num_1, const unsigned long num_2 );
    friend unsigned long operator >=( const BIGNUM& num_1, const unsigned long num_2 );
    friend unsigned long operator <=( const BIGNUM& num_1, const unsigned long num_2 );
    friend unsigned long operator > ( const BIGNUM& num_1, const unsigned long num_2 );
    friend unsigned long operator < ( const BIGNUM& num_1, const unsigned long num_2 );

    BIGNUM power_mod( const BIGNUM& n, const BIGNUM& m );
    BIGNUM gcd( const BIGNUM& num_1, const BIGNUM& num_2 );
};
BIGNUM& BIGNUM::operator =( const BIGNUM& num_2)
{
    int i;
    length = num_2.length;
    for(i=0;i<max_len;i++)
        value[i]=num_2.value[i];
    flag=num_2.flag;
    return *this;
}
BIGNUM& BIGNUM::operator =( const unsigned __int64 num_2 )
{
    int i;
    if(num_2>=0x100000000)
    {
        length = 2;
        value[1]=(unsigned long)(num_2>>32);
        value[0]=(unsigned long)(num_2%0x100000000);
    }
    else
    {
        length=1;
        value[0]=(unsigned long)num_2;
    }
    for(i=length;i<max_len;i++)
        value[i]=0;
    if(num_2<0)
        flag = 0;
    else
        flag = 1;
    return *this;
}
BIGNUM operator +( const BIGNUM& num_1, const BIGNUM& num_2 )
{
    BIGNUM result;
    BIGNUM t1, t2;
    if(num_1==0&&num_2==0)
    {
        result=0;
        return result;
    }
    if(num_1.flag!=num_2.flag)
    {
        t1=num_1;
        t2=num_2;
        if(num_1>=num_2)
        {
            t2.flag=num_1.flag;
            result=t1-t2;
        }
        else
        {
            t1.flag=num_2.flag;
            result=t2-t1;
        }
    }
    else
    {
        result = num_1;
        unsigned carry=0;//进位
        unsigned __int64 sum=0;
        if(result.length<num_2.length)
            result.length=num_2.length;
        for(unsigned i=0;i<result.length;i++)
        {
            sum=num_2.value[i];
            sum=sum+result.value[i]+carry;
            result.value[i]=(unsigned long)(sum%0x100000000);
            carry=(sum>>32);
        }
        result.value[result.length]=carry;
        result.length+=carry;
    }
    return result;
}
BIGNUM operator +( const BIGNUM& num_1, const unsigned long num_2 )
{
    BIGNUM result;
    result = num_1;
    unsigned __int64 sum;
    sum=num_1.value[0];
    sum+=num_2;
    result.value[0]=(unsigned long)(sum%0x100000000);
    if(sum>0xffffffff)
    {
        unsigned i=1;
        while(result.value[i]==0xffffffff)
        {
            result.value[i]=0;
            i++;
        }
        result.value[i]++;
        if(result.length==i)
            result.length++;
    }
    return result;
}
BIGNUM operator -( const BIGNUM& num_1, const BIGNUM& num_2 )
{
    BIGNUM result;
    BIGNUM t1,t2;
    if(num_1==0&&num_2==0)
    {
        result=0;
        return result;
    }
    if(num_1.flag!=num_2.flag)
    {
        t1=num_1;
        t2=num_2;
        t2.flag=t1.flag;
        result=t1+t2;
    }
    else
    {
        result = num_1;
        if(result <= num_2)
            result=num_2-num_1;
        else
        {
            unsigned carry=0;
            unsigned __int64 num;
            unsigned i;
            for(i=0;i<num_1.length;i++)
            {
                if((num_1.value[i]>num_2.value[i])||((num_1.value[i]==num_2.value[i])&&(carry==0)))
                {
                    result.value[i]=num_1.value[i]-carry-num_2.value[i];
                    carry=0;
                }
                else
                {
                    num=0x100000000+num_1.value[i];
                    result.value[i]=(unsigned long)((num-carry-num_2.value[i])%0x100000000);
                    carry=1;
                }
            }
            while(result.value[result.length-1]==0)
                result.length--;
        }
    }
    return result;
}
BIGNUM operator -( const BIGNUM& num_1, const unsigned long num_2 )
{
    BIGNUM result;
    result = num_1;
    if(num_1.value[0]>=num_2)
    {
        result.value[0]-=num_2;
        return result;
    }
    if(num_1.length==1)
    {
        result = 0;
        return result;
    }
    unsigned __int64 num=0x100000000+num_1.value[0];
    result.value[0]=(unsigned long)((num-num_2)%0x100000000);
    int i=1;
    //进位
    while(result.value[i]==0)
    {
        result.value[i]=0xffffffff;
        i++;
    }
    result.value[i]--;
    if(result.value[i]==0)
        result.length--;
    return result;
}
BIGNUM operator *( const BIGNUM& num_1, const BIGNUM& num_2 ) ////
{
    BIGNUM result;
    BIGNUM t1,t2;
    if(num_1.flag!=num_2.flag)
    {
        result.flag=num_1.flag*num_2.flag;
        t1=num_1;
        t2=num_2;
        if(num_1.length==1)
        {
            t2.flag=result.flag;
            result = t2*num_1.value[0];
        }
        if(num_2.length==1)
        {
            t1.flag=result.flag;
            result = t1*num_2.value[0];
        }
    }
    else
    {
        unsigned __int64 sum,mul=0,carry=0;
        unsigned i,j;
        result.length=num_1.length+num_2.length-1;
        for(i=0;i<result.length;i++)
        {
            sum=carry;
            carry=0;
            for(j=0;j<num_2.length;j++)
            {
                if(((i-j)>=0)&&((i-j)<num_1.length))
                {
                    mul=num_1.value[i-j];
                    mul*=num_2.value[j];
                    carry+=mul>>32;
                    mul=mul&0xffffffff;
                    sum+=mul;
                }
            }
            carry+=sum>>32;
            result.value[i]=(unsigned long)(sum%0x100000000);
        }
        if(carry)
        {
            result.length++;
            result.value[result.length-1]=carry;
        }
        result.flag=1;
    }
    return result;
}
BIGNUM operator *( const BIGNUM& num_1, const unsigned long num_2 )
{
    BIGNUM result;
    unsigned __int64 mul;
    unsigned long carry=0;
    result = num_1;
    for(unsigned i=0;i<num_1.length;i++)
    {
        mul=num_1.value[i];
        mul=mul*num_2+carry;
        result.value[i]=(unsigned long)(mul%0x100000000);
        carry=(mul>>32);
    }
    if(carry)
    {
        result.length++;
        result.value[result.length-1]=carry;
    }
    return result;
}
BIGNUM operator /( const BIGNUM& num_1, const BIGNUM& num_2 )
{
    BIGNUM t1, result;
    if(num_1.flag!=num_2.flag)
    {
        result.flag=num_1.flag*num_2.flag;
        if(num_2.length==1)
        {
            t1=num_1;
            t1.flag=result.flag;
            result=t1/num_2.value[0];
        }
    }
    else
    {
        BIGNUM temp,w;
        unsigned i,len;
        unsigned __int64 num,div;
        temp = num_1;
        while(temp >= num_2)
        {
            div=temp.value[temp.length-1];//被除数首位
            num=num_2.value[num_2.length-1];//除数首位
            len=temp.length-num_2.length;
            if((div==num)&&(len==0))  //首位= length=
            {
                result=result+1;
                break;
            }
            if((div<=num)&&len)
            {
                len--;
                div=(div<<32)+temp.value[temp.length-2];
            }
            div=div/(num+1);
            w = div;
            if(len)
            {
                w.length+=len;
                for(i=w.length-1;i>=len;i--)
                    w.value[i]=w.value[i-len];
                for(i=0;i<len;i++)
                    w.value[i]=0;
            }
            result=result+w;
            temp=temp-num_2*w;
        }
        result.flag=1;
    }
    return result;
}
BIGNUM operator /( const BIGNUM& num_1, const unsigned long num_2 )
{
    BIGNUM result;
    result=num_1;
    if(result.length==1)
    {
        result.value[0]=result.value[0]/num_2;
        return result;
    }
    unsigned __int64 div,mul;
    unsigned long carry=0;
    for(int i=result.length-1;i>=0;i--)
    {
        div=carry;
        div=(div<<32)+result.value[i];
        result.value[i]=(div/num_2);
        mul=(div/num_2)*num_2;
        carry=(div-mul);
    }
    if(result.value[result.length-1]==0)
        result.length--;
    return result;
}
BIGNUM operator %( const BIGNUM& num_1, const BIGNUM& num_2 )
{
    BIGNUM result,v;
    if(num_2==1)
    {
        result=0;
        return result;
    }
    if(num_2.length==1)
    {
        result=num_1%num_2.value[0];
        return result;
    }
    unsigned __int64 div,num;
    unsigned long carry=0;
    unsigned i,len;
    result = num_1;
    while(result >= num_2)
    {
        div=result.value[result.length-1];
        num=num_2.value[num_2.length-1];
        len=result.length-num_2.length;
        if((div==num)&&(len==0))
        {
            result=result-num_2;
            break;
        }
        if((div<=num)&&len)
        {
            len--;
            div=(div<<32)+result.value[result.length-2];
        }
        div=div/(num+1);
        v=div;
        v=num_2*v;
        if(len)
        {
            v.length+=len;
            for(i=v.length-1;i>=len;i--)
                v.value[i]=v.value[i-len];
            for(i=0;i<len;i++)
                v.value[i]=0;
        }
        result=result-v;
    }
    return result;
}
unsigned long operator %( const BIGNUM& num_1, const unsigned long num_2 )
{
    if(num_1.length==1)return(num_1.value[0]%num_2);
    unsigned __int64 div;
    unsigned long carry=0;
    for(int i=num_1.length-1;i>=0;i--)
    {
        div=num_1.value[i];
        div+=carry*0x100000000;
        carry=(div%num_2);
    }
    return carry;
}
unsigned long operator !=( const BIGNUM& num_1, const BIGNUM& num_2 )
{
    if(num_1.length!=num_2.length)
        return 1;
    for(int i=num_1.length-1;i>=0;i--)
    {
        if(num_1.value[i]!=num_2.value[i])
            return 1;
    }
    return 0;
}
unsigned long operator ==( const BIGNUM& num_1, const BIGNUM& num_2 )
{
    if(num_1!= num_2)
        return 0;
    else
        return 1;
}
unsigned long operator >=( const BIGNUM& num_1, const BIGNUM& num_2 )
{
    if(num_1.length>num_2.length)
        return 1;
    if(num_1.length<num_2.length)
        return 0;
    for(int i=num_1.length-1;i>=0;i--)
    {
        if(num_1.value[i]>num_2.value[i])
            return 1;
        if(num_1.value[i]<num_2.value[i])
            return 0;
    }
    return 1;
}
unsigned long operator <=( const BIGNUM& num_1, const BIGNUM& num_2 )
{
    if(num_1.length<num_2.length)
        return 1;
    if(num_1.length>num_2.length)
        return 0;
    for(int i=num_1.length-1;i>=0;i--)
    {
        if(num_1.value[i]<num_2.value[i])
            return 1;
        if(num_1.value[i]>num_2.value[i])
            return 0;
    }
    return 1;
}
unsigned long operator > ( const BIGNUM& num_1, const BIGNUM& num_2 )
{
    if(num_1>=num_2&&num_1!=num_2)
        return 1;
    else
        return 0;
}
unsigned long operator < ( const BIGNUM& num_1, const BIGNUM& num_2 )
{
    if(num_1<=num_2&&num_1!=num_2)
        return 1;
    else
        return 0;
}

unsigned long operator !=( const BIGNUM& num_1, const unsigned long  num_2 )
{
    if(num_1.length>1)
        return 1;
    if(num_1.value[0]!=num_2)
        return 1;
    return 0;
}
unsigned long operator ==( const BIGNUM& num_1, const unsigned long num_2 )
{
    if(num_1!= num_2)
        return 0;
    else
        return 1;
}
unsigned long operator >=( const BIGNUM& num_1, const unsigned long num_2 )
{
    if(num_1.length>1||num_1.value[0]>=num_2)
        return 1;
    else
        return 0;
}
unsigned long operator <=( const BIGNUM& num_1, const unsigned long num_2 )
{
    if(num_1.length>1||num_1.value[0]>num_2)
        return 0;
    else
        return 1;
}
unsigned long operator > ( const BIGNUM& num_1, const unsigned long num_2 )
{
    if(num_1>=num_2&&num_1!=num_2)
        return 1;
    else
        return 0;
}
unsigned long operator < ( const BIGNUM& num_1, const unsigned long num_2 )
{
    if(num_1<=num_2&&num_1!=num_2)
        return 1;
    else
        return 0;
}

//广义欧几里得除法 (a,b)=(b,r2)=(rn,0)
BIGNUM gcd( const BIGNUM& num_1, const BIGNUM& num_2 )
{
    BIGNUM temp1;
    temp1=num_1;
    temp1.flag=1;
    if(num_2==0)
    {
        return temp1;
    }
    else
        return gcd(num_2,num_1%num_2);

}
  //模重复平方计算法
BIGNUM BIGNUM::power_mod( const BIGNUM& n, const BIGNUM& m )  //num_2 m
{
    BIGNUM a,temp,b;
    a=1;
    temp=n;
    b=*this;

    while(temp!=0)
    {
        if(temp%2==1)
            a=(a*b)%m;
        else
            a=a;
        b=(b*b)%m;
        temp=temp/2;
    }
    return a;
}

#endif // BIGNUM_H
