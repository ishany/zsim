zsim2: Electric Boogaloo
====

What is this? It's like Zsim, but actually has installation steps and usage for 2018.

`Warning:` This will not work on Linux Kernel 4.10+ (I.e. Ubuntu 18.04 is toast due to an incompatibility in Intel PIN)

# Installation Steps
1. sudo apt install git build-essential gcc-5 g++-5 scons libelf-dev
2. git clone https://github.com/medav/zsim.git
3. tar xvf deps.tar.gz
4. source deps/env.sh
5. scons -jN
6. ./build/opt/zsim tests/simple.cfg

