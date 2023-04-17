#include <stdio.h>
#include <math.h>
#include "LinkedList.h"
#include "HashTable.h"
#include "MultiValueHashTable.h"
#include "Jerry.h"
#define LINE_MAX_LENGTH 300

/** This function is sent to characteristics multiValueHashTable and jerriesList as a free function for its objects.
   the function is a "fake" free because it doesn't really free's the element, we use it as such because we don't have
   copies of jerries in the system so if we will delete a jerry from one adt we will delete it from all adt's
   in our case we will only delete the jerries from the jerriesHash, this function helps us to avoid from freeing
   a jerry more the once a thus causing a segfault or similar error */
status fakeFree(Element elem);

/** This function deepcopy a string - we want to deepcopy so if we delete a jerry and thus its
    characteristics we will not delete the characteristic name which is key in the charaHash
    function is used for the jerries hash and the charaHash as string is a key for these hashes */
Element copyString(Element e);

/// This function frees a string, used for the jerries hash tables and characteristics multiValueHashTable
status freeString(Element e);

/// This function prints a string, used for the jerries hash table and characteristics multiValueHashTable
status printString(Element e);

/** This function compares two strings and return true if they're equal,
   used for the jerries hash table and characteristics multiValueHashTable */
bool eqString(Element elem, Element other);

/** This function converts a string to int using the ascii table,
   used for the jerries hash table and characteristics multiValueHashTable */
int strToInt(Element elem);

/// This function reads from the configuration file and saves the data into the created Adt's to create the jerries daycare system
void readFromFile(FILE * fptr);

/** This function receives a list of chars that represent a planet
    it creates a planet and adds him to the planets list **/
void createPlanetFromFile(char * planetParam[]);

/** This function receives a list of chars that represent a jerry and creates a jerry
   Then the function adds the jerry to the jerries list and the Jerries hash table */
Jerry * createJerryFromFile(char * jerryParam[]);

/** This function receives a list of chars that represent a characteristic and a jerry,
    adds this characteristic to jerry, adds the characteristic and the jerry to the multiValueHashTable */
void addToJerryFromFile(char * charaParam[], Jerry * currJerry);

/// This function cleans the database, meaning frees everything
void cleanFunc();

/** this is a helper function for case 1 in the menu
 * the function tries to add a new jerry to the daycare(adt's) according to parameters received from the user(Rick)
 * if the jerry doesn't exist in the system and received parameters are valid the jerry will be added to the daycare(adt's) and printed afterwords
 * otherwise an appropriate error message will appear and the function will end */
status addToDayCare();

/** this is a helper function for case 2 in the menu
 * the function tries to add a physical characteristic to a jerry in the daycare according to parameters received from the user(Rick)
 * if the jerry (id) is in the system and his characteristic isn't it will be added to the system and all the jerries
 * with that characteristic will be printed, otherwise an appropriate error message will appear and the function will end */
status addCharaToJerry();

/** this is a helper function for case 3 in the menu
 * the function tries to remove a physical characteristic from a jerry in the daycare according to parameters received from the user(Rick)
 * if the jerry (id) and his characteristic is in the system the characteristic will be removed and the characteristics
 * hashTable will be updated accordingly, otherwise an appropriate error message will appear and the function will end */
status removeCharaFromJerry();

/** this is a helper function for case 4 in the menu
 * the function tries to remove a jerry from the daycare according to id received from the user(Rick)
 * if the jerry (id) is in the system he will be deleted and cleaned from the adt's
 * if the id is not in the system an appropriate message will appear and the function will end */
status removeJerryFromDayCare();

/** this is a helper function for case 5 in the menu
 * the function ask from the user(rick) to input a physical characteristic
 * if there is a jerry with that characteristic it asks for a value for it
 * and returns the jerry with the most closest characteristic value to that value
 * after that the jerry will be deleted and cleaned from the adt's */
status findSimilarJerry();

/** this is a helper function for case 6 in the menu
 * the function finds the saddest jerry in the daycare by iterating over the jerrieslist and removes the saddest one */
status removeSaddestJerry();

