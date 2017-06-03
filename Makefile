FLAGS = -g -Wall -Werror
LFLAGS = -lncurses

BIN = battle
OBJS = battle.o character.o logger.o model.o util.o ui.o

all: mkdir $(BIN) none

$(BIN):$(OBJS)
	@echo Linking $@
	@gcc $(FLAGS) $(OBJS) -o bin/$(BIN) $(LFLAGS)

-include $(OBJS:.o=.d)

%.o:%.c
	@echo Compiling $<
	@gcc $(FLAGS) -MMD -MF $*.d -c $<
	
mkdir:
	@echo Making bin directory
	@mkdir -p bin log
	
clean:
	@echo Removing generated file
	@rm -f *.o *.d *~ $(BIN) btlog
	@rm -f -r bin log

none:
	@echo Compilation Complete