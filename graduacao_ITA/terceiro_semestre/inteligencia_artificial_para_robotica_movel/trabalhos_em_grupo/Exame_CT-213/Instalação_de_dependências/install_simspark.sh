#!/usr/bin/env bash
#install server

sudo apt-get install g++ libfreetype6-dev libode-dev libsdl-dev ruby ruby-dev libdevil-dev libboost-dev libboost-thread-dev libboost-regex-dev libboost-system-dev libaudio2 libmng2

tar -xvf Qt4.8.7.tar.xz
sudo dpkg -i *.deb
sudo apt --fix-broken install
rm *.deb

pushd ~
wget https://gitlab.com/robocup-sim/SimSpark/uploads/ec509136d31e4c3c9dbba5057e1f0856/simspark-0.3.1.tar.xz
tar -xvf simspark-0.3.1.tar.xz
wget https://gitlab.com/robocup-sim/SimSpark/uploads/a3f49de105e2f50a1da89f1dd75833a0/rcssserver3d-0.7.2.tar.xz
tar -xvf rcssserver3d-0.7.2.tar.xz

pushd simspark-0.3.1
mkdir -p build
cd build
cmake ..
make -j$(nproc)
sudo make install
sudo ldconfig
popd

pushd rcssserver3d-0.7.2
mkdir -p build
cd build
cmake ..
make -j$(nproc)
sudo make install
sudo ldconfig
sudo echo "/usr/local/lib" >> sudo /etc/ld.so.conf
sudo ldconfig
echo 'export LD_LIBRARY_PATH=/usr/local/lib/simspark' >> ~/.bashrc
export LD_LIBRARY_PATH=/usr/local/lib/simspark
popd

popd
