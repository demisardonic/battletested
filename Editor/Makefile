FLAGS = -g -Wall -Werror
LFLAGS = -lncurses

BIN = editor
OBJS = mapeditor.o selection.o util.o

all: $(BIN) none

$(BIN):$(OBJS)
	@echo Linking $@
	@gcc $(FLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

-include $(OBJS:.o=.d)

%.o:%.c
	@echo Compiling $<
	@gcc $(FLAGS) -MMD -MF $*.d -c $<

clean:
	@echo Removing generated file
	@rm -f *.o *.d $(BIN) *~

none:
	@echo Compilation Complete