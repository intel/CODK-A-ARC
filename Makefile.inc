ARC32_DIR      = $(ARDUINOSW_DIR)/arc32
CPP            = $(ARC32_DIR)/bin/arc-elf32-g++
CC             = $(ARC32_DIR)/bin/arc-elf32-gcc
AR             = $(ARC32_DIR)/bin/arc-elf32-ar
STRIP          = $(ARC32_DIR)/bin/arc-elf32-strip
OBJCOPY        = $(ARC32_DIR)/bin/arc-elf32-objcopy
sketch_converter = $(ARDUINOSW_DIR)/sketch-converter

.PHONY: upload convert-sketch
ARDUINO_IDE=10607
ARDUINOIDE_DIR = $(ARDUINOSW_DIR)/arduino-ide

SERIAL_PORT ?= /dev/ttyACM0
VERBOSE ?= false

CORELIBS_DIR=$(ARDUINOSW_DIR)/corelibs
CORE_SRCS := $(shell find $(CORELIBS_DIR)/cores/arduino -name "*.c" 2>/dev/null) \
            $(shell find $(CORELIBS_DIR)/cores/arduino -name "*.cpp" 2>/dev/null) \
            $(shell find $(CORELIBS_DIR)/variants/arduino_101 -name "*.c" 2>/dev/null) \
            $(shell find $(CORELIBS_DIR)/variants/arduino_101 -name "*.cpp" 2>/dev/null) 
CORE_OBJS1  = $(CORE_SRCS:.c=.o) 
CORE_OBJS  = $(CORE_OBJS1:.cpp=.o) 

SRCS    := $(wildcard *.c) \
           $(wildcard *.cpp) \

OBJS1 := $(SRCS:.c=.o)
OBJS  := $(OBJS1:.cpp=.o)

INCL    += $(foreach dir,$(LIBDIRS), -I$(dir) )
LIBSRCS += $(foreach dir,$(LIBDIRS), $(shell find $(dir) -maxdepth 1 -name "*.cpp" 2>/dev/null;) )

LIBOBJS  := $(LIBSRCS:.cpp=.o)

OUTNAME = arc

INCL += -I $(CORELIBS_DIR)/system/libarc32_arduino101/common \
	-I $(CORELIBS_DIR)/variants/arduino_101 \
	-I $(CORELIBS_DIR)/system/libarc32_arduino101/drivers \
	-I $(CORELIBS_DIR)/system/libarc32_arduino101/bootcode \
	-I $(CORELIBS_DIR)/system/libarc32_arduino101/framework/include \
	-I $(CORELIBS_DIR)/cores/arduino \
	-I $(CORELIBS_DIR)/libraries/Adafruit_NeoPixel \
	-I $(CORELIBS_DIR)/libraries/CurieBLE/src \
	-I $(CORELIBS_DIR)/libraries/CurieEEPROM/src \
	-I $(CORELIBS_DIR)/libraries/CurieI2S/src \
	-I $(CORELIBS_DIR)/libraries/CurieIMU/src \
	-I $(CORELIBS_DIR)/libraries/CurieSoftwareSerial/src \
	-I $(CORELIBS_DIR)/libraries/CurieTime/src \
	-I $(CORELIBS_DIR)/libraries/CurieTimerOne/src \
	-I $(CORELIBS_DIR)/libraries/SerialFlash/src \
	-I $(CORELIBS_DIR)/libraries/Servo/src \
	-I $(CORELIBS_DIR)/libraries/SPI/src \
	-I $(CORELIBS_DIR)/libraries/Wire/src \

CFLAGS=-c -mcpu=quarkse_em -mlittle-endian -g -Os -Wall -fno-reorder-functions -fno-asynchronous-unwind-tables -fno-omit-frame-pointer -fno-defer-pop -Wno-unused-but-set-variable -Wno-main -ffreestanding -fno-stack-protector -mno-sdata -ffunction-sections -fdata-sections -fsigned-char -MMD -D__ARDUINO_ARC__
CPPFLAGS=-c -mcpu=quarkse_em -mlittle-endian -g -Os -Wall -fno-reorder-functions -fno-asynchronous-unwind-tables -fno-omit-frame-pointer -fno-defer-pop -Wno-unused-but-set-variable -Wno-main -ffreestanding -fno-stack-protector -mno-sdata -ffunction-sections -fdata-sections -fsigned-char -MMD -fno-rtti -fno-exceptions -D__ARDUINO_ARC__ -std=c++11
EXTRA_CPPFLAGS=-D__CPU_ARC__ -DCLOCK_SPEED=32 -DCONFIG_SOC_GPIO_32 -DCONFIG_SOC_GPIO_AON -DINFRA_MULTI_CPU_SUPPORT -DCFW_MULTI_CPU_SUPPORT -DHAS_SHARED_MEM "-I$(CORELIBS_DIR)/libarc32_arduino101/common" "-I$(CORELIBS_DIR)/libarc32_arduino101/drivers" "-I$(CORELIBS_DIR)/libarc32_arduino101/bootcode" "-I$(CORELIBS_DIR)/libarc32_arduino101/framework/include"

