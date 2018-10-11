//
//  main.cpp
//  SA
//
//  Created by mov on 03/04/2018.
//  Copyright © 2018 wjp. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "key.hpp"

string cipher_location="换成密文位置/Playfair_cipher.txt";

//读取并预处理待解密的密文 //只保留大小写字母
string process_cipher(string cipher_location)
{
    string s_return,s;
    
    ifstream fin(cipher_location);
    
    
    if(fin){
        ostringstream tmp;
        tmp << fin.rdbuf();
        s = tmp.str();
    }else{
        s="HS SB WN XN SP GQ BU YV NU CB MB NH UB OK AI CU TO UT MC VA SC VT NI UN NL MS QU SY CM UC HM BK MA FA FU IO QD SF YO HI QA RL NX CS EI FU IO QD CE OT OY FC FI LU CU SC IM GY UB RG AF CU LH RM UP HS UC KI AY FU OY QB EC WN TL TO YU DA MC YO FI FU SC IA OT AG TX CE OX EB CM SF AF CV BU IA YV NA UT SE AF UT QA EU CB MB NH UB OK CS PO AF UM AF CS OH LG MC ET EP YO YH SF AM US YU FB FU TO CS YS YO ER MI TL BY CU LH TF FU RL PH CM OI WN AH FC UL MF FU OY XT AL YO HL AF CS TO CS YS YO YL LD YO ID MY FU YH UM GE PN OR FT SE US BN AM KC BO GA OH RI CU PN UT MN LU AE PY SM TS XT IP GY PN LM UV TY DI QY FS NI RE SF YL MR YU TF TL ST LC RU OH SC IU IU IB PG YL FY SM CU PN OE LY UY YT PO UQ";
        
    }
    for(int i=0; i<s.size(); i++){
        if( (s[i]<='Z'&&s[i]>='A') || (s[i]<='z'&&s[i]>='a'))
            s_return+=s[i];
    }

    
    return s_return;
}

int main(int argc, const char * argv[]) {
    
    string ciphertext=process_cipher(cipher_location);
    
    key decipher;
    decipher.ciphertext = ciphertext;
    double step=0.2;
    double temp=10;
    
    char initial_key[5][5]={
     'a', 'b', 'c', 'd','e',
     'f', 'g', 'h', 'i','k',
     'l', 'm', 'n', 'o','p',
     'q', 'r', 's', 't','u',
     'v', 'w', 'x', 'y','z'
    };

    for(int i=0; i<5;i++)
        for(int j=0; j<5;j++)
            decipher.key_best[i][j]=initial_key[i][j];
    
    time_t nowtime=  time(NULL); //获取当前时间
    decipher.init();
    for(double T = temp; T>=0.2 ; T-=step){
        for(int count = 0; count<=10000; count++){
            decipher.T=T;
            decipher.change_key();
            decipher.select_key();
        }
        cout<<T<<endl;
    }
    
    
    cout<<"解密的秘钥："<<endl;
    for(int i=0; i<5;i++){
        for(int j=0; j<5;j++)
            cout<<decipher.key_best[i][j]<<" ";
        cout<<endl;
    }
    
    cout<<"解密的明文："<<endl;
    decipher.k_copy_4();
    cout<<decipher.decipher_func(ciphertext)<<endl;
    
    cout<<time(NULL)-nowtime<<endl<<endl;
    return 0;
}