/** this is a helper function for case 7 in the menu
 * the function asks from the user for a input between 1-3, other input will result in appropriate error message
 * 1 - All Jerries - will print all the jerries in the daycare by their entrance time
 * 2 - All Jerries by physical characteristics - prints all the jerries that hold input characteristic by their entrance time
 * 3- All known planets - prints all the planets in the system by their configuration file order */
void showCaseDayCare();

/** this is a helper function for case 8 in the menu
 * the function asks from the user for a input between 1-3, other input will result in appropriate error message
 * the input determines which play time activity the jerries will take place in
 * each activity changes all jerries happiness levels in different variations according to their current happiness levels
 * after the activity we'll check that all jerries have happiness levels between 0-100
 * if not will adjust them properly (negative will turn to 0, over 100 will turn to 100) */
void jerriesPlay();


/// This helper function receives a jerry and a characteristic he has and return that characteristic value
float findCharaVal(Jerry * jerry, char pc[]);

/** helper function for removing a jerry from the dayCare(adt's)
    first it will remove the jerry from the jerriesList, than from the characteristicsHash by its characteristics
    and finally from the jerriesHash were the jerry will also be freed */
status deleteJerryFromDayCare(Jerry * jerry);

///--global variables--

bool endRun = false; // a global variable that is false if we still need to continue the main and true if were in case 9 and want to end the run
int numOfPlanets; // num of planets in the system as received from config_file
LinkedList planetsList = NULL; // a linked list of planets as elements
LinkedList jerriesList = NULL; // a linkedList of jerries as elements
hashTable jerriesHash = NULL; // hashTable that holds pairs of jerries as values and their id's as keys
mulValHashTable characteristicsHash = NULL; // multiValueHashTable that holds physical characteristics as keys, list of jerries with that physical characteristic

void cleanFunc()
{
    destroyList(planetsList);
    destroyList(jerriesList);
    destroyMultiValueHashTable(characteristicsHash);
    destroyHashTable(jerriesHash);
    if(endRun == false) // if not in case 9 in the main
        exit(1);
}

///--functions for the creation of the adt's--

status fakeFree(Element e)
{
    return success;
}

Element copyString(Element e)
{
    if(e == NULL)
        return NULL;
    char * str1 = (char *)e;
    char * str2 = (char *) malloc(strlen(str1) + 1);
    if(str2 == NULL)
        cleanFunc();
    strcpy(str2, str1);
    return str2;
}

status freeString(Element e)
{
    if(e == NULL)
        return argument_failure;
    free((char *)e);
    return success;
}

status printString(Element e)
{
    if(e==NULL)
        return argument_failure;
    printf("%s : \n", (char *)e);
    return success;
}

bool eqString(Element e, Element other)
{
    if(e == NULL || other == NULL)
        return false;
    if(strcmp((char *)e , (char *)other) == 0)
        return true;
    return false;
}

int strToInt(Element e)
{
    if(e == NULL)
        return 0;
    char * str = (char *)e;
    int sum=0;
    for(int i=0; i<strlen(str); i++)
        sum += (int)str[i];
    return sum;
}

///--functions for the file reading phase--

void createPlanetFromFile(char * planetParameters[])
{
    char *temp;
    double cord_x = strtod(planetParameters[1], &temp);
    double cord_y = strtod(planetParameters[2], &temp);
    double cord_z = strtod(planetParameters[3], &temp);
    Planet * planet = initPlanet(planetParameters[0], cord_x, cord_y, cord_z);
    if(planet == NULL) // problem caused in initializing a planet
        cleanFunc();
    if(appendNode(planetsList, (Element)planet) != success)
        cleanFunc();
}

Jerry * createJerryFromFile(char * jerryParameters[])
{
    char * id = jerryParameters[0];
    char * dim = jerryParameters[1];
    char * planetName = jerryParameters[2];
    int happiness = atoi(jerryParameters[3]);
    Planet * planet = searchByKeyInList(planetsList, planetName);
    Jerry * jerry = initJerry(id, dim, planet, happiness);
    if(jerry == NULL) // problem caused in initializing a jerry
        cleanFunc();
    if(appendNode(jerriesList, (Element)jerry) != success)
        cleanFunc();
    if(addToHashTable(jerriesHash, (Element)jerry->id, (Element)jerry) != success)
        cleanFunc();
    return jerry;
}

