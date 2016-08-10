ifeq ("$(strip $(CODK_DIR))", "")
  $(error Please set the CODK_DIR variable.)
endif

ARDUINOSW_DIR ?=$(CODK_DIR)/software
ARDUINOIDE_DIR ?= $(ARDUINOSW_DIR)/arduino-ide

current_dir = $(shell pwd)

VERBOSE  = true

LIBDIRS  =  

include $(ARDUINOSW_DIR)/Makefile.inc

all: compile

.DEFAULT_GOAL := all
