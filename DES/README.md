
# 实验2 DES对称加密算法 


### 一、实验目的

1)  了解 DES 的加解密原理。  
2)  能够使用 DES 算法对数据进行加密和解密处理。  

### 二、实验内容

     用 C\C++语言实现 DES 加密、解密算法。 

### 三、实验步骤
  * 本实验思路较为简单，对照实验报告指导书DES的设计实现即可。在代码实现时，每个函数的功能均添加了备注。
  * 其中，代码实现过程中：
    - 首先定义各种置换表，放在“table.h”中。
    - 用vector<bool> 存储二进制数据。

  * 代码实现时，忽视了一个重要的问题：C++数组的下标从0开始，但是置换表从1开始。这导致加密结果总出错，且每次加密 结果不同。这是由于数组访问越界（Xcode竟然不报错），这个bug找了5个小时才发现 。
