//
//  main.cpp
//  DES_encipher&decipher
//
//  Created by mov on 08/04/2018.
//  Copyright © 2018 wjp. All rights reserved.
//

#include <iostream>
#include <vector>
#include "table.h"
#include <string>
using namespace std;

//将输入二进制串保存到向量中
vector<bool> convert_vector_bool(string cipher)
{
    vector<bool> re;
    for(int i=0; i<cipher.size();i++){
        if(cipher[i]=='0')
            re.push_back(false);
        else if(cipher[i]=='1')
            re.push_back(true);
    }
    return re;
}
//将字符串转化成二进制形式；
vector<bool> convert(string key)
{
    vector<bool> key_bool;
    for(int i=0; i<key.size();i++)
    {
        unsigned num= key[i];
        vector<bool> tem;
        while(num>0)
        {
            tem.insert(tem.begin(), num%2);
            num/=2;
        }
        
        //解决开头是0的问题,每个char应该有8位
        long a=8-tem.size();
        while(a>0)
        {
            tem.insert(tem.begin(), 0);
            a=8-tem.size();
        }
        
        key_bool.insert(key_bool.end(), tem.begin(),tem.end());
    }
    return key_bool;
}

////将二进制形式转化为字符串；
string convert_2(vector<bool> bool_text)
{
    string re;
    for(int i=0; i<(bool_text.size()/8); i++)
    {
        unsigned int num=0;
        
         num+= 128* bool_text[i*8] +64*bool_text[i*8+1]+ 32* bool_text[i*8+2]+16*bool_text[i*8+3]+8*bool_text[i*8+4]+ 4*bool_text[i*8+5]+ 2*bool_text[i*8+6]+ bool_text[i*8+7];
        re+=(char)num;
    }
    //cout<<re.size()<<endl;
    return re;
    
}

//循环左移位；
vector<bool> left_shift(vector<bool> a, int step)
{
    vector<bool> b;
    long length = a.size();
    if(length!=28)
        cout<<"bug bug"<<endl;
    for(long i=0; i<length; i++)
        b.push_back(a[(i+step)%length]);
    return b;
}

//处理原始秘钥字符串，返回16轮的秘钥；
vector<vector<bool> >  process_key(string key)
{
    if(key.size()!=7)
        cout<<"bug key"<<endl;
    
    vector<bool> key_bool=convert(key);

    vector<vector<bool> > keys;
    
    //秘钥首次置换，存放在key_tem中；
    vector<bool> key_tem;
    for (int i=0; i<56; i++) {
        key_tem.push_back(key_bool[pc_1[i]-1]);
    }
    
    
    for(int k=0; k<16; k++)
    {
        int step= step_vec[k];
        vector<bool> C, D,key_;
        //将秘钥分成C和D
        for(int i=0; i<key_tem.size();i++)
            if(i<28)
                C.push_back(key_tem[i]);
            else
                D.push_back(key_tem[i]);
        
        //训环移位
        //cout<<C.size()<<endl;
        C=left_shift(C, step);
        D=left_shift(D, step);
        
        //将key_tem清空
        while(key_tem.size() != 0)
            key_tem.pop_back();
        //cout<<key_tem.size()<<endl;
        //将CD合并
        key_tem.insert(key_tem.end(), C.begin(),C.end());
        key_tem.insert(key_tem.end(), D.begin(),D.end());
        
        //压缩置换
        for(int i=0; i<48; i++)
            key_.push_back(key_tem[pc_2[i]-1]);
        keys.push_back(key_);
    }
    return keys;
}


//抑或运算
vector<bool> XOR(vector<bool> a, vector<bool> b)
{
    vector<bool> c;
    if(a.size()!=b.size())
    {
        cout<<"bug! bug!"<<endl;
        return c;
    }
    
    for(int i=0; i<a.size(); i++)
        if(a[i]!=b[i])
            c.push_back(true);
        else
            c.push_back(false);
    return c;
}

//f函数
vector<bool> f(vector<bool> R,vector<bool> key)
{
    vector<bool> R_E, SUM, after_S, Return;
    //扩张
    for(int i=0; i<48; i++)
        R_E.push_back( R[ E[i]-1 ] );
    
    SUM=XOR(R_E, key);
    //S-box
    for(int i=0; i<8; i++)
    {
        
        unsigned int col, row, num;
        bool r1= SUM[i*6+0], r2=SUM[i*6+5];
        bool c1= SUM[i*6+1], c2=SUM[i*6+2], c3=SUM[i*6+3], c4=SUM[i*6+4];
        
        //从相应的S盒子中取出值；
        row= r1*2+ r2;
        col= c1*8+ c2*4+ c3*2+ c4;
        //cout<<row<<" "<<col<<endl;
        num= S[i][row][col];
        
        //将值转化成二进制
        vector<bool> tem;
        while(num>0)
        {
            //cout<<num%2<<endl;
            tem.insert(tem.begin(), num%2);
            num/=2;
            //cout<<num<<endl;
        }
        //解决开头是0的问题
        long a=4-tem.size();
        while(a>0)
        {
            tem.insert(tem.begin(),0);
            a=4-tem.size();
        }
        
        after_S.insert(after_S.end(), tem.begin(),tem.end());
    }
    //P-box
    for(int i=0; i<32; i++)
        Return.push_back(after_S[P[i]-1]);
    return Return;
}


