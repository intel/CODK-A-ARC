ARDUINOSW_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
TOOLCHAIN_URL := https://downloadmirror.intel.com/25470/eng/arc-toolchain-linux64-arcem-1.0.1.tar.bz2
TOOLCHAIN     := $(notdir $(TOOLCHAIN_URL))
CORELIBS_ZIP  := /tmp/corelibs.zip
ARDUINO_URL   := https://github.com/arduino/Arduino/archive/1.6.9.zip
ARDUINO_ZIP   := $(notdir $(ARDUINO_URL))

help:
	@echo "Install dependencies: sudo make install-dep"
	@echo "Set up the build env: make setup"
	@echo "Compile only: make compile sketch=<file>.ino"
	@echo "Compile and upload: make upload sketch=<file>.ino"

check-root:
	@if [ `whoami` != root ]; then echo "Please run as sudoer/root" ; exit 1 ; fi

install-dep: check-root
	apt-get update
	apt-get install -y python
	cp -f $(ARDUINOSW_DIR)/bin/99-dfu.rules /etc/udev/rules.d/99-dfu.rules
	cp -f $(ARDUINOSW_DIR)/bin/99-ftdi.rules /etc/udev/rules.d/99-ftdi.rules

setup: arc32 setup-arduino-ide

corelibs:
	$(ARDUINOSW_DIR)/bin/parse_json.py; \
	unzip $(CORELIBS_ZIP) ; \
	mv corelibs* corelibs ;\
	rm $(CORELIBS_ZIP)

arc32:
	@echo "Downloading ARC Toolchain"
	cd /tmp; curl -OL $(TOOLCHAIN_URL)
	@echo "Unpacking ARC Toolchain"
	tar xf /tmp/$(TOOLCHAIN) -C $(ARDUINOSW_DIR)
	rm /tmp/$(TOOLCHAIN)

setup-arduino-ide:
	rm -rf arduino-ide 2>/dev/null
	@echo "Downloading Arduino IDE"
	cd /tmp; curl -OL $(ARDUINO_URL)
	@echo "Unpacking Arduino IDE"
	unzip /tmp/$(ARDUINO_ZIP) Arduino*/libraries/* -d $(ARDUINOSW_DIR)
	mv Arduino-* arduino-ide
	rm /tmp/$(ARDUINO_ZIP)

