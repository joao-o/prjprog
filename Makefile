W_FLAGS    = -Wall -std=c99 #-pedantic
CFLAGS_GTK = `pkg-config --cflags glib-2.0` `pkg-config --cflags gtk+-2.0`
DB_FLAGS   = -ggdb -g

INC_FLAGS  =
GTK_LINK   = `pkg-config --libs glib-2.0` `pkg-config --libs gtk+-2.0`
LINK_LIBS  = $(GTK_LINK) -lm
LINK_FLAGS =

CFLAGS     = $(W_FLAGS) $(INC_FLAGS)  $(CFLAGS_GTK) 
LFLAGS     = $(W_FLAGS) $(LINK_FLAGS) # $(LINK_LIBS) 

CC = gcc
RM = /bin/rm

TARGET = proj
INCLUDE =
PROGS  = proj.c
OBJECT = proj.o

all: comp link

debug: CFLAGS += $(DBG_FLAGS)
debug: LFLAGS += $(DBG_FLAGS)	
debug: all

link: $(TARGET)

comp: $(OBJECT)

proj.o: proj.c
	$(CC) $(CFLAGS) -c $<

proj: $(OBJECT)
	$(CC) $(LFLAGS) -o $@ $^ $(LINK_LIBS)

clean: 
	$(RM) -f *~ *.o $(PROGS)  
