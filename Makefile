FLAGS = -g -Wall -Werror
LFLAGS = -lncurses

BIN = battle
OBJS = main.o logger.o util.o



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
	@rm -f *.o *.d $(BIN) *~ editor

none:
	@echo Compilation Complete

editor:
	@echo Compiling Map Editor
	@gcc $(FLAGS) mapeditor.c -o editor $(LFLAGS)
	@echo Compiling Complete