void addToJerryFromFile(char * charaParam[], Jerry * currJerry)
{
    char *temp;
    char * name = charaParam[0];
    float value = strtof(charaParam[1], &temp);
    PhysicalCharacteristic * charaToAdd = initChara(name, value);
    if (addChara(currJerry, charaToAdd) == memory_failure) // memory failure in adding to jerry
        cleanFunc();
    if (addToMultiValueHashTable(characteristicsHash, (Element)charaToAdd->name, (Element)currJerry) != success)
        cleanFunc();
}

void readFromFile(FILE * fptr)
{
    int counter;
    char *word;
    char line[LINE_MAX_LENGTH];
    char *planetParam[4];
    char *jerryParam[4];
    char *charaParam[2];
    Jerry * currJerry;
    bool stop = false;
    fgets(line, LINE_MAX_LENGTH, fptr); // skip "Planets"
    for(int i=0; i<numOfPlanets; i++) /// inside the loop - splits the line into the arguments needed for planet
    {
        fgets(line, LINE_MAX_LENGTH, fptr);
        counter = 0;
        word = strtok(line, ",");

        while (word != NULL) {
            planetParam[counter] = word;
            counter++;
            word = strtok(NULL, ",");
        }
        createPlanetFromFile(planetParam);
    }
    fgets(line, LINE_MAX_LENGTH, fptr); // skip "Jerries"
    while(fgets(line, LINE_MAX_LENGTH, fptr) != NULL) // till EOF
    { /// inside the loop : splits the line into the arguments needed for Jerry
        while (line[0] == '\t') // jerry has a physical characteristic
        {
            word = strtok(line+1, ":");
            charaParam[0] = word;
            word = strtok(NULL, ":");
            charaParam[1] = word;
            addToJerryFromFile(charaParam, currJerry);
            if (fgets(line, LINE_MAX_LENGTH, fptr) == NULL) // eof
            {
                stop = true;
                break;
            }
        }
        if(stop == true)
            break;
        counter = 0;
        word = strtok(line, ",");
        while (word != NULL)
        {
            jerryParam[counter] = word;
            counter++;
            word = strtok(NULL, ",");
        }
        currJerry = createJerryFromFile(jerryParam);
    }
    fclose(fptr);
}

///--helper functions for the menu--

status addToDayCare()
{
    char inputId[300];
    char planetName[300];
    char dim[300];
    int happiness;
    printf("What is your Jerry's ID ? \n");
    scanf("%s", inputId);
    Element jElem = lookupInHashTable(jerriesHash, inputId);
    if(jElem != NULL) // ID is already in the system
    {
        printf("Rick did you forgot ? you already left him here ! \n");
        return failure;
    }
    printf("What planet is your Jerry from ? \n");
    scanf("%s", planetName);
    Element planet = searchByKeyInList(planetsList, planetName);
    if (planet == NULL) // Planet received doesn't exist
    {
        printf("%s is not a known planet ! \n", planetName);
        return failure;
    }
    printf("What is your Jerry's dimension ? \n");
    scanf("%s", dim);
    printf("How happy is your Jerry now ? \n");
    scanf("%d", &happiness);
    Jerry * jerry = initJerry(inputId, dim, (Planet *)planet, happiness);
    if(jerry == NULL) // error in creation of jerry
        return memory_failure; /// not necessarily a memory failure but I want to end the run and clean the system if creation or one of the adding failed
    if (appendNode(jerriesList, (Element)jerry) != success)
        return memory_failure;
    if (addToHashTable(jerriesHash, (Element) inputId, (Element)jerry) != success)
        return memory_failure;
    printJerry((Element)jerry);
    return success;
}

