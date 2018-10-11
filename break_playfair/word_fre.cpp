//
//  main.cpp
//  cipher1
//
//  Created by mov on 23/03/2018.
//  Copyright © 2018 wjp. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string plain_location="明文的相对位置/alice.txt";
int main(int argc, const char * argv[]) {
    
    double frequency[26];
    int word[26];

    for(int i=0; i<26; i++)
        word[i]=0;
    
    ifstream fin(plain_location);

    int sum=0;
    string s;
    while(getline(fin,s))
    {
        //cout << "Read from file: " << s << endl;
        for(int i=0; i<s.length(); i++)
        {
            char ch=s[i];
            if(ch<='Z' && ch>='A')
            {
            word[ch-'A']+=1;
             sum++;
            }
            else if(ch<='z' && ch>='a')
            {
            word[ch-'a']+=1;
             sum++;
            }
        }
    }
    //根据每个单词出现的次数统计词频；
    for(int i=0; i<26; i++)
        frequency[i]=word[i]/(1.0*sum);

    printf("a到z的频率分别为：\n");
    for(int i=0; i<26; i++)
        printf("%c:  %f\n", 'a'+i, frequency[i]);
    
    return 0;
}
