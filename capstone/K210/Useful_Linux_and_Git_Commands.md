# Useful Linux Commands

## Searching

### Find all files with a particular name

```ls -R | grep <name>```

### Find a keyword looking recursively in all files starting from your root directory and print in a new line

```egrep -Rw '<keyword>' | awk '{print $0,"\n"}'```

### Find the tree structure. Debian/Ubuntu-based systems

Install the tree library: ```sudo apt-get install tree -y```

To output the tree structure for a file or folder: ```tree | grep <name>```

```(bash)
         picard@nik:~/Documents/betaflight$ tree | grep dma_
         │   │   │   │       │   ├── stm32f1xx_hal_dma_ex.h
         │   │   │   │       │   ├── stm32f3xx_hal_dma_ex.h
         │   │   │   │   │   │   ├── stm32f4xx_hal_dma_ex.h
         │   │   │   │   │       ├── stm32f4xx_hal_dma_ex.c
         │   │   │   │       │   ├── stm32f7xx_hal_dma_ex.h
         │   │   │   │           ├── stm32f7xx_hal_dma_ex.c
         │   │   │   │       │   ├── stm32g4xx_hal_dma_ex.h
         │   │   │   │           ├── stm32g4xx_hal_dma_ex.c
         │   │   │   │       │   ├── stm32h7xx_hal_dma_ex.h
         │   │   │   │       │   ├── stm32h7xx_hal_dma_ex.c
         │   │   │   ├── dma_reqmap.c
         │   │   │   ├── dma_reqmap.h
         │   │   │   ├── dma_stm32f4xx.c
         │   │   │   ├── dma_stm32f7xx.c
         │   │   │   ├── dma_stm32g4xx.c
         │   │   │   ├── dma_stm32h7xx.c

```

For more information look here:<https://www.tecmint.com/linux-tree-command-examples/>

### Find files using 'find'

```find [start directory] -name <argument>```

E.g.```picard@nik:~/Documents/betaflight$ find . -name "SITL"./src/main/target/SITL```

### How to use sed to find and replace text in files in Linux

```sed 's/<what I am changing>/<what is being changed>/g' <filename>```

  Link: <https://www.cyberciti.biz/faq/how-to-use-sed-to-find-and-replace-text-in-files-in-linux-unix-shell/>

## Git

### Find changes in a file

```git blame --minimal target.h```

E.g ```git blame --minimal target.h | grep <keyword>```

More info here : <https://help.github.com/en/github/managing-files-in-a-repository/tracking-changes-in-a-file>

### Find the commit history

```git log --oneline --decorate --graph | grep <file>```

```picard@nik:~/Documents/betaflight/src/main/target/SITL$ git log --oneline --decorate --graph | grep SITL```

```(bash)
| * | | | | | f042ad910 Add dummy initializeUnusedPins for SITL
* | | | | | |   b5237bba9 Fix SITL target build and runtime segfaults (#9342)
| * | | | | | 548379bab fixed SITL target debug build and runtime segfaults. Added motorPwmDevice vTable functions and IOConfigGPIO stub
| * | | | | | ddf7a29fb Fixed mocking of pwm output on SITL.
| * | | | | | | 93f933f01 Fixed build for gcc 8 (SITL).
| * | | | | | | | | 6b8d40306 Added 'eeprom.bin' (SITL EEPROM image) to '.gitignore'.
| * | | | | | | | | | 4b7ba85ec Fixed SITL.
| * | | | | | | | 9cd5bcd7a Fixed tests / SITL build.
* | | | | | | | | | | | | |   68756ddd6 Merge pull request #4085 from cs8425/bf-SITL-fix-cli-crash
| * | | | | | | | | | | | | f8c9b57fd fix cli crash on get, dump, diff on SITL
* | | | | | | | | |   c5ed9d566 Merge pull request #3633 from cs8425/bf-SITL-EEPROM-in-RAM
| * | | | | 51ca725f0 SITL fix
* | | | | | | | | | | | |   5aa81c81c Merge pull request #3403 from blckmn/SITL
| * | | | | | | | | | | | | 1ff5ff7d9 Remove SITL specific defines from fc_init
* | | | | | | | | | | |   c75697ef3 Merge pull request #3395 from AndersHoglund/fix_SITL_build
| * | | | | | | | | | | | 08281e8ec Fix SITL compile.
| * | | | | | | | 4451ae5fe Exclude SITL case
| * | | | | | | | | 3cfcf8a4b Fix for SITL target
| * \ \ \ \   468217c7e Merge pull request #3115 from ledvinap/fix-SITL
| | * | | | | 919f0caa6 Fix SITL on Cygwin + minor cleanup
| | * | | | | 00bd8d91d Fix SITL
| * | | | | 099bd97f0 Optional -static SITL
| * | | | 58212464c Fix SITL warnings
* | | | | | | | |   53edcba0d Merge pull request #2841 from cs8425/SITL-fix
* | | | | | | | |   42cbbbb68 Merge pull request #2819 from cs8425/SITL
| * | | | | | | | 120fa2169 implement SITL in gazebosim with ArduCopterPlugin
picard@nik:~/Documents/betaflight/src/main/target/SITL$

```

### Find changes in a file with git log

```git log -- ./<filename>```

For more information please check here: <https://www.atlassian.com/git/tutorials/git-log>
_A good one-liner in Linux can give you a lot of useful output without the need to open any files._

Check link : <https://www-users.york.ac.uk/~mijp1/teaching/2nd_year_Comp_Lab/guides/grep_awk_sed.pdf> has great examples.
