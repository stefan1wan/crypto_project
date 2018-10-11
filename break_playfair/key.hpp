//
//  key.hpp
//  SA
//
//  Created by mov on 03/04/2018.
//  Copyright © 2018 wjp. All rights reserved.
//

#ifndef key_hpp
#define key_hpp

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <vector>


using namespace std;
struct node{
    int x;
    int y;
};

class key
{
private:
    char key[5][5];
    char key_tem[5][5];
    
    double fit_value_tem;
    double fit_value;
    string plaintext;
    
    void k_copy_1();
    void k_copy_2();
    void k_copy_3();
    void swap2rows();
    void swap2cols();
    void swap2letters();
    void swap_all();
    void swap_all_cols();
    void swap_all_rows();
    
    struct node posfind(char a);
    string getplain(struct node a, struct node b);
   
    double fitness_func(string s);
   
    
public:
    double T;
    string ciphertext;
    double fit_key_best;
    char key_best[5][5];
    string decipher_func(string ciphertext);

    void init();
    void change_key();
    void select_key();
    
    void k_copy_4();
};

#endif /* key_hpp */
