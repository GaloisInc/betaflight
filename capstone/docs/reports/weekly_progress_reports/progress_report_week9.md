Weekly Progress Report
======================

Week 9
===========================

### Date:03/02 - 03/09

Authors:
--------

**Bliss Brass - brass@pdx.edu**

**Eric Schulte - eschulte@pdx.edu**

**Nikolay Nikolov - nnikolov@pdx.edu**

**Ruben Maldonado - mruben@pdx.edu**

# Summary:

## Bliss Summary :

### Doing:

- Investigating short in the build
    - Confirmed FC and Receiver are fine.
    - Issue is on the PDB or ESC’s somewhere.

### TODO (Week 10):

- Finish drone build and have it ready to fly
    - Attach propellers
- Fly drone directly with transmitter without computer running Cleanflight configurator in the middle.
- Perform test flight by having drone hover and land.
- Fly drone in a couple small laps.

### Done (Week 9):

- Got drone all fully hooked up and operational
- Resoldered power lines to fit into the frame better.
- Ran diagnostic tests in Cleanflight configurator
        - Tested motors, LED’s and receiver/transmitter connection
        - Practiced re-flashing Cleanflight to the F3 (it was already on it).
- Received Amazon shipment and took the battery home and got it safely charged.
- Drone build
- Shorten ESC wires and heat wrap
- Secure ESC’s via zipties
- Resolder the 1 motor that’s spinning backwards
- Perform final securements with zipties/tape
- Setup the transmitter to communicate properly (documentation was not provided). 

## Eric Summary :

### Doing :

- Creating target.c, target.h, target.mk for HiFive board (located in: src/main/target/sifive-hifive1-revb). (Part of main.c port).

### TODO (Week 10):
- Create target.c, target.h, target.mk for HiFive board (located in: src/main/target/sifive-hifive1-revb). (Part of main.c port).

### Done (Week 9):

- Retargeted Minimum Port files to Betaflight
- Updated Minimum Port documentation for Betaflight

## Nikolay Summary :

### Doing:
- Working on TARGET (target.h, target.c etc)
- Progress report

### TODO (Week 10):
- Modify platform.h (part of main.c port)

###  Done (Week 9):


## Ruben Summary :

### Done (Week 9):
- Reviewed UART protocol for RX
- Creates sub fork of my min changes
- Send proposal to Michal and Dan

### TODO (Week 10):
- Continue with port - will focus on HIFIVE1_REVB.mk file and the betaflight.mk


## Challenges:

Had an issue during the mounting phase of the drone build. Even though the whole system was tested hooked and powered up fully and was working. While the double-sided tape and zip ties were being applied to mount the battery, receiver and FC somehow a short occurred at power-up.

## Notes:

Last week (week 9), we realized there is an extra step needed before porting the receiver module. We must first get Betaflight’s main.c (and it’s dependencies) built for and running on the HiFive board, before work can begin on porting the receiver module. (In order to test a ported receiver module, something first needs to be in place to call said receiver module - i.e. main.c).

Once main.c is running, we can incrementally add ported modules (the receiver, IMU, and the ESCs).

So, work has temporarily shifted from porting the receiver module to porting Betaflight’s main.c. We have divided this work into roughly three parts; Niko, Ruben, and Eric’s tasks this week (week 10) are each a constituent part of porting main.c and its dependencies.




