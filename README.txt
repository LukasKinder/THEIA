complie with gcc *.c -lm -o theia

run with "./theia -f <path>" with <path> being the path to some argumentation frame in trivial graph format 

the -v flag may be used to print the search tree

the -p flag may be used as well but the only valid argument is EE-CO (finding all complete sets).

Example:
./theia -p EE-CO  -f  example_ICCMA2019.tgf 
or
./theia -f example_ICCMA2019.tgf -v


