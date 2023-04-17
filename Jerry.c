#include "Jerry.h"

/// this function is used to free up a Jerry PhysicalCharacteristic array by using the DestroyChara func
static void destroyJerryChara(Jerry *);
/** helper function that returns the position of a physical characteristic in jerry array
  returns -1 if he doesn't have it */
static int positionOfChara(Jerry, char []);

/** --- init functions */

Jerry * initJerry(char id[], char dim_name[], Planet *planet, int happiness)
{
    if(planet == NULL || dim_name == NULL || id == NULL)
        return NULL;
    Jerry *jerry = (Jerry *) malloc(sizeof(Jerry));
    if(jerry == NULL)
    {
        printf("Memory Problem");
        return NULL;
    }
    jerry->id = (char *)malloc(strlen(id) + 1);
    if(jerry->id == NULL)
    {
        destroyJerry(jerry); // already allocated a memory for jerry
        printf("Memory Problem");
        return NULL;
    }
    strcpy(jerry->id, id);
    jerry->happiness = happiness;
    jerry->origin = initOrigin(planet, dim_name);
    if (jerry->origin == NULL)
        return NULL;
    jerry->phy_char = NULL;
    jerry->num_of_chars = 0;
    return jerry;
}

Planet * initPlanet(char name[], double cord_x, double cord_y, double cord_z)
{
    if(name == NULL)
        return NULL;
    Planet *planet = (Planet *) malloc(sizeof(Planet));
    if(planet == NULL)
    {
        printf("Memory Problem \n");
        return NULL;
    }
    planet->name = (char *) malloc(strlen(name) + 1);
    if(planet-> name == NULL)
    {
        destroyPlanet(planet); // already allocated a memory for planet
        printf("Memory Problem \n");
        return NULL;
    }
    strcpy(planet->name,name);
    planet->cord_x = cord_x;
    planet->cord_y=cord_y;
    planet->cord_z=cord_z;
    return planet;
}


PhysicalCharacteristic * initChara(char name[], float value)
{
    if (name == NULL)
        return NULL;
    PhysicalCharacteristic * phy_char = (PhysicalCharacteristic *) malloc(sizeof(PhysicalCharacteristic));
    if(phy_char == NULL)
    {
        printf("Memory Problem \n");
        return NULL;
    }
    phy_char->name = (char *) malloc(strlen(name) + 1);
    if(phy_char->name == NULL)
    {
        destroyChara(phy_char); // already allocated a memory for phy_char
        printf("Memory Problem \n");
        return NULL;
    }
    strcpy(phy_char->name, name);
    phy_char->value = value;
    return phy_char;
}

Origin * initOrigin(Planet *planet, char name[])
{
    Origin * origin = (Origin *) malloc(sizeof(Origin));
    origin->dim_name = (char *) malloc(strlen(name) + 1);
    if(origin == NULL)
    {
        printf("Memory Problem \n");
        return NULL;
    }
    if(origin->dim_name == NULL)
    {
        destroyOrigin(origin); // already allocated a memory for origin
        printf("Memory Problem \n");
        return NULL;
    }
    strcpy(origin->dim_name, name);
    origin->planet = *planet;
    return origin;
}

/** -- characteristic functions */

bool checkCharaInJerry(Jerry jerry, char phy_char[])
{
    if(positionOfChara(jerry, phy_char) == -1)
        return false;
    else
        return true;
}

static int positionOfChara(Jerry jerry, char  phy_char[])
{
    for(int i=0; i<jerry.num_of_chars; i++)
    {
        if(strcmp(jerry.phy_char[i]->name, phy_char) == 0)
            return i;
    }
    return -1;
}

status addChara(Jerry * jerry, PhysicalCharacteristic * phy_char)
{
    if(jerry == NULL || phy_char == NULL)
        return argument_failure;
    if(checkCharaInJerry(*jerry, phy_char->name) == false)
    {
        jerry->num_of_chars++;
        jerry->phy_char = (PhysicalCharacteristic **)realloc(jerry->phy_char, sizeof(PhysicalCharacteristic *)* jerry->num_of_chars);
        if (jerry->phy_char == NULL)
        {
            printf("Memory Problem");
            return memory_failure;
        }
        jerry->phy_char[jerry->num_of_chars - 1] = phy_char;
        return success;
    }
    return failure;
}

