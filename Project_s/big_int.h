#ifndef BIG_INT_H
#define BIG_INT_H

#define max_len 70

class big_int
{
public:
    unsigned length;
    unsigned long value[max_len];
    int flag;
    big_int();
    ~big_int() {}

    friend big_int operator +(const big_int& x, const big_int& y);
    friend big_int operator -(const big_int& x, const big_int& y);
    friend big_int operator *(const big_int& x, const big_int& y);
    friend big_int operator /(const big_int& x, const big_int& y);
    friend big_int operator %(const big_int& x, const big_int& y);
    big_int &operator =(const big_int& y);

    friend big_int operator +(const big_int& x, const unsigned long y);
    friend big_int operator -(const big_int& x, const unsigned long y);
    friend big_int operator *(const big_int& x, const unsigned long y);
    friend big_int operator /(const big_int& x, const unsigned long y);
    friend unsigned long operator %(const big_int& x, const unsigned long y);
    big_int &operator =(const unsigned __int64 y);

    friend unsigned long operator !=(const big_int& x, const big_int& y);
    friend unsigned long operator ==(const big_int& x, const big_int& y);
    friend unsigned long operator >=(const big_int& x, const big_int& y);
    friend unsigned long operator <=(const big_int& x, const big_int& y);
    friend unsigned long operator > (const big_int& x, const big_int& y);
    friend unsigned long operator < (const big_int& x, const big_int& y);

    friend unsigned long operator !=(const big_int& x, const unsigned long y);
    friend unsigned long operator ==(const big_int& x, const unsigned long y);
    friend unsigned long operator >=(const big_int& x, const unsigned long y);
    friend unsigned long operator <=(const big_int& x, const unsigned long y);
    friend unsigned long operator > (const big_int& x, const unsigned long y);
    friend unsigned long operator < (const big_int& x, const unsigned long y);

