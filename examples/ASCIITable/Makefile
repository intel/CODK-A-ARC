ifeq ("$(strip $(CODK_DIR))", "")
  $(error Please set the CODK_DIR variable.)
endif

ARDUINOSW_DIR ?=$(CODK_DIR)/software
ARDUINOIDE_DIR ?= $(ARDUINOSW_DIR)/arduino-ide

current_dir = $(shell pwd)

OBJS     := $(SRCS:.cpp=.o)
VERBOSE   = true

# Add custom library include paths and source folders
INCL    += 
LIBDIRS  =  

LIBSRCS += $(foreach dir,$(LIBDIRS), $(shell find $(dir) -maxdepth 1 -name "*.cpp" 2>/dev/null;) )

include $(ARDUINOSW_DIR)/Makefile.inc

all: upload

.DEFAULT_GOAL := all
