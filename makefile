
dirSRC = src
OBJ = $(dirSRC)/fon.o
objClient = $(dirSRC)/client.o
objServeur = $(dirSRC)/serveur.o
OPTIONS	= -lcurses
CFLAGS = -Iinclude
CC = gcc
client = client
serveur = serveur
dirBIN = bin

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

all: $(client) $(serveur) install

$(dirSRC)/%.o: %.c
	$(CC) -DDEBUG $(CFLAGS) -c $<

$(client): $(OBJ) $(objClient)
	$(CC) $(LFLAGS) -o $@ $^ $(OPTIONS)

$(serveur): $(OBJ) $(objServeur)
	$(CC) $(LFLAGS) -o $@ $^ $(OPTIONS)

install:
	mkdir -p $(dirBIN)
	mv $(OBJ) $(objClient) $(objServeur) $(dirBIN)/

clean:
	rm -f $(dirBIN)/*.o
	rm -r $(dirBIN)

mrproper:
		rm -f $(client) $(serveur) core
