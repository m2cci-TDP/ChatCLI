# ChatCLI
Chat in command line with a server and several clients <br/>

**NOT WORKING** <br/>
**ONLY THE LAST CLIENT CAN SPEAK TO OTHER**

## Compilation
`make` <br/>
`make clean`

## Execution
`cd bin` <br/>
`./chat -h` for help <br/>
`./chat -c [-t IP adress of the server] [-p port]` for the client <br/>
`./chat -s [-p port]` for the server

### Example
`cd bin` <br/>
`./chat -c` or `./chat -c -t 192.0.1.1 -p 1111` <br/>
`./chat -s -p 1111`

<!--
## Other
Si probleme lors de l'execution avec ucblib rajouter: `setenv LD_LIBRARY /usr/ucblib/`

Essaicurses et le make associe `make essaicurse` permettent de comprendre les primitives de gestions de curseur dans plusieurs fenetres. Permet de faire un joli TALK.
-->