status addCharaToJerry()
{
    char inputId[300];
    char inputChara[300];
    float charaValue;
    printf("What is your Jerry's ID ? \n");
    scanf("%s", inputId);
    Jerry * jerry = (Jerry *)lookupInHashTable(jerriesHash, inputId);
    if(jerry == NULL) // ID is not in the system
    {
        printf("Rick this Jerry is not in the daycare ! \n");
        return failure;
    }
    printf("What physical characteristic can you add to Jerry - %s ? \n", inputId);
    scanf("%s", inputChara);
    if (checkCharaInJerry(*jerry, inputChara) == true) // jerry already has this characteristic
    {
        printf("The information about his %s already available to the daycare ! \n", inputChara);
        return failure;
    }
    printf("What is the value of his %s ? \n", inputChara);
    scanf("%f", &charaValue);
    PhysicalCharacteristic * charaToAdd = initChara(inputChara, charaValue);
    if(charaToAdd == NULL)
        return memory_failure; /// same behavior as last function - if failed to create a chara / adding, clean and end the run
    if (addChara(jerry, charaToAdd) != success)
        return memory_failure;
    if(addToMultiValueHashTable(characteristicsHash, (Element)inputChara, (Element)jerry) != success)
        return memory_failure;
    displayMultiValueHashElementsByKey(characteristicsHash, inputChara);
    return success;
}

status removeCharaFromJerry()
{
    char inputId[300];
    char inputChara[300];
    printf("What is your Jerry's ID ? \n");
    scanf("%s", inputId);
    Jerry * jerry = (Jerry *)lookupInHashTable(jerriesHash, (Element)inputId);
    if(jerry == NULL) // ID is not in the system
    {
        printf("Rick this Jerry is not in the daycare ! \n");
        return failure;
    }
    printf("What physical characteristic do you want to remove from Jerry - %s ? \n", inputId);
    scanf("%s", inputChara);
    if (checkCharaInJerry(* jerry, inputChara) == false) // jerry doesn't have characteristic to remove
    {
        printf("The information about his %s not available to the daycare ! \n", inputChara);
        return failure;
    }
    /// removing characteristic from jerry and the jerry from characteristics hashTable
    if(removeFromMultiValueHashTable(characteristicsHash, inputChara, jerry) != success)
        return memory_failure; // same behavior as last function
    if(deleteChara(jerry, inputChara) != success)
        return memory_failure;
    printJerry(jerry);
    return success;
}

status removeJerryFromDayCare()
{
    char inputId[300];
    printf("What is your Jerry's ID ? \n");
    scanf("%s", inputId);
    Jerry * jerry = (Jerry *)lookupInHashTable(jerriesHash, (Element)inputId);
    if(jerry == NULL) // ID is not in the system
    {
        printf("Rick this Jerry is not in the daycare ! \n");
        return failure;
    }
    return deleteJerryFromDayCare(jerry);
}

status findSimilarJerry()
{
    char inputChara[300];
    float charaValue;
    Jerry * currJerry, * closestJerry; //closetJerry will be the jerry who has characteristic is the closest to the input
    double minDiff = INFINITY; // defines the closest characteristic value to the input so far
    printf("What do you remember about your Jerry ? \n");
    scanf("%s", inputChara);
    LinkedList jerriesWithChara = lookupInMultiValueHashTable(characteristicsHash, (Element)inputChara);
    if(jerriesWithChara== NULL) // No jerry in the daycare has this characteristics
    {
        printf("Rick we can not help you - we do not know any Jerry's %s ! \n", inputChara);
        return failure;
    }
    printf("What do you remember about the value of his %s ? \n", inputChara);
    scanf("%f", &charaValue);
    /** finds the jerry with the closest characteristic value to the input using abs function
     we iterate over all the jerries with the input characteristic and update (if needed) who is the closest jerry so far
     to the input value */
    for(int i=1; i<=getLengthList(jerriesWithChara); i++)
    {
        currJerry = (Jerry *)(getDataByIndex(jerriesWithChara, i));
        double diff = fabs((double)(findCharaVal(currJerry, inputChara) - charaValue));
        if (diff < minDiff)
        {
            minDiff = diff;
            closestJerry = currJerry;
        }
    }
    printf("Rick this is the most suitable Jerry we found : \n");
    printJerry(closestJerry);
    return deleteJerryFromDayCare(closestJerry);
}

