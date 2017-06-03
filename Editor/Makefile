FLAGS = -g -Wall -Werror
LFLAGS = -lncurses

BIN = editor
OBJS = mapedit.o model.o selection.o util.o ui.o 

all: $(BIN) none

$(BIN):$(OBJS)
	@echo Linking $@
	@gcc $(FLAGS) $(OBJS) -o bin/$(BIN) $(LFLAGS)

-include $(OBJS:.o=.d)

%.o:%.c
	@echo Compiling $<
	@gcc $(FLAGS) -MMD -MF $*.d -c $<
	
pcedit: none
	@echo Compiling pcedit
	@gcc $(FLAGS) pcedit.c -o bin/pcedit $(LFLAGS)

clean:
	@echo Removing generated file
	@rm -f *.o *.d bin/* *~

none:
	@echo Compilation Complete