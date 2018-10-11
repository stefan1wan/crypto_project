#include<iostream>
#include<gmpxx.h>
#include<string>

using namespace std;


/* 随机生成大素数的函数 */
void generate_prime(mpz_t& p, mpz_t& q){
    // 设置随机算法为默认
    gmp_randstate_t gen; 
    gmp_randinit_default(gen);
    //gmp_randseed_ui(gen, time(NULL)); 

    //随机生成生成大整数
    mpz_urandomb(p, gen, 128);
    mpz_urandomb(q, gen, 128);

    //取大整数的下一个素数
    mpz_nextprime(p, p);
    mpz_nextprime(q, q);
}

/* 生成需要的各个参数 */
void generate(mpz_t &p, mpz_t &q, mpz_t &n, mpz_t &phi_n, mpz_t &a, mpz_t &b){

    /* 随机生成p，q */
    generate_prime(p,q);

    /* 生成n phi_n*/
    mpz_mul(n, p, q);

    mpz_t p_1, q_1;
    mpz_init(p_1);  
    mpz_init(q_1);
    mpz_sub_ui(p_1, p, 1);
    mpz_sub_ui(q_1, q, 1);
    mpz_mul(phi_n, p_1 ,q_1);
    
    /* 生成公钥a， 私钥b*/
    mpz_init_set_ui(a, 65537);
    mpz_invert(b, a, phi_n); /*b为 a在 phi_n 下的模*/
}


/* 加密解密函数, 根据传入的秘钥不同加解密*/
void De_Encrypt(mpz_t& result, mpz_t txt, mpz_t key, mpz_t n){
    mpz_powm(result, txt, key, n);
}

int main()
{
    /* 定义参数并初始化*/
    mpz_t p, q, n, phi_n, a, b;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(phi_n);
    mpz_init(a);
    mpz_init(b);

    /* 生成参数 */
    generate(p, q, n, phi_n, a, b);

    // 输出参数
    cout<<"生成RSA参数如下: "<<endl;
    gmp_printf("p: %Zd\n", p);
    gmp_printf("q: %Zd\n", q);
    gmp_printf("n: %Zd\n", n);
    gmp_printf("phi_n: %Zd\n", phi_n);
    gmp_printf("a: %Zd\n", a);
    gmp_printf("b: %Zd\n", b);

    cout<<endl<<endl;

    cout<<"秘钥对: "<<endl;
    cout<<"公钥: ";
    gmp_printf("%Zd", a);
    cout<<endl;
    cout<<"私钥: ";
    gmp_printf("%Zd", b);
    cout<<endl;
    cout<<"n: ";
    gmp_printf("%Zd", n);
    cout<<endl;

    
    string txt; //输入文本
    int  choice; //选择秘钥
    char cont; //是否继续
    mpz_t re;   //存储结果
    mpz_init(re);
        

    while(1){
        cout<<endl;
        cout<<"Please input the text to En&Decipher('q' to Quit): "<<endl;
        cin>>txt;

        if(txt == "q")
            break;

        cout<<endl;
        cout<<"Use which key? \n1:Private Key\n2:Public Key"<<endl;
        cin>>choice;
        while(choice!= 1 && choice != 2){
            cout<<"wrong input"<<endl;
            cin>>choice;
        }

        mpz_t Got_text;
        mpz_init_set_str(Got_text, txt.c_str(), 10);

        if(choice==1)
            De_Encrypt(re, Got_text, b ,n);
        else 
            De_Encrypt(re, Got_text, a ,n);

        cout<<endl;
        gmp_printf("That is result: \n%Zd\n", re);
     }


     cout<<endl<<"Have a good day!"<<endl;

    /*  clear  */
    return 0;    
}
