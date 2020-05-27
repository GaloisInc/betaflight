# ========== vars
BIN_FILENAME=$(ls obj | grep .bin)
BIN_FILE=obj/$BIN_FILENAME
PORT=/dev/ttyUSB0


UNITY_DIR=Unity
UNITY_REPO=https://github.com/ThrowTheSwitch/Unity
# ==========

# make sure you're running as sudo since kflash and minicom requires
if [[ $EUID -ne 0 ]]; then
   echo "\n\nThis script must be run as root\n\n" 
   exit 1
fi

# clone Unity Repo
# if $UNITY_DIR doesn't exist then go out and clone Unity repo
if [ ! -d "$UNITY_DIR" ]; then
    echo "\n\nINFO: ${UNITY_DIR} not found - pulling Unity source code from\n\n$UNITY_REPO\n\n"
	git clone https://github.com/ThrowTheSwitch/Unity.git
fi

# kill any minicom sessions to allow comm to port
pkill -f minicom

# clean up previous make
rm -rf obj

make
# make sure make completes successful before continuing
if [ -f "$BIN_FILE" ]; then
	echo "$BIN_FILE exist"
else 
	echo "\n\n$BIN_FILE does not exist\n\n"
	exit 1
fi

# if input is 'no' then no flashing
if [ "$1" != "no" ]; then
	#cp $BIN_FILE $KFLASH_PATH
	cd obj && kflash -p $PORT $BIN_FILENAME
	# allow bin to runs once then we can restart again
	sleep 1
	gnome-terminal -- sh -c 'sudo minicom'
fi
