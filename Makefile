MAKEFLAGS += -r

CC = gcc
CFLAGS = -DGL_GLEXT_PROTOTYPES -c
IFLAGS = -I./
LFLAGS = -lglfw -lGL -lm -lpng

SDIR := ./src
ODIR := ./obj
BDIR := ./

CFILES = $(shell find $(SDIR) -name '*.c')
HFILES = $(shell find $(SDIR) -name '*.h')
OFILES = $(CFILES:$(SDIR)/%.c=$(ODIR)/%.o)

BINARY = open_gl_demo

all: $(ODIR) $(BINARY)

debug: CFLAGS += -DDEBUG
debug: $(BINARY)

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c $(SDIR)/%.h
	$(CC) -o $@ $(CFLAGS) $<

install: $(BINARY)
	cp $< ~/.bin/

clean:
	$(RM) $(ODIR)/*.o $(BINARY)

$(BINARY): $(OFILES)
	$(CC) -o $@ $^ $(LFLAGS)

car: $(BINARY)
	./$(BINARY)

.PHONY: all clean