status deleteChara(Jerry *jerry, char phy_char[])
{
    if (jerry == NULL || phy_char == NULL)
        return argument_failure;
    int chara_position = positionOfChara(*jerry, phy_char);
    if (chara_position != -1) // has this Characteristic
    {
        PhysicalCharacteristic * temp = jerry->phy_char[chara_position]; //save for freeing up later
        int new_size = jerry-> num_of_chars - 1;
        for (int i = chara_position; i < new_size; i++)
            jerry->phy_char[i] = jerry->phy_char[i+1]; // shift characteristics left;
        jerry->phy_char = realloc(jerry->phy_char, new_size * sizeof(PhysicalCharacteristic *));
        if (jerry->phy_char == NULL && jerry->num_of_chars >1)
        {
            printf("Memory Problem");
            return memory_failure;
        }
        destroyChara(temp);
        jerry->num_of_chars--;
        return success;
    }
    return failure;
}

/** destruction functions */

status destroyPlanet(Element elem)
{
    if(elem == NULL)
        return argument_failure;
    Planet * planet = (Planet *)elem;
    free(planet->name);
    free(planet);
    return success;
}

void destroyOrigin(Origin *origin)
{
    free(origin->dim_name);
    free(origin);
}

status destroyChara(Element elem)
{
    if(elem == NULL)
        return argument_failure;
    PhysicalCharacteristic * chara = (PhysicalCharacteristic *)elem;
    free(chara->name);
    free(chara);
    return success;
}

status destroyJerry(Element elem)
{
    if(elem == NULL)
        return argument_failure;
    Jerry * jerry = (Jerry *)elem;
    free(jerry->id);
    destroyJerryChara(jerry);
    destroyOrigin(jerry->origin);
    free(jerry);
    return success;
}

static void destroyJerryChara(Jerry *jerry)
{
    for (int i = 0; i < jerry->num_of_chars; i++)
    {
        destroyChara(jerry->phy_char[i]);
    }
    free(jerry->phy_char);
}


status printJerry(Element elem)
{
    if (elem == NULL)
        return argument_failure;
    Jerry * jerry = (Jerry *)elem;
    printf("Jerry , ID - %s : \n", jerry->id);
    printf("Happiness level : %d \n", jerry->happiness);
    printf("Origin : %s \n", jerry->origin->dim_name);
    printPlanet(&(jerry->origin->planet));
    if (jerry->num_of_chars != 0)
    {
        printf("Jerry's physical Characteristics available : \n");
        if (jerry->num_of_chars != 0)
        {
            printf("\t%s : %.2f ", jerry->phy_char[0]->name, jerry->phy_char[0]->value);
            for (int i = 1; i < jerry->num_of_chars; i++)
                printf(", %s : %.2f ", jerry->phy_char[i]->name, jerry->phy_char[i]->value);
            printf("\n");
        }
    }
    return success;
}

status printPlanet(Element elem)
{
    if (elem == NULL)
        return argument_failure;
    Planet * planet = (Planet *)elem;
    printf("Planet : %s (%.2f,%.2f,%.2f) \n", planet->name, planet->cord_x, planet->cord_y, planet->cord_z);
    return success;
}

Element copyJerry(Element elem)
{
    return elem;
}

Element copyPlanet(Element elem)
{
    return elem;
}
bool eqJerry(Element elem, Element other)
{
    if(elem == NULL || other == NULL)
        return false;
    Jerry * jerry1 = (Jerry *)elem;
    Jerry * jerry2 = (Jerry *)other;
    if(strcmp(jerry1->id, jerry2->id) == 0)
        return true;
    return false;
}

bool eqPlanet(Element elem, Element other)
{
    if(elem == NULL || other == NULL)
        return false;
    Planet * planet1 = (Planet *)elem;
    Planet * planet2 = (Planet *)other;
    if(strcmp(planet1->name, planet2->name) == 0)
        return true;
    return false;
}

bool nameInPlanet(Element elem, Element key)
{
    if(elem == NULL || key == NULL)
        return false;
    Planet * planet = (Planet *)elem;
    char * name = (char *)key;
    if(strcmp(planet->name, name) == 0)
        return true;
    return false;
}

bool idInJerry(Element elem, Element key)
{
    if(elem == NULL || key == NULL)
        return false;
    Jerry * jerry = (Jerry *)elem;
    char * id = (char *)key;
    if(strcmp(jerry->id, id) == 0)
        return true;
    return false;
}
