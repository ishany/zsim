export PINPATH=$(pwd)/deps/pin-2.14-71313-gcc.4.4.7-linux
export DRAMSIMPATH=$(pwd)/deps/DRAMSim2-2.2.1

export USERLIB=$(pwd)/deps/libconfig-1.7.2/lib:$LD_LIBRARY_PATH:$(pwd)/deps/hdf5-1.10.4/lib:$DRAMSIMPATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$USERLIB

export USERINC=$(pwd)/deps/libconfig-1.7.2/include:$LD_LIBRARY_PATH:$(pwd)/deps/hdf5-1.10.4/include
