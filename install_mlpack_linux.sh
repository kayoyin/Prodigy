sudo -apt-get install libarmadillo-dev
wget http://www.mlpack.org/files/mlpack-3.0.3.tar.gz
tar -xvzpf mlpack-3.0.3.tar.gz
mkdir mlpack-3.0.3/build && cd mlpack-3.0.3/build
cmake ../
make -j1
sudo make install
