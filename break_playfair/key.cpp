//
//  key.cpp
//  SA
//
//  Created by mov on 03/04/2018.
//  Copyright © 2018 wjp. All rights reserved.
//

#include "key.hpp"
#include <math.h>
#include "qgr.h"

extern double qgram[];

//key->key_tem change
void key::k_copy_1()
{
    
    for(int i=0; i<5; i++)
        for(int j=0; j< 5; j++)
            key_tem[i][j]=key[i][j];
}

//key_tem-->key
void key::k_copy_2()
{
    
    for(int i=0; i<5; i++)
        for(int j=0; j< 5; j++)
            key[i][j]=key_tem[i][j];
    
}

void key::k_copy_3()
{
    
    for(int i=0; i<5; i++)
        for(int j=0; j< 5; j++)
            key_best[i][j]=key_tem[i][j];
    
}

void key::k_copy_4()
{
    
    for(int i=0; i<5; i++)
        for(int j=0; j< 5; j++)
            key[i][j]=key_best[i][j];
    
}


struct node key::posfind(char a)
{
    struct node tem;
    
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            if(key[i][j]==a)
            {
                tem.x=i;
                tem.y=j;
                return tem;
            }
    tem.x=5;
    tem.y=5;
    return tem;
}

string key::getplain(struct node a, struct node b)
{
    
    string re="";
    
    if(a.x==b.x)
    {
        re+=key[a.x][(a.y+4)%5];
        re+=key[a.x][(b.y+4)%5];
        
    }
    else if(a.y==b.y)
    {
        re+=key[(a.x+4)%5][a.y];
        re+=key[(b.x+4)%5][a.y];
    }
    else
    {
        re+=key[a.x][b.y];
        re+=key[b.x][a.y];
    }
    
    return re;
}

string key::decipher_func(string ciphertext)
{
    string plaintext="";
    for(int i=0; i< ciphertext.length(); i+=2)
    {
        char ch1= ciphertext[i];
        char ch2= ciphertext[i+1];
        
        int dis='a'-'A';
        if(ch1<='Z'&& ch1>='A')
            ch1=ch1 + dis;
        
        if(ch2<='Z'&& ch2>='A')
            ch2=ch2 + dis;
        
        if(ch1=='j')
            ch1='i';
        if(ch2=='j')
            ch2='i';
        
        struct node first_pos=  posfind(ch1);
        struct node second_pos= posfind(ch2);
        plaintext+=getplain(first_pos, second_pos);
    }
    return plaintext;
}

//改变key的值；
void key::change_key()
{
    int i=rand()%50;
    switch(i)
    {
    case 0: swap2cols();break;
    case 1: swap2rows();break;
    case 2: swap_all();break;
    case 3: swap_all_cols();break;
    case 4: swap_all_rows();break;
    default:
            swap2letters();
    }
}

void key::select_key()
{
    //用秘钥生成明文
    plaintext=decipher_func(ciphertext);
    //评估新生成明文的健壮性
    fit_value=fitness_func(plaintext);

    double fit=fit_value-fit_value_tem;
    //change
    if(fit>=0)
    {
        fit_value_tem=fit_value;
        k_copy_1();//key->key_tem
        //cout<<fit_value<<endl;
    }
    else
    {
        //p为变化的概率： fit越大，变化的概率越大，fit小于0，T越大，变化的概率越小；
        double p=exp(fit/T);
        double ran=(1.0)*rand()/RAND_MAX;
        
        if(ran<p){
            fit_value_tem=fit_value;
            k_copy_1();
        }
        else{
            k_copy_2();
            fit_value=fit_value_tem;
        }
    }

    if(fit_value_tem>fit_key_best)
    {
        fit_key_best=fit_value_tem;
        k_copy_3();
    }
}

//评估解密出的明文的健壮性
double key::fitness_func(string s)
{
    //利用1统计出的明文的每个字母的频率
    //统计密文频率
    //返回欧几里德距离
    
//    //统计新生成的明文的词频
//    ana_fre ana_fre;
//    vector<double> new_get=ana_fre.get_fre_playfair_by_string(plaintext);
//    double distance=0.0;
//    for(int i=0; i<25 ;i++)
//        distance+=(new_get[i]-frequency[i])*(new_get[i]-frequency[i]);
//
//    return -1000000.0*distance;
    //另一种计算方法
    
    double score = 0;
    char temp[4];
    for (int i=0;i<s.size()-3;i++){
        temp[0]=s[i]-'a';
        temp[1]=s[i+1]-'a';
        temp[2]=s[i+2]-'a';
        temp[3]=s[i+3]-'a';
        // we have to index into the correct part of the array
        score += qgram[17576*temp[0] + 676*temp[1] + 26*temp[2] + temp[3]];
    }

    return score;
}


void key::init()
{
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            key_tem[i][j]=key_best[i][j];
    
    k_copy_2();
    //随机数
    fit_value=0;
    srand((unsigned)time(NULL));
    //计算得到明文
    plaintext=decipher_func(ciphertext);
    //统计初始健壮性
    fit_value_tem=fitness_func(plaintext);
    fit_key_best=fit_value_tem;
    
}


void key::swap2rows(){
    int i = rand()%5;
    int j = rand()%5;
    
    for(int k=0;k<5;k++){
        key[i][k]=key_tem[j][k];
        key[j][k]=key_tem[i][k];
    }
}

void key::swap2cols(){
    int i = rand()%5;
    int j = rand()%5;
    
    for(int k=0; k<5; k++){
        key[k][i]=key_tem[k][j];
        key[k][j]=key_tem[k][i];
    }
}

void key::swap2letters(){
    int x1=rand()%5;
    int x2=rand()%5;
    int y1=rand()%5;
    int y2=rand()%5;
    
    key[x1][y1]=key_tem[x2][y2];
    key[x2][y2]=key_tem[x1][y1];
    
}

void key::swap_all(){
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++){
            int id=5*i+j;
            id= 24-id;
            int x=id/5, y= id%5;
            key[i][j]=key_tem[x][y];
        }
}

void key::swap_all_rows(){
    for(int k=0; k<5; k++){
        key[0][k]=key_tem[4][k];
        key[1][k]=key_tem[3][k];
        key[3][k]=key_tem[1][k];
        key[4][k]=key_tem[0][k];
    }
    
}

void key::swap_all_cols(){
    for(int k=0; k<5; k++){
        key[k][0]=key_tem[k][4];
        key[k][1]=key_tem[k][3];
        key[k][3]=key_tem[k][1];
        key[k][4]=key_tem[k][0];
    }
}


