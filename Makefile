CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/battle
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -Werror
LIB := -lncurses
INC := -I include

$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)"
	@$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "Compiling $<"
	@touch $@
	@$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo Removing generated file
	@$(RM) -r $(BUILDDIR) $(TARGET)
  
.PHONY: clean