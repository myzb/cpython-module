# ------------------------------------------------------------------------------
# PYTHON MODULE C-EXTENSION MAKEFILE
#
# This file was generated using the build output provided by setuptools setup.py
# > build flags have to match whatever python was compiled with!
# ------------------------------------------------------------------------------

ARCH   ?= linux64

ifeq ($(filter linux64 win64, $(ARCH)), )
$(error Error: Only 64-bit builds are supported)
endif

MODULE	= mymodule

CDEFS	= -DDEBUG=0
OUT_DIR	= .out-$(ARCH)

OBJS	= \
	$(OUT_DIR)/mymodule.o \
	$(OUT_DIR)/myclass.o

# Windows
# Note, build flags must match whatever python was compiled with
# to find out use: python3 -m sysconfig | less
ifeq ($(ARCH),win64)
CC	= x86_64-w64-mingw32-gcc
OPT	= -O

# note: LDNAME must be $(MODULE).<arch-ext-suffix>.so or $(MODULE).so
# to find out use: python3 -m sysconfig | less | grep EXT_SUFFIX
LIB_EXT = .cp36-win_amd64.pyd
LDNAME	= $(MODULE)$(LIB_EXT)
PY_HOME	= /path/to/Python
PY_VER	= Python36

LIBS	+= \
	-lpython36

INC_DIR = \
	-I$(PY_HOME)/$(PY_VER)/include
LIB_DIR = \
	-L$(PY_HOME)/$(PY_VER)/libs

LDFLAGS	= -s
CFLAGS	= -mdll
endif

# Linux
# Note, build flags must match whatever python was compiled with
# to find out use: python3 -m sysconfig | less
ifeq ($(ARCH),linux64)
CC	= gcc
OPT	= -O3 -g

# note: LDNAME must be $(MODULE).<arch-ext-suffix>.so or $(MODULE).so
# to find out use: python3 -m sysconfig | less | grep EXT_SUFFIX
LIB_EXT = .cpython-310-x86_64-linux-gnu.so
LDNAME	= $(MODULE)$(LIB_EXT)
INC_DIR	= -I/usr/include/python3.10

LDFLAGS	= -pthread
CFLAGS	= -pthread -Wno-unused-result -Wsign-compare -fwrapv -fPIC -DNDEBUG
endif

TARGET  = $(OUT_DIR)/$(LDNAME)

CFLAGS	+= -Wall $(OPT) $(CDEFS) $(INC_DIR)
LDFLAGS	+= -shared $(LIB_DIR)

# ------------------------------------------------------------------------------

all: directories $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	@echo 'Finished building target: $@'
	@echo ' '

$(OUT_DIR)/%.o : %.c
	@echo " CC	$@"
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	-mkdir -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)
