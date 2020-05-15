Weekly Progress Report
======================

Week 8
===========================

### Date:02/24 - 03/02

Authors:
--------

**Bliss Brass - brass@pdx.edu**

**Eric Schulte - eschulte@pdx.edu**

**Nikolay Nikolov - nnikolov@pdx.edu**

**Ruben Maldonado - mruben@pdx.edu**

Summary:
---------

## Bliss Summary :

### Doing:

- Drone build
	- Cutting power lines down to fit better.

### TODO:

- Finish drone build and have it ready to fly
 - Solder motors to the frame
 - Flash Clean/Betaflight to FC
 - Complete frame assembly
 - Secure ESC’s via zipties

### Done:

- Drone 90% built
    - Assembled drone kit,
    - Soldered parts
    - Figured out connections and configurations of power lines and Transmitter/Receiver
    - Compiled list of additional missing parts to ask Galois to buy

## Eric Summary :

### Doing :

- Beginning i-Bus, UART driver research.

### TODO :

- Research Flysky’s i-Bus protocol (used by the onboard receiver).
- Investigate Betaflight and SiFive UART drivers (needed for receiver).
- Need to figure out: How do we get the HiFive’s UART talking to Betaflight?
- Create wiki documentation on i-Bus and UART drivers.

### Done :

- Updated project schedule and project proposal based on Dan/Michal’s recommendations during - last week’s meeting (2/27)
- Sent back to Galois for approval.

## Nikolay Summary :

### Doing:

- Flashing Betafight

### TODO :

- Min port for betaflight

###  Done :

- Weekly report .md template
- Meeting with Galois
- Reached out to maintainers
- Reviewed PDS 
- Assisted Bliss with building the drone

## Ruben Summary :

### Done :

- Sent the required emails
- Started min port with the metal API provided by Freedom
- Email Michal for the Friday meeting - he proposed a new schedule 2/27 5 PM. Need to confirm with Roy
- Continue with min port (goal with compiling sample program) - didn’t finish since Eric did complete the minimum port

- Receipts and shopping with Michal - will ask Michal next week in person
- Started researching UART protocol for RX
- Started Betaflight min port

### TODO :

- Continue UART protocol for RX
- Will min port to Betaflight
- Create sub fork of my min changes
- Send proposal to Michal and dan
- Send proposal 

## Challenges:

- Drone build got delayed by missing parts (securements for ESC’s) not included in the kit or on hand.
- The first drone build! (but fun!).
- Transitioned everything to Betaflight

## Notes:

- Michal ordered protoshield, drone battery charger, double-sided tape and eta ¾
- Galois has suggested we switch from porting Cleanflight to porting Betaflight. Two reasons: 
- Betaflight is more active than Cleanflight, meaning more community support.
- A merge request to Betaflight will be more relevant than a merge request to Cleanflight at this point.
- We have agreed to switch to Betaflight.

### General Notes:






