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
