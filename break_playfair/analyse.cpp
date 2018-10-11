//
//  main.cpp
//  keyanalyse
//
//  Created by mov on 23/03/2018.
//  Copyright © 2018 wjp. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>
#include <vector>

using namespace std;

typedef pair<char,char>    Point;
typedef pair<Point,int>    Pair1;
typedef pair<Point,double> Pair2;
typedef pair <Pair1,Pair1> PAIR1;
typedef pair <Pair2,Pair2> PAIR2;


string cipher_location="密文位置";
string plain_location="明文位置/alice.txt";
struct CmpByValue1 {
    bool operator()(const PAIR1& lhs, const PAIR1& rhs) {
        return lhs.first.second > rhs.first.second;
    }
};

struct CmpByValue2 {
    bool operator()(const PAIR2& lhs, const PAIR2& rhs) {
       return lhs.first.second > rhs.first.second;
    }
};

int main(int argc, const char * argv[]) {
    
    
    double frequency[25][25];
    int word[25][25];
    
    
    for(int i=0; i<25; i++)
        for(int j=0; j<25 ;j++)
            word[i][j]=0;
    
    ifstream fin(plain_location);
    
    int sum=0;
    string s;
    while(getline(fin,s))
    {
        //cout << "Read from file: " << s << endl;
        //统计每种组合的个数
        for(int i=0; i<s.length(); i+=2)
        {
            char ch1=s[i];
            char ch2;
            if(i==s.length()-1)
                ch2='x';
            else
                ch2=s[i+1];
            
            if(ch2==ch1)
            {
                ch2='x';
                i-=1;
            }
            
            int ind1=0, ind2=0;
            
            if(ch1<='Z' && ch1>='A')
                ind1=ch1-'A'>8? ch1-'A'-1 : ch1-'A';
            else if(ch1<='z' && ch1>='a')
                ind1=ch1-'a'>8? ch1-'a'-1 : ch1-'a';
            
            
            if(ch2<='Z' && ch2>='A')
                ind2=ch2-'A'>8?ch2-'A'-1 : ch2-'A';
            else if(ch2<='z' && ch2>='a')
                ind2=ch2-'a'>8?ch2-'a'-1 : ch2-'a';
            
            word[ind1][ind2]+=1;
            sum++;
        }
    }
    
    fin.close();
    //根据每个单词出现的次数统计词频；
    for(int i=0; i<25; i++)
        for(int j=0; j<25; j++)
            frequency[i][j]=word[i][j]/(1.0*sum);

    //以下代码读取密文
    fin.open(cipher_location);
    
    int word_cipher[25][25];
    for(int i=0; i<25; i++)
        for(int j=0; j<25 ;j++)
            word_cipher[i][j]=0;
    
    string s1;
    int ss=0;
    while(fin>>s1)
    {
        
        char ch1=s1[0], ch2=s1[1];
        //cout<<s1<<endl;
        ss++;
        int ind1=ch1-'A'>8? ch1-'A'-1 : ch1-'A';
        int ind2=ch2-'A'>8? ch2-'A'-1 : ch2-'A';
        
        word_cipher[ind1][ind2]+=1;
    }
    printf("密文对个数为：%d\n",ss);
    
    
    fin.close();
    
    
    map< Pair2, Pair2 > plain_fre;
    map< Pair1, Pair1 > cipher_fre;

    for(int i=0; i<25; i++)
        for(int j=i+1; j<25; j++)
    {
        
        int idi=i>8?'a'+i+1:'a'+i;
        int idj=j>8?'a'+j+1:'a'+j;
        
        if(word_cipher[i][j]>=2 || word_cipher[j][i]>=2 )
        {
        
        if(word_cipher[i][j] >= word_cipher[j][i])
            cipher_fre.insert(PAIR1(Pair1(Point(idi,idj),word_cipher[i][j]),Pair1(Point(idj,idi),word_cipher[j][i])));
        else
            cipher_fre.insert(PAIR1(Pair1(Point(idj,idi),word_cipher[j][i]),Pair1(Point(idi,idj),word_cipher[i][j])));
            
        }
    }
    
    cout<<endl;
    for(int i=0; i<25; i++)
     for(int j=i+1; j<25; j++)
    {
        int idi=i>8?'a'+i+1:'a'+i;
        int idj=j>8?'a'+j+1:'a'+j;
        
        if(word[i][j]>=5000 || word[j][i]>=5000)
        {
            
            if(word[i][j] >= word[j][i])
                plain_fre.insert(PAIR2(Pair2(Point(idi,idj),1.0*word[i][j]/1800),Pair2(Point(idj,idi),1.0*word[j][i]/1800)));
            else
                plain_fre.insert(PAIR2(Pair2(Point(idj,idi),1.0*word[j][i]/1800),Pair2(Point(idi,idj),1.0*word[i][j]/1800)));
        }
    }
    
    
    vector<PAIR1> cipher_f(cipher_fre.begin(), cipher_fre.end());
    vector<PAIR2> plain_f(plain_fre.begin(), plain_fre.end());
    
    sort(cipher_f.begin(), cipher_f.end(), CmpByValue1());
    sort(plain_f.begin(),  plain_f.end(),  CmpByValue2());
    cout<<"明文："<<endl;
    
    for (vector<PAIR2>::iterator iter = plain_f.begin(); iter != plain_f.end(); ++iter) {
        cout <<setiosflags(ios::fixed)<<setprecision(1)<< "("<<iter-> first.first.first<<','<<iter->first.first.second<<")"<<" "<<iter-> first.second<< "  ";
        cout <<setiosflags(ios::fixed)<<setprecision(1)<< "("<<iter-> second.first.first<<','<<iter->second.first.second<<")"<<" "<<iter-> second.second<< endl;
        
    }
    
    cout<<"密文："<<endl;
    for (vector<PAIR1>::iterator iter = cipher_f.begin(); iter != cipher_f.end(); ++iter) {
        cout << "("<<iter-> first.first.first<<','<<iter->first.first.second<<")"<<" "<<iter-> first.second<< "  ";
        cout << "("<<iter-> second.first.first<<','<<iter->second.first.second<<")"<<" "<<iter-> second.second<< endl;
    }
    
    return 0;
}
