CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/battle

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -Werror -std=c++11
LIB := -lncurses
INC := -I include

$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)"
	@mkdir -p $(shell dirname "$(TARGET)")
	@$(CC) $^ -o $(TARGET) $(LIB)
	@mkdir -p log

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "Compiling $<"
	@mkdir -p $(shell dirname "$@")
	@$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo Removing generated file
	@$(RM) -rf $(BUILDDIR) $(TARGET)

.PHONY: clean
