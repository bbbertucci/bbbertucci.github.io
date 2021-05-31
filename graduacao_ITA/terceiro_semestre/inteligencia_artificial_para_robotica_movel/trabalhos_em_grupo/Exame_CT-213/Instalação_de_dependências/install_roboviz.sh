#!/bin/bash

pushd ~
git clone https://github.com/magmaOffenburg/RoboViz.git
popd

pushd ~/RoboViz/scripts
chmod +x build.sh
./build.sh
popd
