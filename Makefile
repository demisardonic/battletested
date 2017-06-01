FLAGS = -g -Wall -Werror
LFLAGS = -lncurses

BIN = battle
OBJS = battle.o character.o logger.o model.o util.o ui.o

all: $(BIN) none

editor: $(EBIN) mon

$(BIN):$(OBJS)
	@echo Linking $@
	@gcc $(FLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

-include $(OBJS:.o=.d)

%.o:%.c
	@echo Compiling $<
	@gcc $(FLAGS) -MMD -MF $*.d -c $<

clean:
	@echo Removing generated file
	@rm -f *.o *.d $(BIN) $(EBIN) *~

none:
	@echo Compilation Complete