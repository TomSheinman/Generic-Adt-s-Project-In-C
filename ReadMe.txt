General - static functions are used as helper private functions
	code documantion is usually on h files and functions signature and not on the actual function, only exception is in the hashTable beacuse im not responsable for the h file
	get functions don't return a copy of an element it returns the elmenet - also wrote that in the h files. 

HashTable - I've built the hash table based on the previous adts - LinkedList and keyValuePair, the hash table is a list (the table) of linked list where each linked list may hold pairs of key and values
                     the linked list is used for cases of chaning.
                     I've added some functions for KeyValuePair such as copy print and compare in order to create the linked lists in the creation of the hash by sending it the required functions (of keyValuePair) 

MultiValueHashTable - I've built the multiHashTablet also based on the previous adts - LinkedList keyValuePair and also the hashTable, the multiHashTable has hash table as a argument only this time 
		     we can have multiple values for each key, meaning every key has a linked list as its pair and in that list we store the values.
		     thats why i created a copy function for the linked list and sent it along with the some other functions of linkedlist (print, destroy) to the hash table creation as the value functions.
	                       in most functions in this adt i've used functions from the hashTable as they are very similar.

in both this adt's i've used shallow copy for the helper functions i've build for the pair (hashTable) and LinkedList(Multi) and thats beacuse we want to add to the actual pair and not to its copy, same goes for the linklist

JerryBoreeMain - i've used the adt's that i created previously to build the jerries dayCare as such.
	             LinkedList of planets - saves the planets the are in the configuration file. when we want to create a new jerry we search for the planet name in the planets list and if it exists we create the jerry using that planet
	             LinkedList of jerries - used to keep all the jerries in the order that they were added to the system, this is helpful for cases 6,7
	             HashTable of jerries where their id ( char * ) is the key - helps to find a jerry in 0(1) if we receive his id.
	             MultiValueHashTable of jerries - the keys are charasteristics names and the values are jerries that have that charasteristic, helpful when we receive a physical charasteristic and want to find all
	             the jerries with that charasteristic and all their charasteristic in O(number of Jerries with physical characteristic + total number of their physical characteristics) like in cases 2,5,7
	             
	             in alot of parts in the menu helper functions i've used memory failure status as a way to terminate the program, i assume a memory problem accourd beacuse i send to values to the functions
`	             so i know if a null returnd a memory problem occourd
	             the cleanFunc in the main is responsible to print a memory problem accourd and clean the system, exit the run


  			     
