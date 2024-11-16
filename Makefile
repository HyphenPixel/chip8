#
# 'make'        build executable file 'main' without DEBUG
# 'make debug'  build executable file 'main' with DEBUG
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = gcc

# define common compile-time flags
CFLAGS := -Wall -Wextra -g

# define library paths in addition to /usr/lib
LFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# define output directory
OUTPUT := output

# define source directory
SRC := src

# define include directory
INCLUDE := include

# define lib directory
LIB := lib

ifeq ($(OS),Windows_NT)
MAIN := main.exe
SOURCEDIRS := $(SRC)
INCLUDEDIRS := $(INCLUDE)
LIBDIRS := $(LIB)
FIXPATH = $(subst /,\\,$1)
RM := del /q /f
MD := mkdir
else
MAIN := main
SOURCEDIRS := $(shell find $(SRC) -type d)
INCLUDEDIRS := $(shell find $(INCLUDE) -type d)
LIBDIRS := $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD := mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES := $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS := $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
SOURCES := $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

# define the C object files 
OBJECTS := $(SOURCES:.c=.o)

# define the dependency output files
DEPS := $(OBJECTS:.o=.d)

OUTPUTMAIN := $(call FIXPATH,$(OUTPUT)/$(MAIN))

# Default target: Build without DEBUG
all: $(OUTPUT) $(MAIN)
	@echo Build complete without DEBUG!

# Build with DEBUG flag
debug: CFLAGS += -DDEBUG
debug: $(OUTPUT) $(MAIN)
	@echo Build complete with DEBUG!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

# include all .d files
-include $(DEPS)

# rule for building .o and .d files from .c
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -MMD $< -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run' complete!
