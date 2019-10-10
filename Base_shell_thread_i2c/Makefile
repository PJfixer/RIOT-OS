APPLICATION=helloworld
INCLUDES += -I$(CURDIR)/inc
DIRS += $(CURDIR)/src
BOARD ?= native
USEMODULE += shell shell_commands ps xtimer  periph_i2c lcd_grove 
FEATURES_REQUIRED += periph_i2c
RIOTBASE ?= $(CURDIR)/../../../RIOT

DEVELHELP ?= 1

include $(RIOTBASE)/Makefile.include
