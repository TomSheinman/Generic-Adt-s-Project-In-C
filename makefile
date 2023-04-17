all: JerryBoree

JerryBoree: LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o Jerry.o
	gcc LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o Jerry.o -o JerryBoree

Jerry.o: Jerry.c Jerry.h Defs.h
	gcc -c Jerry.c

LinkedList.o: LinkedList.c LinkedList.h Defs.h
	gcc -c LinkedList.c

KeyValuePair.o : KeyValuePair.c KeyValuePair.h Defs.h
	gcc -c KeyValuePair.c

HashTable.o : HashTable.c HashTable.h Defs.h KeyValuePair.c KeyValuePair.h LinkedList.c LinkedList.h
	gcc -c HashTable.c

MultiValueHashTable.o : MultiValueHashTable.c MultiValueHashTable.h HashTable.c HashTable.h Defs.h KeyValuePair.c KeyValuePair.h LinkedList.c LinkedList.h
	gcc -c MultiValueHashTable.c

JerryBoreeMain.o : JerryBoreeMain.c MultiValueHashTable.c MultiValueHashTable.h HashTable.c HashTable.h Defs.h KeyValuePair.c KeyValuePair.h LinkedList.c LinkedList.h Jerry.h
	gcc -c JerryBoreeMain.c
clean : 
	@echo 'cleaning'
	rm –f *.o

