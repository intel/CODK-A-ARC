ifeq ("$(strip $(CODK_DIR))", "")
  $(error Please set the CODK_DIR variable.)
endif

ARDUINOSW_DIR ?=$(CODK_DIR)/arduino101_software

current_dir = $(shell pwd)

VERBOSE  = true

LIBDIRS  =  $(ARDUINOSW_DIR)/corelibs/libraries/CurieEEPROM/src

include $(ARDUINOSW_DIR)/Makefile.inc

all: compile

.DEFAULT_GOAL := all