    big_int power_mod(const big_int& x, const big_int& y);
    big_int gcd(const big_int& x, const big_int& y);
};
big_int::big_int()
{
    length = 1;
    for (int i = 0; i<max_len; i++)value[i] = 0;
    flag = 1;
}
big_int& big_int::operator =(const big_int& y)
{
    int i;
    length = y.length;
    for (i = 0; i<max_len; i++)
        value[i] = y.value[i];
    flag = y.flag;
    return *this;
}
big_int& big_int::operator =(const unsigned __int64 y)
{
    int i;
    if (y >= 0x100000000)
    {
        length = 2;
        value[1] = (unsigned long)(y >> 32);
        value[0] = (unsigned long)(y % 0x100000000);
    }
    else
    {
        length = 1;
        value[0] = (unsigned long)y;
    }
    for (i = length; i<max_len; i++)
        value[i] = 0;
    if (y<0)
        flag = 0;
    else
        flag = 1;
    return *this;
}
big_int operator +(const big_int& x, const big_int& y)
{
    big_int z;
    big_int t1, t2;
    if (x == 0 && y == 0)
    {
        z = 0;
        return z;
    }
    if (x.flag != y.flag)
    {
        t1 = x;
        t2 = y;
        if (x >= y)
        {
            t2.flag = x.flag;
            z = t1 - t2;
        }
        else
        {
            t1.flag = y.flag;
            z = t2 - t1;
        }
    }
    else
    {
        z = x;
        unsigned carry = 0;
        unsigned __int64 sum = 0;
        if (z.length<y.length)z.length = y.length;
        for (unsigned i = 0; i<z.length; i++)
        {
            sum = y.value[i];
            sum = sum + z.value[i] + carry;
            z.value[i] = (unsigned long)(sum % 0x100000000);
            carry = (sum >> 32);
        }
        z.value[z.length] = carry;
        z.length += carry;
    }
    return z;
}
big_int operator +(const big_int& x, const unsigned long y)
{
    big_int z;
    z = x;
    unsigned __int64 sum;
    sum = x.value[0];
    sum += y;
    z.value[0] = (unsigned long)(sum % 0x100000000);
    if (sum>0xffffffff)
    {
        unsigned i = 1;
        while (z.value[i] == 0xffffffff)
        {
            z.value[i] = 0;
            i++;
        }
        z.value[i]++;
        if (z.length == i)z.length++;
    }
    return z;
}
big_int operator -(const big_int& x, const big_int& y)
{
    big_int z;
    big_int t1, t2;
    if (x == 0 && y == 0)
    {
        z = 0;
        return z;
    }
    if (x.flag != y.flag)
    {
        t1 = x;
        t2 = y;
        t2.flag = t1.flag;
        z = t1 + t2;
    }
    else
    {
        z = x;
        if (z <= y)
            z = y - x;
        else
        {
            unsigned carry = 0;
            unsigned __int64 num;
            unsigned i;
            for (i = 0; i<x.length; i++)
            {
                if ((x.value[i]>y.value[i]) || ((x.value[i] == y.value[i]) && (carry == 0)))
                {
                    z.value[i] = x.value[i] - carry - y.value[i];
                    carry = 0;
                }
                else
                {
                    num = 0x100000000 + x.value[i];
                    z.value[i] = (unsigned long)((num - carry - y.value[i]) % 0x100000000);
                    carry = 1;
                }
            }
            while (z.value[z.length - 1] == 0)z.length--;
        }
    }
    return z;
}
big_int operator -(const big_int& x, const unsigned long y)
{
    big_int z;
    z = x;
    if (x.value[0] >= y) { z.value[0] -= y; return z; }
    if (x.length == 1) { z = 0; return z; }
    unsigned __int64 num = 0x100000000 + x.value[0];
    z.value[0] = (unsigned long)((num - y) % 0x100000000);
    int i = 1;
    while (z.value[i] == 0) { z.value[i] = 0xffffffff; i++; }
    z.value[i]--;
    if (z.value[i] == 0)z.length--;
    return z;
}
big_int operator *(const big_int& x, const big_int& y)
{
    big_int z;
    big_int t1, t2;
    if (x.flag != y.flag)
    {
        z.flag = x.flag*y.flag;
        t1 = x;
        t2 = y;
        if (x.length == 1)
        {
            t2.flag = z.flag;
            z = t2*x.value[0];
        }
        if (y.length == 1)
        {
            t1.flag = z.flag;
            z = t1*y.value[0];
        }
    }
    else
    {
        unsigned __int64 sum, mul = 0, carry = 0;
        unsigned i, j;
        z.length = x.length + y.length - 1;
        for (i = 0; i<z.length; i++)
        {
            sum = carry;
            carry = 0;
            for (j = 0; j<y.length; j++)
            {
                if (((i - j) >= 0) && ((i - j)<x.length))
                {
                    mul = x.value[i - j];
                    mul *= y.value[j];
                    carry += mul >> 32;
                    mul = mul & 0xffffffff;
                    sum += mul;
                }
            }
            carry += sum >> 32;
            z.value[i] = (unsigned long)(sum % 0x100000000);
        }
        if (carry) { z.length++; z.value[z.length - 1] = carry; }
        z.flag = 1;
    }
    return z;
}
big_int operator *(const big_int& x, const unsigned long y)
{
    big_int z;
    unsigned __int64 mul;
    unsigned long carry = 0;
    z = x;
    for (unsigned i = 0; i<x.length; i++)
    {
        mul = x.value[i];
        mul = mul*y + carry;
        z.value[i] = (unsigned long)(mul % 0x100000000);
        carry = (mul >> 32);
    }
    if (carry) { z.length++; z.value[z.length - 1] = carry; }
    return z;
}
big_int operator /(const big_int& x, const big_int& y)
{
    big_int t1, u;
    if (x.flag != y.flag)
    {
        u.flag = x.flag*y.flag;
        if (y.length == 1)
        {
            t1 = x;
            t1.flag = u.flag;
            u = t1 / y.value[0];
        }
    }
    else
    {
        big_int v, w;
        unsigned i, len;
        unsigned __int64 num, div;
        v = x;
        while (v >= y)
        {
            div = v.value[v.length - 1];
            num = y.value[y.length - 1];
            len = v.length - y.length;
            if ((div == num) && (len == 0)) { u = u + 1; break; }
            if ((div <= num) && len) { len--; div = (div << 32) + v.value[v.length - 2]; }
            div = div / (num + 1);
            w = div;
            if (len)
            {
                w.length += len;
                for (i = w.length - 1; i >= len; i--)w.value[i] = w.value[i - len];
                for (i = 0; i<len; i++)w.value[i] = 0;
            }
            u = u + w;
            v = v - y*w;
        }
        u.flag = 1;
    }
    return u;
}
big_int operator /(const big_int& x, const unsigned long y)
{
    big_int z;
    z = x;
    if (z.length == 1) { z.value[0] = z.value[0] / y; return z; }
    unsigned __int64 div, mul;
    unsigned long carry = 0;
    for (int i = z.length - 1; i >= 0; i--)
    {
        div = carry;
        div = (div << 32) + z.value[i];
        z.value[i] = (div / y);
        mul = (div / y)*y;
        carry = (div - mul);
    }
    if (z.value[z.length - 1] == 0)z.length--;
    return z;
}
big_int operator %(const big_int& x, const big_int& y)
{
    big_int u, v;
    if (y == 1)
    {
        u = 0;
        return u;
    }
    if (y.length == 1)
    {
        u = x%y.value[0];
        return u;
    }
    unsigned __int64 div, num;
    unsigned long carry = 0;
    unsigned i, len;
    u = x;
    while (u >= y)
    {
        div = u.value[u.length - 1];
        num = y.value[y.length - 1];
        len = u.length - y.length;
        if ((div == num) && (len == 0)) { u = u - y; break; }
        if ((div <= num) && len) { len--; div = (div << 32) + u.value[u.length - 2]; }
        div = div / (num + 1);
        v = div;
        v = y*v;
        if (len)
        {
            v.length += len;
            for (i = v.length - 1; i >= len; i--)v.value[i] = v.value[i - len];
            for (i = 0; i<len; i++)v.value[i] = 0;
        }
        u = u - v;
    }
    return u;
}
unsigned long operator %(const big_int& x, const unsigned long y)
{
    if (x.length == 1)return(x.value[0] % y);
    unsigned __int64 div;
    unsigned long carry = 0;
    for (int i = x.length - 1; i >= 0; i--)
    {
        div = x.value[i];
        div += carry * 0x100000000;
        carry = (div%y);
    }
    return carry;
}
unsigned long operator !=(const big_int& x, const big_int& y)
{
    if (x.length != y.length) return 1;
    for (int i = x.length - 1; i >= 0; i--)
    {
        if (x.value[i] != y.value[i])return 1;
    }
    return 0;
}
unsigned long operator ==(const big_int& x, const big_int& y)
{
    if (x != y) return 0;
    else return 1;
}
unsigned long operator >=(const big_int& x, const big_int& y)
{
    if (x.length>y.length) return 1;
    if (x.length<y.length) return 0;
    for (int i = x.length - 1; i >= 0; i--)
    {
        if (x.value[i]>y.value[i])return 1;
        if (x.value[i]<y.value[i])return 0;
    }
    return 1;
}
unsigned long operator <=(const big_int& x, const big_int& y)
{
    if (x.length<y.length) return 1;
    if (x.length>y.length) return 0;
    for (int i = x.length - 1; i >= 0; i--)
    {
        if (x.value[i]<y.value[i])return 1;
        if (x.value[i]>y.value[i])return 0;
    }
    return 1;
}
unsigned long operator > (const big_int& x, const big_int& y)
{
    if (x >= y&&x != y) return 1;
    else return 0;
}
unsigned long operator < (const big_int& x, const big_int& y)
{
    if (x <= y&&x != y) return 1;
    else return 0;
}