status removeSaddestJerry()
{
    int minHappiness = 101; // happiness of the saddest jerry so far (init with value higher than possible for a jerry)
    int numOfJerries = getLengthList(jerriesList);
    Jerry * currJerry, * saddestJerry; // saddestJerry points to the saddestJerry in the daycare so far
    if(numOfJerries == 0) // no jerries in the daycare
    {
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
        return failure;
    }
    for(int i=1; i<=numOfJerries; i++)
    {
        currJerry = (Jerry *)getDataByIndex(jerriesList, i);
        if(currJerry->happiness < minHappiness)
        {
            minHappiness = currJerry->happiness;
            saddestJerry = currJerry;
        }
    }
    printf("Rick this is the most suitable Jerry we found : \n");
    printJerry(saddestJerry);
    return deleteJerryFromDayCare(saddestJerry);
}

void showCaseDayCare()
{
    char input[300];
    printf("What information do you want to know ? \n");
    printf("1 : All Jerries \n");
    printf("2 : All Jerries by physical characteristics \n");
    printf("3 : All known planets \n");
    scanf("%s", input);
    if(strlen(input) > 1)
    {
        printf("Rick this option is not known to the daycare ! \n");
        return;
    }
    int inputVal = atoi(input);
    switch (inputVal)
    {
        case 1: /// All Jerries
            if(getLengthList(jerriesList) == 0)
                printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
            else
                displayList(jerriesList);
            return;

        case 2: /// All Jerries by physical characteristics
            printf("What physical characteristics ? \n");
            char inputChara[300];
            scanf("%s", inputChara);
            if(displayMultiValueHashElementsByKey(characteristicsHash, inputChara) != success)
                printf("Rick we can not help you - we do not know any Jerry's %s ! \n", inputChara);
            return;

        case 3: /// All known planets
            displayList(planetsList);
            return;

        default: /// input is not 1-3
        {
            printf("Rick this option is not known to the daycare ! \n");
            return;
        }
    }
}

void jerriesPlay()
{
    char input[300];
    Jerry * currJerry;
    printf("What activity do you want the Jerries to partake in ? \n");
    printf("1 : Interact with fake Beth \n");
    printf("2 : Play golf \n");
    printf("3 : Adjust the picture settings on the TV \n");
    scanf("%s", input);
    if(strlen(input) > 1)
    {
        printf("Rick this option is not known to the daycare ! \n");
        return;
    }
    int inputVal = atoi(input);
    switch (inputVal)
    {
        case 1: /// Interact with fake Beth
            for (int i = 1; i <= getLengthList(jerriesList); i++)
            {
                currJerry = (Jerry *)getDataByIndex(jerriesList, i);
                if(currJerry->happiness >= 20)
                    currJerry->happiness += 15;
                else
                    currJerry->happiness -= 5;
            }
            break;

        case 2: /// Play golf
            for (int i = 1; i <= getLengthList(jerriesList); i++)
            {
                currJerry = (Jerry *)getDataByIndex(jerriesList, i);
                if(currJerry->happiness >= 50)
                    currJerry->happiness += 10;
                else
                    currJerry->happiness -= 10;
            }
            break;

        case 3: /// Adjust the picture settings on the TV
            for (int i = 1; i <= getLengthList(jerriesList); i++)
            {
                currJerry = (Jerry *) getDataByIndex(jerriesList, i);
                currJerry->happiness += 20;
            }
            break;

        default:
            printf("Rick this option is not known to the daycare ! \n");
            return;
    }
    for (int i = 1; i <= getLengthList(jerriesList); i++) /// check jerries happiness level are in range (0-100)
    {
        currJerry = (Jerry *) getDataByIndex(jerriesList, i);
        if (currJerry->happiness > 100)
            currJerry->happiness = 100;
        if (currJerry->happiness < 0)
            currJerry->happiness = 0;
    }
    printf("The activity is now over ! \n");
    displayList(jerriesList);
}


///--helper functions for the helper functions of th menu--

float findCharaVal(Jerry *jerry, char pc[])
{
    for(int i=0; i<jerry->num_of_chars; i++)
    {
        if(strcmp(jerry->phy_char[i]->name, pc) == 0)
            return jerry->phy_char[i]->value;
    }
    return -1;
}

