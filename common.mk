OUT = bin

SEARCHPATH += src
vpath %.c $(SEARCHPATH)
vpath %.h $(SEARCHPATH)

DEPS += defs.h structs.h
_OBJS += background.o
_OBJS += draw.o 
_OBJS += init.o input.o
_OBJS += highscore.o
_OBJS += utility.o
_OBJS += main.o
_OBJS += title.o
_OBJS += gamestage.o
_OBJS += sound.o
_OBJS += text.o

OBJS = $(patsubst %,$(OUT)/%,$(_OBJS))

# top-level rule to create the program.
all: $(PROG)

# compiling other source files
$(OUT)/%.o:	%.c %.h $(DEPS)
	@mkdir -p $(OUT)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c -o $@ $<

# clean up everthing can be automatically recreated
clean:
	$(RM) -rf $(OUT) ($PROG)