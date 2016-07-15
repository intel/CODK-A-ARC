#!/bin/bash -e

port=/dev/ttyACM0
board=Intel:arc32
board2=${board}:arduino_101

./arduino-headless --install-boards $board
./arduino-headless --board $board2 --port $port --save-prefs
