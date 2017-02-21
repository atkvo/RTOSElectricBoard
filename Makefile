EXECUTABLE = spilab


BUILDDIR = build
LIB_PATHS = 
INCLUDE_PATHS = 

# SJONE BOARD PROGRAMMING
HYPERLOAD_PATH = ./hyperload.py
HYPERLOAD_PORT =  /dev/tty.usbserial-A503JOHW

# SJONE BOARD SERIAL CONNECTION
BAUD_RATE = 38400
# Uses miniterm.py as the serial COM device
# Uses HYPERLOAD_PORT as the communication port

## Uncomment below to enable build WITH mpu
# WITH_MPU = 1

# ------- START OVERRIDES --------
# *** Define this to override ARM base library paths (parent folder that contains lib/include)
# GCC_BASE_OVERRIDE = /usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/arm-none-eabi/
# GCC_BASE_OVERRIDE = /Users/atkvo/Downloads/dev/arm/gcc-arm-none-eabi-4_9-2015q3/arm-none-eabi/

# *** Define this to override arm-none-eabi-* binaries
# GCC_BIN_BASE_OVERRIDE = /usr/local/Caskroom/gcc-arm-embedded/5_4-2016q2,20160622/gcc-arm-none-eabi-5_4-2016q2/bin/
# GCC_BIN_BASE_OVERRIDE = /Users/atkvo/Downloads/dev/arm/gcc-arm-none-eabi-4_9-2015q3/bin/
# ------- END OVERRIDES --------

## Should not need to edit below this line
## ----------------------------------------

ifdef GCC_BIN_BASE_OVERRIDE
	CPP = $(GCC_BIN_BASE)/arm-none-eabi-g++
	CC = $(GCC_BIN_BASE)/arm-none-eabi-gcc
	OBJCOPY = $(GCC_BIN_BASE)/arm-none-eabi-objcopy
	LD = $(GCC_BIN_BASE)/arm-none-eabi-ld
else
	CPP = arm-none-eabi-g++
	CC = arm-none-eabi-gcc
	OBJCOPY = arm-none-eabi-objcopy
	LD = arm-none-eabi-ld
endif


SOURCES_CPP := $(shell find . -name '*.cpp')
ifdef WITH_MPU
	SOURCES_C := $(shell find .  -name '*.c')
else
	SOURCES_C := $(shell find .  -name '*.c' | grep -v portable/mpu)
endif

SOURCES := $(SOURCES_CPP)
SOURCES += $(SOURCES_C)

OBJECTS_BASE := $(patsubst %.cpp, %.cpp.o,$(SOURCES_CPP))
OBJECTS_BASE += $(patsubst %.c, %.c.o,$(SOURCES_C))
# Prepend objects with the build directory path now so they're placed outside of source
OBJECTS := $(addprefix $(BUILDDIR)/,$(OBJECTS_BASE))

# CORTEX SPECIFIC
CORTEX_M3_LIB = armv7-m
CORTEX_M3_CFLAGS = -mthumb -mcpu=cortex-m3
MCU_CC_FLAGS = $(CORTEX_M3_CFLAGS)
# -T "" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"test.map" -specs=nano.specs
# LDFLAGS = -Wl,-nostartfiles,-specs=nano.specs,-Map,$(EXECUTABLE).map,-Tloader.ld,-nostartfiles
LDFLAGS = -Wl,-Map,$(BUILDDIR)/$(EXECUTABLE).map
# LDFLAGS := -nostartfiles -specs=nano.specs
# LDFLAGS += -Map=$(EXECUTABLE).map -T loader.ld
# LDFLAGS += -Xlinker --gc-sections

CFLAGS = $(CORTEX_M3_CFLAGS)
CFLAGS += -nostartfiles
CFLAGS += -fdata-sections
CFLAGS += --specs=nano.specs
CFLAGS += -T "loader.ld"
# CFLAGS += -ffunctions-sections
CFLAGS += -fmessage-length=0
# CFLAGS += -std=c++11
CFLAGS += -Xlinker --gc-sections
CFLAGS += -fabi-version=0
CFLAGS += -fno-exceptions
CFLAGS += -fpermissive
CFLAGS += -Wall -Wshadow -Wlogical-op -Wfloat-equal
ifdef WITH_MPU
	CFLAGS += -DBUILD_CFG_MPU=1