status deleteJerryFromDayCare(Jerry * jerry)
{
    for (int i=0; i<jerry->num_of_chars; i++) // deleting jerry from characteristicsHash by going through all his characteristics
    {
        if(removeFromMultiValueHashTable(characteristicsHash, (Element)jerry->phy_char[i]->name, (Element)jerry) != success)
            return memory_failure; // same behavior as previous functions
    }
    if(deleteNode(jerriesList, (Element)jerry) != success)
        return memory_failure; // not necessarily a memory failure but I want to end the run and clean the system if any of the deletions fails
    // this will also free jerry if successful - previous deletion used fakeFree so jerry wasn't really freed just deleted from the adt's
    if (removeFromHashTable(jerriesHash, (Element)jerry->id) != success)
        return memory_failure;
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
    return success;
}

int main(int argc, char **argv)
{
    numOfPlanets = atoi(argv[1]);
    /** -- create the adt's -- **/
    planetsList = createLinkedList(copyPlanet, destroyPlanet, printPlanet, eqPlanet, nameInPlanet);
    jerriesList = createLinkedList(copyJerry, fakeFree, printJerry, eqJerry, idInJerry);
    jerriesHash = createHashTable(copyString, freeString, printString, copyJerry, destroyJerry, printJerry,eqString, strToInt, 3 * numOfPlanets);
    characteristicsHash = createMultiValueHashTable(copyString, freeString, printString, copyJerry, fakeFree, printJerry , eqString, eqJerry,  strToInt, 6 * numOfPlanets);
    if(planetsList == NULL || jerriesList == NULL || jerriesHash == NULL || characteristicsHash == NULL)
        cleanFunc();
    FILE *fptr = fopen(argv[2], "r");
    if(fptr == NULL)
    {
        printf("File Problem \n");
        cleanFunc();
    }
    readFromFile(fptr);
    /** -- menu -- **/
    int inputVal;
    char input[300];
    while (endRun == false) // stops at 9
    {
        printf("Welcome Rick, what are your Jerry's needs today ? \n");
        printf("1 : Take this Jerry away from me \n");
        printf("2 : I think I remember something about my Jerry \n");
        printf("3 : Oh wait. That can't be right \n");
        printf("4 : I guess I will take back my Jerry now \n");
        printf("5 : I can't find my Jerry. Just give me a similar one \n");
        printf("6 : I lost a bet. Give me your saddest Jerry \n");
        printf("7 : Show me what you got \n");
        printf("8 : Let the Jerries play \n");
        printf("9 : I had enough. Close this place \n");
        scanf("%s", input);
        if (strlen(input) > 1) // invalid input - should be only one char (0-9)
        {
            printf("Rick this option is not known to the daycare ! \n");
            continue;
        }
        inputVal = atoi(input);
        switch (inputVal)
        {
            case 1: /// Take this Jerry away from me

                if(addToDayCare() == memory_failure)
                    cleanFunc();
                break;

            case 2: /// I think I remember something about my Jerry

                if(addCharaToJerry() == memory_failure)
                    cleanFunc();
                break;

            case 3: /// Oh wait. That can't be right

                if(removeCharaFromJerry() == memory_failure)
                    cleanFunc();
                break;

            case 4: /// I guess I will take back my Jerry now

                if(removeJerryFromDayCare() == memory_failure)
                    cleanFunc();
                break;

            case 5: /// I can't find my Jerry. Just give me a similar one

                if(findSimilarJerry() == memory_failure)
                    cleanFunc();
                break;

            case 6: /// I lost a bet. Give me your saddest Jerry
                if (removeSaddestJerry() == memory_failure)
                    cleanFunc();
                break;

            case 7:
                showCaseDayCare();
                break;

            case 8: /// Let the Jerries play
                if(getLengthList(jerriesList) == 0)
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                else
                    jerriesPlay();
                break;

            case 9: /// I had enough. Close this place
                endRun = true;
                cleanFunc();
                printf("The daycare is now clean and close ! \n");
                break;

            default:
                printf("Rick this option is not known to the daycare ! \n");
                break;
        }
    }
}



