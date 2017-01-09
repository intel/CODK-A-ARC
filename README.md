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

If your sketch has dependencies, you must include it's path
in the makefile using the `LIBDIR =` variable.

Arduino Built-in Libraries are located at:

`$(ARDUINOIDE_DIR)/libraries/`
For example you can include the WIFi library as follows:

`LIBDIR = $(ARDUINOIDE_DIR)/libraries/WiFi/src `

All Curie versions of the Arduino Built-in Libraries, and Curie specific 
Libraries are located at:

`$(ARDUINOSW_DIR)/corelibs/libraries/ `

The following Libraries are available:

+ CurieBLE
+ CurieEEPROM
+ CurieIMU
+ CurieMailbox
+ CurieSMC
+ CurieSoftwareSerial
+ CurieTime
+ SerialFLash
+ Servo
+ SPI
+ Wire

For example you can include the WIFi library as follows:

`LIBDIR = $(ARDUINOSW_DIR)/corelibs/libraries/Wire/src `
	
The folder for User and External Libraries is referenced as follows:

`$(ARDUINOSW_DIR)/libraries/ `

For example you can include your library library as follows:

`LIBDIR = $(ARDUINOSW_DIR)/libraries/<Your_Library>/src `