vector<bool> DES_encipher(vector<bool> text_bool, string key)
{
    //得到十六轮需要的秘钥
    vector<vector<bool> > keys=process_key(key);
    
    //初始置换
    vector<bool> text_in;
    for(int i=0; i<64; i++)
        text_in.push_back(text_bool[IP[i]-1]);
    
    //定义并获取L和R
    vector<bool> L,R;
    for(int i=0; i<64; i++)
        if(i<32)
            L.push_back(text_in[i]);
        else
            R.push_back(text_in[i]);
    
    //16轮循环
    for(int k=0; k<16; k++)
    {
        vector<bool> L_tem=L,R_tem=R;
        //获取左半部分
        L= R_tem;
        //获取右半部分
        //cout<<R.size()<<endl;
        R= XOR(f(R_tem,keys[k]),L_tem);
    }
    //将text_in清空
    while(text_in.size() != 0)
        text_in.pop_back();
    //先插入右侧，再插入左侧，这里包含了左右交换的步骤
    text_in.insert(text_in.end(), R.begin(),R.end());
    text_in.insert(text_in.end(), L.begin(),L.end());
    
    //逆初始置换
    vector<bool> re;
    for(int i=0; i<64; i++)
        re.push_back(text_in[IP_1[i]-1]);
    
    return re;
}

vector<bool> DES_decipher(vector<bool> text_bool, string key)
{
    //得到十六轮需要的秘钥
    vector<vector<bool> > keys=process_key(key);
    
    //初始置换
    vector<bool> text_in;
    for(int i=0; i<64; i++)
        text_in.push_back(text_bool[IP[i]-1]);
    //cout<<IP[63]<<endl;

    //定义并获取L和R
    vector<bool> L,R;
    for(int i=0; i<64; i++)
        if(i<32)
            L.push_back(text_in[i]);
        else
            R.push_back(text_in[i]);
    
    //16轮循环
    //解密时，秘钥需要倒置；
    for(int k=0; k<16; k++)
    {
        vector<bool> L_tem=L,R_tem=R;
        //获取左半部分
        L=R_tem;
        //获取右半部分
        R=XOR(f(R_tem,keys[15-k]),L_tem);
    }
    //将text_in清空
    while(text_in.size() != 0)
        text_in.pop_back();
    //先插入右侧，再插入左侧，这里包含了左右交换的步骤
    text_in.insert(text_in.end(), R.begin(),R.end());
    text_in.insert(text_in.end(), L.begin(),L.end());
    
    //逆初始置换
    vector<bool> re;
    for(int i=0; i<64; i++)
        re.push_back(text_in[IP_1[i]-1]);
    
    return re;
}

//加密文本
vector<bool> encipher_text(string text,string key)
{
    int n=8 - text.size()%8;
    if(n!=8)
    {
        while(n--)
        text+=' ';
    }
    vector<bool> re;
    int i;
    
    //cout<<text.size()<<endl;
    for(i=0; i<text.size(); i+=8)
    {
        string substring=text.substr(i,8);
        vector<bool> tem=DES_encipher(convert(substring), key);
        re.insert(re.end(), tem.begin(),tem.end());
    }
    
    return re;
}


//解密文本
string decipher_text(vector<bool> cipher, string key)
{
    string re;
    int i;
    //cout<<cipher.size()<<endl;
    for(i=0; i<cipher.size(); i+=64)
    {
        //string substring=cipher.substr(i,8);
        //cout<<substring.size()<<endl;
        vector<bool> sub;
        for(int j=i; j<i+64; j++)
            sub.push_back(cipher[j]);
        re+=convert_2(DES_decipher(sub, key));
    }
    return re;
}

int main()
{
    
    //定义明文
    string text1="hello, I'm writing DES code.";
    //定义密文
    string key="cipher1";
    
    //输出明文和秘钥
    cout<<"明文: "<<endl;
    cout<<text1<<endl<<endl;
    cout<<"秘钥: "<<endl;
    cout<<key<<endl<<endl;
    
    //加密
    vector<bool> en= encipher_text(text1, key);    //en中存放加密结果: vector<bool>
    //输出加密结果
    cout<<"DES加密: "<<endl;
    for(int i=0; i<en.size();i++)
        cout<<(int)en[i];
    cout<<endl;
    
    //解密
    string de= decipher_text(en, key);         //de中存放解密结果: string
    //输出解密结果
    cout<<"DES解密："<<endl;
    cout<<de<<endl<<endl;
    
    
    return 0;
}
