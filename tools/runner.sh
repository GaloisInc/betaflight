# ========== vars
# place this script anywhere but just update path to BT
# and script will look for tools on same level as script
BT_PATH=../../betaflight

PORT=/dev/ttyUSB0
BAUD=115200
DIR=/opt/kendryte-toolchain
KENDRYTE_TAR=kendryte-toolchain.tar.gz
PATH_KENDRYTE_TC=https://s3.cn-north-1.amazonaws.com.cn/dl.kendryte.com/documents/kendryte-toolchain-ubuntu-amd64-8.2.0-20190213.tar.gz
REQUIRED_PKG="minicom"
PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG|grep "install ok installed")

KFLASH_REPO=https://github.com/kendryte/kflash.py.git
KFLASH_DIR=kflash

UNITY_ROOT=unit_testing_k210
UNITY_DIR=$UNITY_ROOT/Unity
UNITY_REPO=https://github.com/ThrowTheSwitch/Unity.git
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
  echo "\nINFO: $DIR not found - pulling kendryte toolchain from\n\n$PATH_KENDRYTE_TC\n\n"
  curl $PATH_KENDRYTE_TC > $KENDRYTE_TAR
  tar -xvf $KENDRYTE_TAR -C /opt
  # clean up
  rm $KENDRYTE_TAR
fi

# if $KFLASH_DIR doesn't exist then clone repo
if [ ! -d "$KFLASH_DIR" ]; then
  echo "\nINFO: $KFLASH_DIR not found - pulling kflash from\n\n$KFLASH_REPO\n\n"
  mkdir -p $KFLASH_DIR && git clone $KFLASH_REPO $KFLASH_DIR
fi

# if $UNITY_DIR doesn't exist then clone repo
if [ ! -d "$UNITY_DIR" ]; then
  echo "\n\nINFO: $UNITY_DIR not found - pulling Unity source code from\n\n$UNITY_REPO\n\n"
  #mkdir if it doesn't exist
  mkdir -p $UNITY_ROOT && git clone $UNITY_REPO $UNITY_DIR
fi

# kill any minicom sessions to allow comm to port
pkill -f minicom

# clean up previous make
rm -rf ../obj

#cd $BT_PATH && make
make -C $BT_PATH

# wait to allow file to become avaliable
sleep 1

BIN_FILENAME=$(ls ../obj | grep .bin)
BIN_FILE=../obj/$BIN_FILENAME

# make sure make completes successful before continuing
if [ -f "$BIN_FILE" ]; then
  echo "$BIN_FILE exist"
else 
  echo "\n\n$BIN_FILE does not exist\n\n"
  exit 1
fi

# if input is 'no' then no flashing
if [ "$1" != "no" ]; then
  cp $BIN_FILE $KFLASH_DIR
  python3 $KFLASH_DIR/kflash.py -p $PORT $KFLASH_DIR/$BIN_FILENAME
  # allow bin to runs once then we can restart again
  sleep 1
  gnome-terminal -- sh -c 'sudo minicom -b $BAUD -D $PORT'
fi
