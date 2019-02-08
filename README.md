# TpReseaux4
temporary repository for a network application

## Compilation
`make` <br/>
`make clean`

## Execution
`cd bin` <br/>
`./client [-t ipAdress] [-p port or protocol]` <br/>
`./serveur [-p port or protocol]`

### Example
`cd bin` <br/>
`./client` or `./client -t 192.0.1.1 -p udp` <br/>
`./serveur -p udp`

## Other
Si probleme lors de l'execution avec ucblib rajouter: `setenv LD_LIBRARY /usr/ucblib/`

Essaicurses et le make associe `make essaicurse` permettent de comprendre les primitives de gestions de curseur dans plusieurs fenetres. Permet de faire un joli TALK.
