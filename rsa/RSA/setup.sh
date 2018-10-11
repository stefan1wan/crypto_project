#!/bin/sh
cd /usr/local

sudo wget https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2
sudo tar -jvxf gmp-6.1.2.tar.bz2

cd gmp-6.1.2
sudo ./configure --enable-cxx
sudo make
sudo make check
sudo make install

# 参考网址：https://zybuluo.com/veightz/note/12742
