#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INPUT_SIZE 256
#define NUM_COMMANDS 5

void command(char *buffer, unsigned int size)
{
    unsigned int position = 0, bufsize = size;

    // Read a character from stdin
    scanf("%s", buffer);

    // If we have exceeded the buffer, reallocate.
    if (position >= size)
    {
        size += bufsize;
        buffer = realloc(buffer, size);
        if (!buffer)
        {
            fprintf(stderr, "Exceeded buffer: allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
}

unsigned int readUserInput(char *command)
{
    // compares the user input stdin with the given command flags and calls the custom command funktion
    // command flags
    int a = 0;
    char *cExit = "exit";
    char *cHelp = "help";
    char *cSpell = "spell";
    char *cCountLetter = "countLetter";
    char *cReplace = "replace";

    // comparison of string stdin and command flag 
    if (strcmp(cExit, command) == 0)
        a = 1;
    else if (strcmp(cHelp, command) == 0)
        a = 2;
    else if (strcmp(cSpell, command) == 0)
        a = 3;
    else if (strcmp(cCountLetter, command) == 0)
        a = 4;
    else if (strcmp(cReplace, command) == 0)
        a = 5;
    else if (strcmp(cReplace, command) != 0)
        a = 6;

    // cases for custom command funktions
    switch (a)
    {
    //Exit: Case = 1
    case 1:
        fprintf(stdout, "Goodbye !\n");
        return 0;
    //Help: Case = 2
    case 2:
        // help text
        fprintf(stdout, "-------- Mini Shell --------\n");
        fprintf(stdout, "The following commands are supported:\n");
        fprintf(stdout, "- exit : Terminates the Mini Shell.\n");
        fprintf(stdout, "- help : Prints help information.\n");
        fprintf(stdout, "- spell : Asks for a string. Spells the string by printing each char of the string to a new line.\n");
        fprintf(stdout, "- countLetter : Asks for a string and a char. Prints the number of occurrences of the char in the string.\n");
        fprintf(stdout, "- replace : Asks for a string and two characters. Replaces all occurrences of the first char within the string with the second char.\n");
        fprintf(stdout, "- All regular shell commands are supported as well.\n");
        fprintf(stdout, "----------------------------\n");
        return 1;
    //Spell: Case = 3
    case 3:
        fprintf(stdout, "Please provide a string:\n");
        //int i = 0;
        char *str = malloc(sizeof(char) * 15);
        scanf("%s", str);
        //if( str[ i ] == '\n') str[i] = '\0';
        for (int i = 0; i <= (int)strlen(str)-1; i++) printf("%c\n", str[i]);
        return 1;
    //CounterLetter: Case 4;
    case 4:
        // initialize 
        fprintf(stdout, "Please provide a string:\n");
        char *str2 = malloc(sizeof(char) * 15);
        char *str3 = malloc(sizeof(char) * 1);
        scanf("%s", str2);
        fprintf(stdout,"Please provide a character:\n");
        scanf("%s", str3);

        // count letter str3 in str2
        int i, count;
        for (i=0, count=0; str2[i]; i++) count += (str2[i] == str3[0]);
        fprintf(stdout, "Character %c occurred %d times.\r", str3[0], count);
        return 1;
    //Replace: Case 5;
    case 5:
        //Replace: char str5 with str6 in str4
        // initialize 
        fprintf(stdout, "Please provide a string:\n");
        char *str4 = malloc(sizeof(char) * 15);
        char *str5 = malloc(sizeof(char) * 1);
        char *str6 = malloc(sizeof(char) * 1);
        scanf("%s", str4);
        fprintf(stdout,"Please provide the first character:\n");
        scanf("%s", str5);
        fprintf(stdout,"Please provide the second character:\n");
        scanf("%s", str6);
        for (i=0, count=0; str4[i]; i++) if (str4[i] == str5[0]) str4[i] = str6[0];
        fprintf(stdout, "New word: %s\n", str4);
        return 1;
    default:
        // Command to System  
        printf("SystemCall: ");
        system(command);
        return 1;
    }
}

int main(void)
{
    char *buffer = malloc(sizeof(char) * INPUT_SIZE);
    unsigned int status = 0;
    fprintf(stdout, "This is the Mini Shell:\n");
    do
    {
        command(buffer, INPUT_SIZE);
        status = readUserInput(buffer);
    } while (status == 1);
    return 0;
}
