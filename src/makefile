#**************************************************************************************************
#
#   raylib makefile for Desktop platforms, Web (Wasm), Raspberry Pi (DRM mode) and Android
#
#   Copyright (c) 2013-2025 Ramon Santamaria (@raysan5)
#
#   This software is provided "as-is", without any express or implied warranty. In no event
#   will the authors be held liable for any damages arising from the use of this software.
#
#   Permission is granted to anyone to use this software for any purpose, including commercial
#   applications, and to alter it and redistribute it freely, subject to the following restrictions:
#
#     1. The origin of this software must not be misrepresented; you must not claim that you
#     wrote the original software. If you use this software in a product, an acknowledgment
#     in the product documentation would be appreciated but is not required.
#
#     2. Altered source versions must be plainly marked as such, and must not be misrepresented
#     as being the original software.
#
#     3. This notice may not be removed or altered from any source distribution.
#
#**************************************************************************************************

.PHONY: all clean

# Define required environment variables
#------------------------------------------------------------------------------------------------
# Define target platform: PLATFORM_DESKTOP, PLATFORM_WEB, PLATFORM_DRM, PLATFORM_ANDROID
PLATFORM              ?= PLATFORM_WEB

# Define project variables
PROJECT_NAME          ?= noteren
PROJECT_VERSION       ?= 1.0
PROJECT_BUILD_PATH    ?= ../bin
PROJECT_SOURCE_FILES  := $(wildcard *.c */*.c)

# raylib library variables
RAYLIB_SRC_PATH       ?= ../include/
RAYLIB_INCLUDE_PATH   ?= $(RAYLIB_SRC_PATH)/raylib/src
RAYLIB_LIB_PATH       ?= $(RAYLIB_SRC_PATH)

# Library type used for raylib: STATIC (.a) or SHARED (.so/.dll)
RAYLIB_LIBTYPE        ?= STATIC

# Build mode for project: DEBUG or RELEASE
BUILD_MODE            ?= RELEASE

# PLATFORM_WEB: Default properties
BUILD_WEB_ASYNCIFY    ?= TRUE
BUILD_WEB_SHELL       ?= shell.html
BUILD_WEB_HEAP_SIZE   ?= 128MB
BUILD_WEB_STACK_SIZE  ?= 1MB
BUILD_WEB_ASYNCIFY_STACK_SIZE ?= 1048576
BUILD_WEB_RESOURCES   ?= FALSE
BUILD_WEB_RESOURCES_PATH  ?= resources

# Determine PLATFORM_OS in case PLATFORM_DESKTOP selected
ifeq ($(PLATFORM),PLATFORM_WEB)
    ifeq ($(OS),Windows_NT)
        PLATFORM_OS = WINDOWS
        ifndef PLATFORM_SHELL
            PLATFORM_SHELL = cmd
        endif
    else
        UNAMEOS = $(shell uname)
        ifeq ($(UNAMEOS),Linux)
            PLATFORM_OS = LINUX
        endif
        ifeq ($(UNAMEOS),Darwin)
            PLATFORM_OS = OSX
        endif
        ifndef PLATFORM_SHELL
            PLATFORM_SHELL = sh
        endif
    endif
endif

# Define default C compiler: CC
#------------------------------------------------------------------------------------------------
CC = gcc

ifeq ($(PLATFORM),PLATFORM_WEB)
    # HTML5 emscripten compiler
    # WARNING: To compile to HTML5, code must be redesigned
    # to use emscripten.h and emscripten_set_main_loop()
    CC = emcc
endif


# Define default make program: MAKE
#------------------------------------------------------------------------------------------------
MAKE ?= make

# Define compiler flags: CFLAGS
#------------------------------------------------------------------------------------------------
#  -O1                  defines optimization level
#  -g                   include debug information on compilation
#  -s                   strip unnecessary data from build
#  -Wall                turns on most, but not all, compiler warnings
#  -std=c99             defines C language mode (standard C from 1999 revision)
#  -std=gnu99           defines C language mode (GNU C from 1999 revision)
#  -Wno-missing-braces  ignore invalid warning (GCC bug 53119)
#  -Wno-unused-value    ignore unused return values of some functions (i.e. fread())
#  -D_DEFAULT_SOURCE    use with -std=c99 on Linux and PLATFORM_WEB, required for timespec
CFLAGS = -Wall -D_DEFAULT_SOURCE -Wno-missing-braces -Wno-unused-value -Wno-pointer-sign $(PROJECT_CUSTOM_FLAGS)
#CFLAGS += -Wextra -Wmissing-prototypes -Wstrict-prototypes

CFLAGS += -std=gnu99

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -D_DEBUG
else
    ifeq ($(BUILD_WEB_ASYNCIFY),TRUE)
        CFLAGS += -O3
    else
        CFLAGS += -Os
    endif
endif

# Define include paths for required headers: INCLUDE_PATHS
#------------------------------------------------------------------------------------------------
INCLUDE_PATHS += -I. -Iexternal -I$(RAYLIB_INCLUDE_PATH) -I../include

# Define library paths containing required libs: LDFLAGS
#------------------------------------------------------------------------------------------------
LDFLAGS = -L. -L$(RAYLIB_LIB_PATH)

ifeq ($(PLATFORM),PLATFORM_WEB)
    # -Os                        # size optimization
    # -O2                        # optimization level 2, if used, also set --memory-init-file 0
    # -sUSE_GLFW=3               # Use glfw3 library (context/input management)
    # -sALLOW_MEMORY_GROWTH=1    # to allow memory resizing -> WARNING: Audio buffers could FAIL!
    # -sTOTAL_MEMORY=16777216    # to specify heap memory size (default = 16MB) (67108864 = 64MB)
    # -sUSE_PTHREADS=1           # multithreading support
    # -sWASM=0                   # disable Web Assembly, emitted by default
    # -sASYNCIFY                 # lets synchronous C/C++ code interact with asynchronous JS
    # -sFORCE_FILESYSTEM=1       # force filesystem to load/save files data
    # -sASSERTIONS=1             # enable runtime checks for common memory allocation errors (-O1 and above turn it off)
    # -sMINIFY_HTML=0            # minify generated html from shell.html
    # --profiling                # include information for code profiling
    # --memory-init-file 0       # to avoid an external memory initialization code file (.mem)
    # --preload-file resources   # specify a resources folder for data compilation
    # --source-map-base          # allow debugging in browser with source map
    # --shell-file shell.html    # define a custom shell .html and output extension
    LDFLAGS += -sUSE_GLFW=3 -sTOTAL_MEMORY=$(BUILD_WEB_HEAP_SIZE) -sSTACK_SIZE=$(BUILD_WEB_STACK_SIZE) -sFORCE_FILESYSTEM=1 -sMINIFY_HTML=0

    # Build using asyncify
    ifeq ($(BUILD_WEB_ASYNCIFY),TRUE)
        LDFLAGS += -sASYNCIFY -sASYNCIFY_STACK_SIZE=$(BUILD_WEB_ASYNCIFY_STACK_SIZE)
    endif

    # Add resources building if required
    ifeq ($(BUILD_WEB_RESOURCES),TRUE)
        LDFLAGS += --preload-file $(BUILD_WEB_RESOURCES_PATH)
    endif

    # Add debug mode flags if required
    ifeq ($(BUILD_MODE),DEBUG)
        LDFLAGS += -sASSERTIONS=1 --profiling
    endif

    # Define a custom shell .html and output extension
    LDFLAGS += --shell-file $(BUILD_WEB_SHELL)
    EXT = .html
endif

# Define libraries required on linking: LDLIBS
# NOTE: To link libraries (lib<name>.so or lib<name>.a), use -l<name>
#------------------------------------------------------------------------------------------------
ifeq ($(PLATFORM),PLATFORM_WEB)
    # Libraries for web (HTML5) compiling
    LDLIBS = $(RAYLIB_LIB_PATH)/libraylib.web.a
endif


# Define all object files from source files
#------------------------------------------------------------------------------------------------
OBJS = $(patsubst %.c,$(PROJECT_BUILD_PATH)/%.o,$(PROJECT_SOURCE_FILES))

# Default target entry
# NOTE: We call this Makefile target or Makefile.Android target
all:
	$(MAKE) $(PROJECT_NAME)

# Link target
$(PROJECT_NAME): $(OBJS)
	$(CC) -o $(PROJECT_BUILD_PATH)/$(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)

# Compile source files into bin directory
$(PROJECT_BUILD_PATH)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

.PHONY: clean_shell_cmd clean_shell_sh

# Clean everything
clean:	clean_shell_$(PLATFORM_SHELL)
	@echo Cleaning done

clean_shell_sh:
ifeq ($(PLATFORM),PLATFORM_WEB)
    ifeq ($(PLATFORM_OS),LINUX)
		rm -fv *.o $(PROJECT_NAME).data $(PROJECT_NAME).html $(PROJECT_NAME).js $(PROJECT_NAME).wasm
    endif
    ifeq ($(PLATFORM_OS),OSX)
		rm -f *.o $(PROJECT_NAME).data $(PROJECT_NAME).html $(PROJECT_NAME).js $(PROJECT_NAME).wasm
    endif
endif

clean_shell_cmd: SHELL=cmd
clean_shell_cmd:
	del *.o *.exe $(PROJECT_NAME).data $(PROJECT_NAME).html $(PROJECT_NAME).js $(PROJECT_NAME).wasm /s