else
	CFLAGS += -DBUILD_CFG_MPU=0
endif

INCLUDE_PATHS += -I.
INCLUDE_PATHS += -Inewlib
INCLUDE_PATHS += -IL0_LowLevel
INCLUDE_PATHS += -IL0_LowLevel/include
INCLUDE_PATHS += -IL1_FreeRTOS
INCLUDE_PATHS += -IL1_FreeRTOS/include
INCLUDE_PATHS += -IL1_FreeRTOS/portable
INCLUDE_PATHS += -IL2_Drivers
INCLUDE_PATHS += -IL2_Drivers/base
INCLUDE_PATHS += -IL3_Utils
INCLUDE_PATHS += -IL3_Utils/tlm
INCLUDE_PATHS += -IL4_IO
INCLUDE_PATHS += -IL4_IO/fat
INCLUDE_PATHS += -IL4_IO/wireless
INCLUDE_PATHS += -IL5_Application

## Handle overrides
ifdef GCC_BASE_OVERRIDE
	LIB_PATHS += $(addprefix $(GCC_LIB), $(CORTEX_M3_LIB))
else
	LIB_PATHS += $(CORTEX_M3_LIB)
endif

ifdef GCC_BASE_OVERRIDE
	GCC_LIB = $(GCC_BASE_OVERRIDE)/lib
	GCC_INC = $(GCC_BASE_OVERRIDE)/include
	INCLUDE_PATHS += -I$(GCC_INC)
	CFLAGS += -L$(GCC_LIB)
endif
## ------------------

CFLAGS += $(INCLUDE_PATHS)
CFLAGS += $(addprefix -L, $(LIB_PATHS))

all: $(BUILDDIR)/$(EXECUTABLE).hex

$(BUILDDIR)/$(EXECUTABLE).hex: $(BUILDDIR)/$(EXECUTABLE)
	echo "CREATING HEX\n"
	$(OBJCOPY) -O ihex $(BUILDDIR)/$(EXECUTABLE) $(BUILDDIR)/$(EXECUTABLE).hex

$(BUILDDIR)/$(EXECUTABLE): $(OBJECTS)
	echo "LINKING\n"
	# $(CPP) $(CFLAGS) $(addprefix $(BUILDDIR), $(OBJECTS)) -o $(BUILDDIR)/$(EXECUTABLE) $(LDFLAGS)
	$(CPP) $(CFLAGS) $(OBJECTS) -o $(BUILDDIR)/$(EXECUTABLE) $(LDFLAGS)

$(BUILDDIR)/%.cpp.o: %.cpp
# %.cpp.o: %.cpp
	echo "COMPILING\n"
	mkdir -p $(BUILDDIR)/$(shell dirname $<)
	# $(CPP) $(CFLAGS) -c $< -o $(BUILDDIR)/$@
	$(CPP) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.c.o: %.c
# %.c.o: %.c
	echo "COMPILING\n"
	mkdir -p $(BUILDDIR)/$(shell dirname $<)
	# $(CC) $(CFLAGS) -c $< -o $(BUILDDIR)/$@
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	# rm -f $(addprefix $(BUILDDIR), $(OBJECTS))
	rm -f $(OBJECTS)
	rm -f $(BUILDDIR)/$(EXECUTABLE)
	rm -f $(BUILDDIR)/$(EXECUTABLE).hex
	rm -f $(BUILDDIR)/$(EXECUTABLE).map
	echo "----" $(EXECUTABLE) "cleaned\n"

program:
	python $(HYPERLOAD_PATH) -f $(BUILDDIR)/$(EXECUTABLE).hex -p $(HYPERLOAD_PORT)

connect:
	miniterm.py $(HYPERLOAD_PORT) $(BAUD_RATE) --rts 0 --dtr 0


print-%: ; @echo $* = $($*)
