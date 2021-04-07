# Makefile for clack

# executable name
TARGET = clack

# libraries to link
LIBS =  
LIBFLAGS = $(addprefix -l, $(LIBS))

# general flags to use for linking and compiling
GFLAGS = -std=c99 -Wall -Wextra -pedantic 

CC = gcc
# flags for compiling
CFLAGS = $(GFLAGS)

LINKER   = gcc
# flags for linking
LFLAGS   = $(GFLAGS) $(LIBFLAGS) 

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
