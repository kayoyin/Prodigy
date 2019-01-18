#!/bin/bash

# install boost
sudo apt-get install -y libboost-all-dev

sudo apt-get install -y txt2man

# install cython
sudo apt-get install -y python-pip
sudo pip install cython

pushd .
wget http://sourceforge.net/projects/arma/files/armadillo-9.200.4.tar.xz
tar xJf armadillo-9.200.4.tar.xz
cd armadillo-9.200.4
mkdir build
cd build
cmake ../
make
sudo make install
popd
rm -rf armadillo-9.200.4
rm armadillo-9.200.4.tar.xz

pushd .
wget https://github.com/mlpack/mlpack/archive/mlpack-3.0.4.tar.gz
tar xzf mlpack-3.0.4.tar.gz
cd mlpack-mlpack-3.0.4
mkdir build
cd build
cmake ../
make
sudo make install
popd
rm mlpack-3.0.4.tar.gz
rm -rf mlpack-mlpack-3.0.4

pushd .
mkdir build
cd build
cmake ../
make
wget https://drive.google.com/file/d/1facBLd9K4M_szjrW6URXR8SIwUQE4yfr/view?usp=sharing -O model.xml

