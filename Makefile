W_FLAGS    = -Wall
CFLAGS_GTK = `pkg-config --cflags glib-2.0` `pkg-config --cflags gtk+-2.0`
DB_FLAGS   = -ggdb -g
OPT_FLAGS  = -O3

INC_FLAGS  = -I./include
GTK_LINK   = `pkg-config --libs glib-2.0` `pkg-config --libs gtk+-2.0`
LINK_LIBS  = $(GTK_LINK) -lm
LINK_FLAGS =

CFLAGS     = $(W_FLAGS) $(INC_FLAGS)  $(CFLAGS_GTK)
LFLAGS     = $(W_FLAGS) $(LINK_FLAGS) $(LINK_LIBS)

CC =  gcc
RM = /bin/rm

TARGET = galileo
PROGS  = proj.c callbacks.c miscui.c draw.c
OBJECT = proj.o callbacks.o miscui.o draw.o
TESTE = cairotest.c

all: comp link 

optimus:CFLAGS += $(OPT_FLAGS)
optimus:LFLAGS += $(OPT_FLAGS)
optimus: all

debug: CFLAGS += $(DB_FLAGS)
debug: LFLAGS += $(DB_FLAGS)
debug: all

link: $(TARGET)

comp: $(OBJECT)

proj.o: proj.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS)

callbacks.o: callbacks.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS)

draw.o: draw.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS)

miscui.o: miscui.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS)

$(TARGET): $(OBJECT) $(INCLUDE)
	$(CC) $(LFLAGS) -o $@ $^ $(LINK_LIBS)

pdf: doc/Projtexshort.tex doc/instruc.tex
	pdflatex  --output-directory doc/ doc/Projtexshort.tex
	pdflatex  --output-directory doc/ doc/instruc.tex

edit:
	vim Makefile

count:
	wc -l *.c

clean:
	$(RM) -f *~ *.o $(TARGET) test include/*~   
