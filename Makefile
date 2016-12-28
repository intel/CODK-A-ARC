ARDUINOSW_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
TOOLCHAIN_URL := https://downloadmirror.intel.com/25470/eng/arc-toolchain-linux64-arcem-1.0.1.tar.bz2
TOOLCHAIN     := $(notdir $(TOOLCHAIN_URL))
CORELIBS_URL  ?= https://github.com/01org/corelibs-arduino101/archive/master.zip
CORELIBS_ZIP  := $(notdir $(CORELIBS_URL))
ARDUINO_URL   := https://github.com/arduino/Arduino/archive/1.6.9.zip
ARDUINO_ZIP   := $(notdir $(ARDUINO_URL))

help:

check-root:
	@if [ `whoami` != root ]; then echo "Please run as sudoer/root" ; exit 1 ; fi

install-dep: check-root
	apt-get update
	apt-get install -y python
	cp -f $(ARDUINOSW_DIR)/bin/99-dfu.rules /etc/udev/rules.d/99-dfu.rules
	cp -f $(ARDUINOSW_DIR)/bin/99-ftdi.rules /etc/udev/rules.d/99-ftdi.rules
	cp -f $(ARDUINOSW_DIR)/bin/99-jlink.rules /etc/udev/rules.d/99-jlink.rules
	service udev restart

setup: arc32 arduino-ide corelibs

corelibs:
	@echo "Downloading corelibs"
	cd /tmp; curl -OL $(CORELIBS_URL)
	@echo "Unpacking corelibs"
	unzip /tmp/$(CORELIBS_ZIP) -d $(ARDUINOSW_DIR)
	mv $(ARDUINOSW_DIR)/corelibs-* $(ARDUINOSW_DIR)/corelibs
	rm /tmp/$(CORELIBS_ZIP)


arc32:
	@echo "Downloading ARC Toolchain"
	cd /tmp; curl -OL $(TOOLCHAIN_URL)
	@echo "Unpacking ARC Toolchain"
	tar xf /tmp/$(TOOLCHAIN) -C $(ARDUINOSW_DIR)
	rm /tmp/$(TOOLCHAIN)

arduino-ide:
	@echo "Downloading Arduino IDE"
	cd /tmp; curl -OL $(ARDUINO_URL)
	@echo "Unpacking Arduino IDE"
	unzip /tmp/$(ARDUINO_ZIP) Arduino*/libraries/* -d $(ARDUINOSW_DIR)
	mv Arduino-* arduino-ide
	rm /tmp/$(ARDUINO_ZIP)

