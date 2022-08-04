DISCONTINUATION OF PROJECT.

This project will no longer be maintained by Intel.

Intel has ceased development and contributions including, but not limited to, maintenance, bug fixes, new releases, or updates, to this project. 

Intel no longer accepts patches to this project.

If you have an ongoing need to use this project, are interested in independently developing it, or would like to maintain patches for the open source software community, please create your own fork of this project. 
CODK-A applications are developed in C/C++. You can import a sketch (.ino)
file to use with CODK-A, but you must first convert it to a valid .cpp
file:

`$ make convert-sketch SKETCH=sketch.ino`

Using the included Blink.ino example sketch, the precedure for converting,
building and uploading an existing sketch follows (ensure that your
Arduino 101 board is connected to your PC, and that you know the serial
device name e.g. `/dev/ttyACM0`):

Make sure `CODK_DIR` variable is set to the CODK top-level path
```
  $ cd arc/examples/Blink
  $ make convert-sketch SKETCH=Blink.ino
  $ make compile
  $ make upload SERIAL_PORT=/dev/ttyACM0
```
