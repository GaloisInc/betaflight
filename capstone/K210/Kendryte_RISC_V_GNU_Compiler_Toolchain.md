# Kendryte RISC-V GNU Compiler Toolchain (Ubuntu)

## Install the packages


`sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev`


## Get the gnu-toolchain

`git clone --recursive https://github.com/kendryte/kendryte-gnu-toolchain`

## Installation (Newlib)

### To build the Newlib cross-compiler new to add the executable to $PATH

1.  `$ echo $PATH`
2.  `cd ~/`
3.  `vim .bashrc` or other text editor than vim
4.  Add the end of the file append
    `PATH=$PATH:/opt/kendryte-toolchain/bin`
5.  Source your .bashrc,type `source ~/.bashrc` and check with `$ echo $PATH` If everything
    went well you should see in the output
    `.../x86_64-linux:/opt/kendryte-toolchain/bin`

### Add folder and chanage permissions

1.  `cd /opt`
2.  `sudo mkdir kendryte-toolchain` - if there is not already existing directory
3.  `sudo chmod 777 kendryte-toolchain`
4.  `cd` into the kendryte-gnu-toolchain you downloaded
5.  Execute `./configure --prefix=/opt/kendryte-toolchain --with-cmodel=medany --with-arch=rv64imafc --with-abi=lp64f`
6.  In no errors then type `make -j8`
