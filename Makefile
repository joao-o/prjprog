W_FLAGS    = -Wall #-pedantic
CFLAGS_GTK = `pkg-config --cflags glib-2.0` `pkg-config --cflags gtk+-2.0`
DB_FLAGS   = -ggdb -g
OPT_FLAGS  = -O3

INC_FLAGS  = -I./include
GTK_LINK   = `pkg-config --libs glib-2.0` `pkg-config --libs gtk+-2.0`
LINK_LIBS  = $(GTK_LINK) -lm
LINK_FLAGS =

CFLAGS     = $(W_FLAGS) $(INC_FLAGS)  $(CFLAGS_GTK) 
LFLAGS     = $(W_FLAGS) $(LINK_FLAGS) $(LINK_LIBS) 

CC = gcc
RM = /bin/rm

TARGET = proj
INCLUDE =
PROGS  = proj.c callbacks.c prjcairo.c phys.c cairofunc.c miscui.c
OBJECT = proj.o callbacks.o prjcairo.o phys.o cairofunc.o miscui.o
TESTE = cairotest.c

all: comp link

optimus:CFLAGS += $(OPT_FLAGS)
optimus:LFLAGS += $(OPT_FLAGS)
optimus: all

debug: CFLAGS += $(DB_FLAGS)
debug: LFLAGS += $(DB_FLAGS)	
debug: all

edit:
	vim Makefile

link: $(TARGET)

comp: $(OBJECT)

proj.o: proj.c
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS)

callbacks.o: callbacks.c
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS)

prjcairo.o: prjcairo.c
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS)

phys.o: phys.c
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS)

miscui.o: miscui.c
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS)

cairofunc.o: cairofunc.c
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS)

$(TARGET): $(OBJECT)
	$(CC) $(LFLAGS) -o $@ $^ $(LINK_LIBS)

clean: 
	$(RM) -f *~ *.o $(TARGET) 
