# ========== vars
BIN_FILENAME=betaflight_4.2.0_MAIXBIT_norevision.bin
BIN_FILE=obj/$BIN_FILENAME
KFLASH_PATH=../kflash
PORT=/dev/ttyUSB0
# ==========

# make sure you're running as sudo since kflash and minicom requires
if [[ $EUID -ne 0 ]]; then
   echo "\n\nThis script must be run as root\n\n" 
   exit 1
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
