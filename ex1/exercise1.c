#include "common.h"

char material[][MAX_STRING_LENGTH] = {"Jets", "Fog", "Foam", "Dry agent"};

/**
 * character order:
 * Reactivity   0: nope, 1: V 
 * Protection   0: Full, 1: BA, 2: BA for fire only
 * Containment  0: Dilute, 1: Contain
*/
const char secondCodeState[13][3] = {"100", "000", "110", "120", "010", "020", "101", "001", "111", "121", "111", "121"};

/**
* ASCII char to int 
*/
int ctoi(char c)
{
    return c - 48;
}

int getIndexState(char c, int isReverseColor) {
    int stateIndex = -1;
    switch (c)
    {
    case 'P':
        stateIndex = 0;
        break;
    case 'R':
        stateIndex = 1;
        break;
    case 'S':
        if (!isReverseColor)
        {
            stateIndex = 2;
        }
        else
        {
            stateIndex = 3;
        }
        break;
    case 'T':
        if (!isReverseColor)
        {
            stateIndex = 4;
        }
        else
        {
            stateIndex = 5;
        }
        break;
    case 'W':
        stateIndex = 6;
        break;
    case 'X':
        stateIndex = 7;
        break;
    case 'Y':
        if (!isReverseColor)
        {
            stateIndex = 8;
        }
        else
        {
            stateIndex = 9;
        }
        break;
    case 'Z':
        if (!isReverseColor)
        {
            stateIndex = 10;
        }
        else
        {
            stateIndex = 11;
        }
        break;
    }
    return stateIndex;
}

void secondCodeHandler(char c, int isReverseColor)
{
    int index = getIndexState(c, isReverseColor);
    if(index < 0 || index > 11) {
        error("Index out of bound");
        return;
    }
    char *state = (char*) secondCodeState[index];
    if(state[0] == '1') {
        printf("Reactivity: can be violently reactive");
    }

    printf("\nProtection: ");
    if(state[1] == '0') {
        printf("full protective clothing must be worn");
    } else if(state[1] == '1') {
        printf("breathing apparatus");
    } else {
        printf("breathing apparatus, protective gloves for fire only");
    }

    printf("\nContainment: ");
    if(state[2] == '1') {
        printf("a need to avoid spillages from entering drains or water courses.");
    } else {
        printf("the dangerous goods may be washed down to a drain with a large quantity of water");
    }
}

void readHAZCHEMCode(char *code)
{
    int len = strlen(code);
    if (len == 0 || len > 3)
    {
        error("Format error");
        return;
    }

    int isReverseColor = 0;
    if(code[1] == 'S' || code[1] == 'T' || code[1] == 'Y' || code[1] == 'Z') {
        printf("Is the S reverse coloured? y/n: ");
        if(strcmp(getInputString(), "y")) {
            isReverseColor = 1;
        }
    }

    printf("\n\n***Emergency action advice***\n");

    int first = ctoi(code[0]);
    if (first < 1 || first > 4)
    {
        error(NULL);
        return;
    }

    printf("Material: %s\n", material[first - 1]);

    secondCodeHandler(code[1], isReverseColor);

    if (len == 3)
    {
        if (code[2] == 'E')
        {
            printf("\nEvacuation: consider evacuation\n");
        }
    }

    printf("*****************************\n");
}

int main(int argc, char const *argv[])
{
    printf("Enter HAZCHEM code: ");
    char* hazchemCode = getInputString();
    readHAZCHEMCode(hazchemCode);
    return 0;
}