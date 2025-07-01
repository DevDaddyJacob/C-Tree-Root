# ===== USER SETTINGS =========================================================

PLATFORM = guess

C_STANDARD = 99

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g $(SYSCFLAGS)

UNAME = uname

RM = rm -rf
MKDIR = mkdir -p


# ===== MAKE VARS =============================================================

OUTPUT_NAME = treeroot

PLATFORMS = guess windows macosx linux

CFLAGS += -std=c$(C_STANDARD)

SOURCE_DIR = src
BINARY_DIR = bin
OBJECT_DIR = obj

SOURCES = $(wildcard $(SOURCE_DIR)/*.c) $(wildcard $(SOURCE_DIR)/**/*.c)
HEADERS = $(wildcard $(SOURCE_DIR)/*.h) $(wildcard $(SOURCE_DIR)/**/*.h)
OBJECTS = $(addprefix $(OBJECT_DIR)/, $(subst $(SOURCE_DIR)/, , $(SOURCES:%.c=%.o)))

OUTPUT_FILE = $(BINARY_DIR)/$(OUTPUT_NAME)

ALL = all

# ===== TARGETS ===============================================================

default: $(PLATFORM)

echo:
	@ echo "MAKE = $(MAKE)"
	@ echo "UNAME = $(UNAME)"
	@ echo "PLATFORM = $(PLATFORM)"
	@ echo "PLATFORMS = $(PLATFORMS)"
	@ echo "RM = $(RM)"
	@ echo "MKDIR = $(MKDIR)"
	@ echo "C_STANDARD = $(C_STANDARD)"
	@ echo "CSTD = $(CSTD)"
	@ echo "CC = $(CC)"


all: clean $(OUTPUT_FILE)


# Remove all of the build files
clean:
	@ $(RM) $(BINARY_DIR)
	@ $(RM) $(OBJECT_DIR)


# Link the interpreter.
$(OUTPUT_FILE): $(OBJECTS)
	@ echo "*" $(CC) -o $@ $(OBJECTS) $(CFLAGS)
	@ $(MKDIR) "./$(BINARY_DIR)"
	@ $(CC) -o $@ $(OBJECTS) $(CFLAGS)


# Compile object files.
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	@ echo "*" $(CC) -c -o $@ $< $(CFLAGS)
	@ $(MKDIR) $(addprefix ./, $(dir $(OBJECTS)))
	@ $(CC) -c -o $@ $< $(CFLAGS)


guess:
	@ echo "*" Guessing `$(UNAME)`
	@ $(MAKE) `$(UNAME)`


CYGWIN_NT-10.0-22631 windows:
	@ echo "> building for windows"
	@ echo "*" $(MAKE) $(ALL) SYSCFLAGS=-DOS_WINDOWS
	@ $(MAKE) $(ALL) SYSCFLAGS=-DOS_WINDOWS


Darwin macos macosx:
	@ echo "> building for macos"
	@ echo "*" $(MAKE) $(MAKE) $(ALL) SYSCFLAGS="-DOS_MAC"
	@ $(MAKE) $(ALL) SYSCFLAGS="-DOS_MAC"


Linux linux:
	@ echo "> building for linux"
	@ echo "*" $(MAKE) $(ALL) SYSCFLAGS="-DOS_LINUX"
	@ $(MAKE) $(ALL) SYSCFLAGS="-DOS_LINUX"
