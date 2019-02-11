.PHONY: test

dirSRC = src
OBJ = $(dirSRC)/fon.o $(dirSRC)/util.o $(dirSRC)/client.o $(dirSRC)/serveur.o
objChat = $(dirSRC)/chat.o
OPTIONS	= -lcurses
CFLAGS = -Iinclude
CC = gcc
dirBIN = bin
dirLIB = lib

appli = essaicurse
srcAppli = $(dirSRC)/EssaiCurses.c

# Adaptation a Darwin / MacOS X avec fink
# Du fait de l'absence de libtermcap on se fait pas mal
# cracher dessus mais ca marche...
ifeq ($(shell uname), Darwin)
LFLAGS += -L/opt/local/lib
CFLAGS += -I/opt/local/include
endif
#Changer si necessaire le chemin d'acces aux librairies

# Adaptation a Linux
ifeq ($(shell uname), Linux)
OPTIONS	+= -ltermcap
endif

# Adaptation a FreeBSD
# Attention : il faut utiliser gmake...
ifeq ($(shell uname),FreeBSD)
OPTIONS	+= -ltermcap
endif

# Adaptation a Solaris
ifeq ($(shell uname),SunOS)
OPTIONS	+= -ltermcap  -lsocket -lnsl
CFLAGS	+= -I..
endif

all: chat install

$(dirSRC)/%.o: %.c
	$(CC) -DDEBUG $(CFLAGS) -c $<

chat: $(OBJ) $(objChat)
	$(CC) $(LFLAGS) -o $@ $^ $(OPTIONS)

install:
	rm -rf $(dirBIN) $(dirLIB)
	mkdir -p $(dirBIN) $(dirLIB)
	mv $(OBJ) $(objChat) $(dirLIB)/
	mv chat $(dirBIN)/

$(appli):
	$(CC) $(srcAppli) -o $@ -L/usr/ccs/lib -L/usr/ucblib $(OPTIONS)
	mkdir -p $(dirLIB)
	mv $(appli) $(dirBIN)/

test:
	make -C test
	./test/check
	make -C test clean

clean:
	rm -f $(dirLIB)/*.o
	rm -r $(dirLIB)

mrproper:
	rm -f $(dirBIN)/$(appli)
	rm -r $(dirBIN)
