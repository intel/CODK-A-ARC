IDEVER := 1.6.9
ARDUINOSW_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
arduino-headless := $(ARDUINOSW_DIR)/arduino-headless

help:
	@echo "Install dependencies: sudo make install-dep"
	@echo "Set up the build env: make setup"
	@echo "Compile only: make compile sketch=<file>.ino"
	@echo "Compile and upload: make upload sketch=<file>.ino"

check-root:
	@if [ `whoami` != root ]; then echo "Please run as sudoer/root" ; exit 1 ; fi

install-dep: check-root
	apt-get update
	apt-get install xvfb xz-utils

setup:
	@echo "Downloading Arduino IDE"
	curl -OL http://downloads.arduino.cc/arduino-$(IDEVER)-linux64.tar.xz
	@echo "Unpacking Arduino IDE"
	tar xf arduino-$(IDEVER)-linux64.tar.xz
	ln -s arduino-$(IDEVER) arduino
	./install-arduino101.sh

compile:
	@echo $(ARDUINOSW_DIR)
	@echo $(arduino-headless)
	$(arduino-headless) --verify $(sketch)

upload:
	$(arduino-headless) --upload $(sketch)

clean:
	rm -rf arduino-$(IDEVER)
	rm arduino-$(IDEVER)-linux64.tar.xz
	rm arduino

.PHONY: help check-root install-dep setup compile upload clean
