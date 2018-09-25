#ifndef RSA_H
#define RSA_H

#include "miller_rabin.h"

//将大数以十六进制格式输出到字符串str
void out_put(BIGNUM &x, char *str)
{
    int length=0;
    if((x.length==1)&&(x.value[0]==0)){str[0]='0';str[1]='\0';return;}
    char* t="0123456789ABCDEF";
    int a;
    char ch;
    BIGNUM temp;
    temp=x;
    while(temp.value[temp.length-1]>0)
    {
        length++;
        temp=temp/16;
    }
    str[length]='\0';
    temp=x;
    while(temp.value[temp.length-1]>0)
    {
        a=temp%16;
        ch=t[a];
        str[--length]=ch;
        temp=temp/16;
    }
}

void key(BIGNUM &p, BIGNUM &q, BIGNUM &e,BIGNUM &n,BIGNUM &d)
{
    BIGNUM b;   //b=ψn
    char ch[500]="";
    cout<<"正在生成p..."<<endl;
    p=get_big_prime();
    cout<<"p生成完毕"<<endl;
    cout<<"p = ";
    out_put(p,ch);
    puts(ch);

    cout<<endl<<"正在生成q..."<<endl;
    q=get_big_prime();
    cout<<"q生成完毕"<<endl;
    cout<<"q = ";
    out_put(q,ch);
    puts(ch);

    n=p*q;
    cout<<endl<<"公共密钥n:"<<endl;
    out_put(n,ch);
    puts(ch);

    b=(p-1)*(q-1);
    cout<<endl<<"产生公共密钥e..."<<endl;
    e=rand();
    while(gcd(e,b)!=1)
        e=rand();
    cout<<"e产生完毕"<<endl;
    cout<<"e = ";
    cout<<e.value[0]<<endl;

    cout<<endl<<"计算私有密钥d..."<<endl;
    ex_euclid(e,b,d);
    cout<<"d计算完毕"<<endl<<endl;
    cout<<"d = ";
    out_put(d,ch);
    puts(ch);
}

void encryption(string st,BIGNUM &e,BIGNUM &d,BIGNUM &n, BIGNUM &M,BIGNUM &C,char ch[500])
{
     FILE *fp1,*fp2;
   // BIGNUM M,C;
    //char ch[500]="";
    char buf[10000]="";
    //unsigned long  c[1000]={0};
    char cha;
    int k=0,ck=0;
    int i,j;
   // FILE *fp1,*fp2;
   // fp1=fopen("a.txt","r");
   // fread(buf,sizeof(char),sizeof(buf),fp1);
   // fp2=fopen("b.txt","a");
    //puts(buf);
   // key(e,n,d);
    for(i = 0; i< st.length(); i++){
        buf[i] = st[i];
    }


    fp1=fopen("a.txt","r");
    fread(buf,sizeof(char),sizeof(buf),fp1);
    fp2=fopen("b.txt","a");
    while(buf[k])
    {
        for(i=0;i<31;i++)
        {
            for(j=0;j<4;j++)
            {
                cha=buf[k++];
                if(j==0)
                    M.value[i]=(unsigned long)cha;
                else
                    M.value[i]=M.value[i]+(unsigned long)(cha<<(8*j));
            }
        }
        M.length=++i;
        C=M.power_mod(e,n);
        out_put(C,ch);
        cout<<endl<<"加密后的密文为:"<<endl;
        puts(ch);
        //
        /*for(int p=C.length-1;p>=0;p--)
        {
        c[ck++]=C.value[p];
        }*/
        memset(ch,0,sizeof(char)*500);

     //   fwrite(&C,sizeof(BIGNUM),1,fp2);//

        memset(ch,0,sizeof(char)*500);

        fwrite(&C,sizeof(BIGNUM),1,fp2);//
        //M=0;
        //C=0;
    }

    cout<<endl;

    fclose(fp1);
   fclose(fp2);
}


#endif // RSA_H