ELFFLAGS=-nostartfiles -nodefaultlibs -nostdlib -static -Wl,-X -Wl,-N -Wl,-mcpu=quarkse_em -Wl,-marcelf -Wl,--gc-sections
ARFLAGS=rcs

arduino=$(ARDUINOSW_DIR)/sketch-converter
LIB_DIR=$arduino/../libraries

CORE_A = $(CORELIBS_DIR)/cores/arduino/core.a

CHECK = splint

BUILDDIR := $(shell mktemp -d)

check_defined = \
    $(foreach 1,$1,$(__check_defined))
__check_defined = \
    $(if $(value $1),, \
      $(error Undefined $1$(if $(value 2), ($(strip $2)))))

$(call check_defined, CODK_DIR, \
	    	      BUILDDIR, \
		      CORELIBS_DIR)

### And here is how to use it ###


debug: 
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(LIBSRCS)

libarc32_arduino101:
	cd $(CORELIBS_DIR)/system/libarc32_arduino101/; make; cd -

%.o: %.cpp
	@echo "------"
	$(CPP) -c $(CPPFLAGS) -DF_CPU=32000000L -DARDUINO=10609 -DARDUINO_ARC32_TOOLS \
	       -DARDUINO_ARCH_ARC32 $(EXTRA_CPPFLAGS) $(INCL) $< -o $@

%.o: %.c
	@echo "------"
	$(CC) $(CFLAGS) -DF_CPU=32000000L -DARDUINO=10609 -DARDUINO_ARC32_TOOLS \
	      -DARDUINO_ARCH_ARC32 $(EXTRA_CPPFLAGS) $(INCL) $< -o $@

check_src:
ifeq "$(strip $(SRCS))" "" 
	$(error No source files detected.)
else
	@echo ...$(SRCS)...
endif

$(CORE_A): check_src $(CORE_OBJS)
	for i in $(CORE_OBJS); do \
	  $(AR) $(ARFLAGS) $(CORE_A) $$i; \
	done

$(OUTNAME).elf: $(OBJS) $(LIBOBJS) $(CORE_A) 
	$(CC) $(ELFFLAGS) "-T$(CORELIBS_DIR)/variants/arduino_101/linker_scripts/flash.ld" \
	"-Wl,-Map,$(OUTNAME).map" -o $@ "-L." "-L$(CORELIBS_DIR)/variants/arduino_101" \
	-Wl,--whole-archive "-larc32drv_arduino101" -Wl,--no-whole-archive \
	-Wl,--start-group "-larc32drv_arduino101" -lnsim -lc -lm -lgcc $^ 
	cp $@ arc-debug.elf

compile: $(OUTNAME).elf
	$(OBJCOPY) -S -O binary -R .note -R .comment -R COMMON -R .eh_frame $< arc.bin
	$(STRIP) $(OUTNAME).elf
	$(OBJCOPY) -S -O ihex -R .note -R .comment -R COMMON -R .eh_frame  $< $(OUTNAME).hex

upload: compile
	@echo Resetting port $(SERIAL_PORT)
	$(ARDUINOSW_DIR)/bin/resetPort.py $(SERIAL_PORT)
	sleep 1
	@echo Uploading $< to port $(SERIAL_PORT)
	$(ARDUINOSW_DIR)/bin/arduino101load $(ARDUINOSW_DIR)/bin/x86/bin $(OUTNAME).bin $(SERIAL_PORT) $(VERBOSE)

check: $(CPPSRCS)
	@echo "------"
	$(CHECK) $(INCL) $<

convert-sketch: 
	rm -rf $(current_dir)/tmp
	rm -f $(SKETCH).cpp
	mkdir $(current_dir)/tmp
	$(sketch_converter)/arduino-builder \
	-compile \
	-logger=machine \
	-hardware "$(sketch_converter)/hardware" \
	-hardware "$(sketch_converter)/packages" \
	-tools "$(sketch_converter)/tools-builder" \
	-tools "$(sketch_converter)/packages" \
	-libraries "$(ARDUINOSW_DIR)/libraries" \
	-libraries "$(ARDUINOIDE_DIR)/libraries" \
	-fqbn=Intel:arc32:arduino_101 \
	-vid-pid=0X8087_0X0AB6 \
	-ide-version=10608 \
	-warnings=none \
	-build-path=$(current_dir)/tmp \
	-prefs=build.warn_data_percentage=75 \
	"$(SKETCH)"  
	cp $(current_dir)/tmp/sketch/$(SKETCH).cpp $(current_dir)

clean-all: clean
	@echo Removing $(CORE_OBJS) $(CORE_A) $(CORELIBS_DIR)/cores/arduino/core.a
	rm -f $(CORE_OBJS) $(CORE_A) $(CORELIBS_DIR)/cores/arduino/core.a

clean:
	@echo Removing objects
	rm -f *.o $(OBJS) $(LIBOBJS) *.S *.elf *.bin *.hex *.d *.map\
              arc-debug.elf $(SRCS:%.cpp=%.d) $(SRCS:%.cpp=%.s)

.PRECIOUS: %.cpp
