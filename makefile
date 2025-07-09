# ===== COMMANDS SETTINGS =====================================================

UNAME = uname
RM = rm -rf
MKDIR = mkdir -p
CC = gcc


# ===== ENVIRONMENT VARIABLES =================================================

# If the build should log progress to the console or be silent
# Usage:
# 	`make VERBOSE=1` 	For verbose build logging
# 	`make VERBOSE=0` 	For silent build logging (default)
VERBOSE ?= 0


# ===== SYSTEM DETECTION ======================================================

# Determine the OS being used
OS_TYPE := $(shell $(UNAME) -s | tr '[:upper:]' '[:lower:]')
ifneq (,$(findstring cygwin_nt,$(OS_TYPE)))
	OS_TYPE = windows
endif


# Determine the architecture type being used
ARCH_TYPE := $(shell $(UNAME) -m | tr '[:upper:]' '[:lower:]')


# Determine the extension on the executable file that is created
ifeq (,$(findstring windows,$(OS_TYPE)))
	EXE_EXT := .exe
else
	EXE_EXT := 
endif


# ===== BUILD VARIABLES =======================================================

# The ISO C standard to use for compilation
C_STANDARD := 99


# The C compilation flags to use
C_FLAGS ?=
CFLAGS := -Wall -Wextra -pedantic -std=c$(C_STANDARD) $(C_FLAGS)


# Flags to use with calls to the make command
MAKE_FLAGS :=

# Add verbose flag
MAKE_FLAGS += VERBOSE=$(VERBOSE)

# Add OS make flag
ifeq ($(OS_TYPE),windows)

else ifeq ($(OS_TYPE),darwin)

else ifeq ($(OS_TYPE),macos)

else ifeq ($(OS_TYPE),macosx)
endif

# Add silent make flag
ifeq ($(VERBOSE),0)
	MAKE_FLAGS += --silent
endif





# ===== MAKE VARS =============================================================

# $1 	Root directory
# $2	Regex patern to match
_rwildcard=$(shell node tools/recursive_wildcard.js $1 $2)

SOURCE_DIR = src
BINARY_DIR = bin
OBJECT_DIR = obj
TEST_DIR = test
TEST_RUN_DIR = $(TEST_DIR)/run
BENCHMARK_DIR = benchmark
BENCHMARK_RUN_DIR = $(BENCHMARK_DIR)/run

SOURCES = $(shell node tools/recursive_wildcard.js $(SOURCE_DIR) .*\.c)
HEADERS = $(shell node tools/recursive_wildcard.js $(SOURCE_DIR) .*\.h)
OBJECTS = $(addprefix $(OBJECT_DIR)/, $(subst $(SOURCE_DIR)/, , $(SOURCES:%.c=%.o)))

EXE = treeroot$(EXE_EXT)


# ===== TARGETS ===============================================================

# ===== MISC TARGETS ==========================================================

.DEFAULT: help
		
.PHONY: help
.NOTPARALLEL: help
## Prints this menu
help:
	@ printf "For more targets and info see the comments in the Makefile.\n\n"
	@ grep --no-group-separator -A 1 '^## .*' Makefile | \
		sed 'N;s/\n/[SEP_RAR]/' | \
		sed -r 's/## (.*)\[SEP_RAR\](.*):.*/\2=SEP_RAR=\1/' | \
		sort | \
		awk 'BEGIN {FS = "=SEP_RAR="}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'


.PHONY: dump
.NOTPARALLEL: dump
## Dumps variables to the console, useful for debugging
dump:
	@ echo "=====[ COMMANDS SETTINGS ]====="
	@ echo "MAKE = $(MAKE)"
	@ echo "UNAME = $(UNAME)"
	@ echo "RM = $(RM)"
	@ echo "MKDIR = $(MKDIR)"
	@ echo "CC = $(CC)"
	@ echo "=====[ ENVIRONMENT VARIABLES ]====="
	@ echo "VERBOSE = $(VERBOSE)"
	@ echo "=====[ SYSTEM DETECTION ]====="
	@ echo "OS_TYPE = $(OS_TYPE)"
	@ echo "ARCH_TYPE = $(ARCH_TYPE)"
	@ echo "EXE_EXT = $(EXE_EXT)"
	@ echo "=====[ BUILD VARIABLES ]====="
	@ echo "C_STANDARD = $(C_STANDARD)"
	@ echo "CFLAGS = $(CFLAGS)"
	@ echo "MAKE_FLAGS = $(MAKE_FLAGS)"
	@ echo "=====[ MAKE VARS ]====="
	@ echo "SOURCE_DIR = $(SOURCE_DIR)"
	@ echo "BINARY_DIR = $(BINARY_DIR)"
	@ echo "OBJECT_DIR = $(OBJECT_DIR)"
	@ echo "TEST_DIR = $(TEST_DIR)"
	@ echo "TEST_RUN_DIR = $(TEST_RUN_DIR)"
	@ echo "BENCHMARK_DIR = $(BENCHMARK_DIR)"
	@ echo "BENCHMARK_RUN_DIR = $(BENCHMARK_RUN_DIR)"
	@ echo "SOURCES = $(SOURCES)"
	@ echo "HEADERS = $(HEADERS)"
	@ echo "OBJECTS = $(OBJECTS)"
	@ echo "EXE = $(EXE)"


.PHONY: clean
.NOTPARALLEL: clean
## Alias for `clean-all`
clean: clean-all


.PHONY: clean-all
.NOTPARALLEL: clean-all
## Runs all of the clean jobs
clean-all: clean-binary clean-object


.PHONY: clean-binary
.NOTPARALLEL: clean-binary
## Cleans and deletes the files in the binary directory
clean-binary:
	@ $(RM) $(BINARY_DIR)


.PHONY: clean-object
.NOTPARALLEL: clean-object
## Cleans and deletes the files in the object directory
clean-object:
	@ $(RM) $(OBJECT_DIR)


# ===== BENCHMARK TARGETS =====================================================

.PHONY: benchmark
.NOTPARALLEL: benchmark
## Alias for `benchmark-all`
benchmark: benchmark-all

.PHONY: benchmark-all
.NOTPARALLEL: benchmark-all
## Runs all benchmarks
benchmark-all:
	@ echo placeholder for $@


# ===== TEST TARGETS ==========================================================

.PHONY: test
.NOTPARALLEL: test
## Alias for `test-all`
test: test-all


.PHONY: test-all
.NOTPARALLEL: test-all
## Runs all tests
test-all:
	@ echo placeholder for $@


# ===== BUILD TARGETS =========================================================

## Cleans the bin and obj folders before building
build-force: clean-binary clean-object build


## Builds the project into an executable
build:
ifeq ($(VERBOSE),1)
	@ echo "> building for $(OS_TYPE)"
	@ echo "*" $(MAKE) $(EXE) $(MAKE_FLAGS)
endif
	@ $(MAKE) $(EXE) $(MAKE_FLAGS)


# Link the interpreter.
$(EXE): $(OBJECTS)
ifeq ($(VERBOSE),1)
	@ echo "*" $(CC) -o ./$(BINARY_DIR)/$@ $(OBJECTS) $(CFLAGS)
endif
	@ $(MKDIR) "./$(BINARY_DIR)"
	@ $(CC) -o ./$(BINARY_DIR)/$@ $(OBJECTS) $(CFLAGS)


# Compile object files.
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
ifeq ($(VERBOSE),1)
	@ echo "*" $(CC) -c -o $@ $< $(CFLAGS)
endif
	@ $(MKDIR) $(addprefix ./, $(dir $(OBJECTS)))
	@ $(CC) -c -o $@ $< $(CFLAGS)
