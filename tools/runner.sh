# ========== vars
BIN_FILENAME=$(ls betaflight/obj | grep .bin) # requires BF to have been made at least once before
BIN_FILE=obj/$BIN_FILENAME
KFLASH_PATH=../kflash
PORT=/dev/ttyUSB0
DIR=/opt/kendryte-toolchain
KENDRYTE_TAR=kendryte-toolchain.tar.gz
PATH_KENDRYTE_TC=https://s3.cn-north-1.amazonaws.com.cn/dl.kendryte.com/documents/kendryte-toolchain-ubuntu-amd64-8.2.0-20190213.tar.gz
REQUIRED_PKG="minicom"
PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG|grep "install ok installed")
# ==========

# make sure you're running as sudo since kflash and minicom requires
if [[ $EUID -ne 0 ]]; then
   echo "\n\nThis script must be run as root\n\n" 
   exit 1
fi

# install minicom if not installed
echo "Checking for $REQUIRED_PKG: $PKG_OK"
if [ "" = "$PKG_OK" ]; then
  echo "No $REQUIRED_PKG. Setting up $REQUIRED_PKG."
  sudo apt-get --yes install $REQUIRED_PKG 
fi

# get kendryte toolchain
# if $DIR doesn't exist then pull toolchain
if [ ! -d "$DIR" ]; then
  echo "\nINFO: ${DIR} not found - pulling kendryte toolchain from\n\n$PATH_KENDRYTE_TC\n\n"
  curl $PATH_KENDRYTE_TC > $KENDRYTE_TAR
  tar -xvf $KENDRYTE_TAR -C /opt
  # clean up
  rm $KENDRYTE_TAR
fi

# kill any minicom sessions to allow comm to port
pkill -f minicom
# clean up previous make
rm -rf betaflight/obj

cd betaflight && make
# make sure make completes successful before continuing
if [ -f "$BIN_FILE" ]; then
	echo "$BIN_FILE exist"
else 
	echo "\n\n$BIN_FILE does not exist\n\n"
	exit 1
fi

# if input is 'no' then no flashing
if [ "$1" != "no" ]; then
	cp $BIN_FILE $KFLASH_PATH
	cd $KFLASH_PATH && python3 kflash.py -p $PORT $BIN_FILENAME
	# allow bin to runs once then we can restart again
	sleep 1
	gnome-terminal -- sh -c 'sudo minicom'
fi
