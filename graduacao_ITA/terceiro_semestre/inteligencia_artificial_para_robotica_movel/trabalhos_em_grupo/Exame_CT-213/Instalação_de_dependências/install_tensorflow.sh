#!/bin/bash
# Get more info on <https://github.com/cjweeks/tensorflow-cmake> and <https://www.tensorflow.org/install/install_linux>

#You have to install protobuf 3.5.0 to compile tensorflow C++.

function systemPause {
	echo -e '\nVerifique se todos os passos tiveram sucesso e aperte Enter'
	read -n 1 -r tecla
	while [[ ! $tecla = "" ]] 
	do
	read -n 1 -r tecla
	done
}

pushd $HOME

#Installing bazel dependencies
echo -e '\nInstalação do Bazel'
sleep 2
sudo apt-get install pkg-config zip g++ zlib1g-dev unzip python
wget https://github.com/bazelbuild/bazel/releases/download/0.16.1/bazel-0.16.1-installer-linux-x86_64.sh
chmod +x bazel-0.16.1-installer-linux-x86_64.sh
./bazel-0.16.1-installer-linux-x86_64.sh --user
echo 'export PATH="$PATH:$HOME/bin"' >> ~/.bashrc
export PATH="$PATH:$HOME/bin"

#Downloading and compiling tensorflow
systemPause
echo -e '\nInstalação do Tensorflow (É uma instalação demorada, coloque seu computador para carregar)'
sleep 4
git clone https://github.com/FloopCZ/tensorflow_cc.git
cd tensorflow_cc
git checkout v1.8.0

sudo apt-get install build-essential curl git cmake unzip autoconf autogen automake libtool mlocate \
                     zlib1g-dev g++-7 python python3-numpy python3-dev python3-pip python3-wheel wget
sudo updatedb
cd tensorflow_cc
mkdir build && cd build 
cmake -DTENSORFLOW_STATIC=OFF -DTENSORFLOW_SHARED=ON ..
make -j$(nproc) && sudo make install
systemPause

#Installing tensorflow
cd tensorflow
sudo cp bazel-bin/tensorflow/libtensorflow_cc.so /usr/local/lib

sudo mkdir -p /usr/local/include/google/tensorflow
sudo cp -r tensorflow /usr/local/include/google/tensorflow/
sudo find /usr/local/include/google/tensorflow/tensorflow -type f  ! -name "*.h" -delete

sudo cp bazel-genfiles/tensorflow/core/framework/*.h  /usr/local/include/google/tensorflow/tensorflow/core/framework
sudo cp bazel-genfiles/tensorflow/core/kernels/*.h  /usr/local/include/google/tensorflow/tensorflow/core/kernels
sudo cp bazel-genfiles/tensorflow/core/lib/core/*.h  /usr/local/include/google/tensorflow/tensorflow/core/lib/core
sudo cp bazel-genfiles/tensorflow/core/protobuf/*.h  /usr/local/include/google/tensorflow/tensorflow/core/protobuf
sudo cp bazel-genfiles/tensorflow/core/util/*.h  /usr/local/include/google/tensorflow/tensorflow/core/util
sudo cp bazel-genfiles/tensorflow/cc/ops/*.h  /usr/local/include/google/tensorflow/tensorflow/cc/ops

sudo cp -r third_party /usr/local/include/google/tensorflow/
sudo rm -r /usr/local/include/google/tensorflow/third_party/py

popd  