unsigned long operator !=(const big_int& x, const unsigned long  y)
{
    if (x.length>1) return 1;
    if (x.value[0] != y) return 1;
    return 0;
}
unsigned long operator ==(const big_int& x, const unsigned long y)
{
    if (x != y) return 0;
    else return 1;
}
unsigned long operator >=(const big_int& x, const unsigned long y)
{
    if (x.length>1 || x.value[0] >= y) return 1;
    else return 0;
}
unsigned long operator <=(const big_int& x, const unsigned long y)
{
    if (x.length>1 || x.value[0]>y) return 0;
    else return 1;
}
unsigned long operator > (const big_int& x, const unsigned long y)
{
    if (x >= y&&x != y) return 1;
    else return 0;
}
unsigned long operator < (const big_int& x, const unsigned long y)
{
    if (x <= y&&x != y) return 1;
    else return 0;
}
big_int gcd(const big_int& x, const big_int& y)
{
    if (y == 0)
        return x;
    else
        return gcd(y, x%y);
}
big_int big_int::power_mod(const big_int& x, const big_int& y)
{
    big_int u, v, z;
    u = 1;
    v = x;
    z = *this;
    while (v != 0)
    {
        if (v % 2 == 1)
            u = (u*z) % y;
        z = (z*z) % y;
        v = v / 2;
    }
    return u;
}



#endif // BIG_INT_H
