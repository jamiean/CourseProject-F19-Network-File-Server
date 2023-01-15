make clean
make
./createfs
export FS_CRYPT=CLEAR
./fs 8888 < passwords > output.txt