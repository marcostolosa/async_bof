# Async BOF Makefile

# Compiler
CC=x86_64-w64-mingw32-gcc
LOCAL_CC=gcc

# Directories
SRC_DIR=src
INC_DIR=include
OUT_DIR=bin

# Compile flags
CFLAGS=-c -Wall -I$(INC_DIR) -DBOF_BUILD
LOCAL_CFLAGS=-c -Wall -I$(INC_DIR) -DLOCAL_TEST
LDFLAGS=

# Source files
SOURCES=$(SRC_DIR)/async_bof.c
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o)

# Target file
TARGET=$(OUT_DIR)/async_bof.o
LOCAL_TARGET=$(OUT_DIR)/async_bof_local.exe

# Default target
all: $(TARGET)

# Create output directory
$(OUT_DIR):
	if not exist "$(OUT_DIR)" mkdir "$(OUT_DIR)"

# Compile source files for BOF
$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Local test version
local: $(LOCAL_TARGET)

$(LOCAL_TARGET): $(SRC_DIR)/async_bof.c | $(OUT_DIR)
	$(LOCAL_CC) -Wall -I$(INC_DIR) -DLOCAL_TEST -o $@ $<

# Local test object file only
local-obj: $(OUT_DIR)/async_bof_local.o

$(OUT_DIR)/async_bof_local.o: $(SRC_DIR)/async_bof.c | $(OUT_DIR)
	$(LOCAL_CC) $(LOCAL_CFLAGS) -o $@ $<

# Test the local version
test: local
	$(LOCAL_TARGET)

# Clean
clean:
	if exist "$(OUT_DIR)" rmdir /s /q "$(OUT_DIR)"

# Install to Cobalt Strike directory (needs to be modified according to actual path)
install: $(TARGET)
	cp $(TARGET) /path/to/cobaltstrike/async_bof.o

.PHONY: all clean local local-obj test install