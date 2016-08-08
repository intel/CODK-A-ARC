Curie Open Development Kit (CODK) Requirements
--------------
- 64-bit Linux distribution

(Tested on Debian 8, Debian 7, Ubuntu 14.04)
(NOTE: add more distros if anyone's tested on something else)

CODK-A first-time setup
-----------------------

  # Install the Repo tool;

  Make sure you have a '~/bin' directory and that it is included in your path:

    $ mkdir ~/bin
    $ export PATH=~/bin:$PATH

  # Download the Repo tool and ensure that it is executable:

    $ curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
    $ chmod a+x ~/bin/repo

  # Install CODK-A-Manifest

    $ mkdir ~/CODK-A
    $ cd ~/CODK-A
    $ repo init -u https://github.com/01org/CODK-A-Manifest.git
    $ repo sync -j4

  # Set CODK_DIR

    $ export CODK_DIR=~/CODK-A

    (put this in your ~/.bashrc or whatever)
    
  # Set up CODK-A-Software

    $ sudo make install-dep -C $CODK_DIR/arduino101_software
    $ make setup -C $CODK_DIR/arduino101_software
    
  # Set up CODK-A Firmware

    $ cd $CODK_DIR
    $ sudo make -C $CODK_DIR/arduino101_firmware/projects/arduino101 one_time_setup

CODK-A Software
---------------

  # Using CODK-A-Software

  CODK-A applications are developed in C/C++. You can import a sketch (.ino)
  file to use with CODK-A, but you must first convert it to a valid .cpp
  file:

    $ make convert-sketch SKETCH=sketch.ino

  Using the included Blink.ino example sketch, the precedure for converting,
  building and uploading an existing sketch follows (ensure that your
  Arduino 101 board is connected to your PC, and that you know the serial
  device name e.g. "/dev/ttyACM3"):

    $ cd $CODK_DIR/arduino101_software/examples/Blink
    $ make convert-sketch SKETCH=Blink.ino
    $ make compile
    $ make upload SERIAL_PORT=/dev/ttyACM3


CODK-A Firmware 
---------------

# Build CODK-A Firmware

    $ make -C $CODK_DIR/arduino101_firmware/projects/arduino101 setup image

# Flash CODK-A Firmware

    $ cd arduino101_flashpack
    $ ./create_flasher.sh
    $ ./flash_dfu.sh
