//
//  main.cpp
//  decipher
//
//  Created by mov on 03/04/2018.
//  Copyright © 2018 wjp. All rights reserved.
//

#include <iostream>
using namespace std;

struct node{
    int x;
    int y;
};

char key[5][5]={'b', 'i', 't', 'y','a',
                'k', 'd', 'f', 'g','h',
                'p', 'l', 'm', 'n','o',
                'z', 'q', 'v', 'w','x',
                'r', 's', 'e', 'c','u'
};

string getridSpace(string s)
{
    
    string tem;
    for(int i=0; i<s.size(); i++)
        if(s[i]!=' '){
            tem+=s[i];
        }
    return tem;
}

struct node posfind(char a)
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

string getplain(struct node a, struct node b)
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

int main(int argc, const char * argv[]) {
    string ciphertext="HS SB WN XN SP GQ BU YV NU CB MB NH UB OK AI CU TO UT MC VA SC VT NI UN NL MS QU SY CM UC HM BK MA FA FU IO QD SF YO HI QA RL NX CS EI FU IO QD CE OT OY FC FI LU CU SC IM GY UB RG AF CU LH RM UP HS UC KI AY FU OY QB EC WN TL TO YU DA MC YO FI FU SC IA OT AG TX CE OX EB CM SF AF CV BU IA YV NA UT SE AF UT QA EU CB MB NH UB OK CS PO AF UM AF CS OH LG MC ET EP YO YH SF AM US YU FB FU TO CS YS YO ER MI TL BY CU LH TF FU RL PH CM OI WN AH FC UL MF FU OY XT AL YO HL AF CS TO CS YS YO YL LD YO ID MY FU YH UM GE PN OR FT SE US BN AM KC BO GA OH RI CU PN UT MN LU AE PY SM TS XT IP GY PN LM UV TY DI QY FS NI RE SF YL MR YU TF TL ST LC RU OH SC IU IU IB PG YL FY SM CU PN OE LY UY YT PO UQ";
    
    ciphertext=getridSpace(ciphertext);
    
    string plaintext="";
    clock_t cstart,cends;
    cstart=clock();
    for(int i=0; i< ciphertext.length(); i+=2)
    {
        char ch1= ciphertext[i];
        char ch2= ciphertext[i+1];

        int dis='A'-'a';
        if(ch1<='Z'&& ch1>='A')
            ch1=ch1-dis;
        
        if(ch2<='Z'&& ch2>='A')
            ch2=ch2-dis;
        if(ch1=='j')
            ch1='i';
        if(ch2=='j')
            ch2='i';
        
        struct node first_pos=posfind(ch1);
        struct node second_pos=posfind(ch2);
        
        plaintext+=getplain(first_pos, second_pos);
    }

    
    cout<<plaintext<<endl;
    cout<<"解密花费时间为："<<endl;
    cends=clock();
    cout<< cends-cstart <<"ms"<<endl;
    return 0;
}
