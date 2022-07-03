//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hexdump(FILE *input, FILE *output);

int main(int argc, char *argv[])
{
    // Dateizeiger erstellen (file handler)
    FILE *input;

    // If Numer of Inputarguments in console < 1 -> No input file parsed
    if (argc < 1)
    {
        return 1;
    }
    else
    {
        /* Opens all passed files and parses the input to function hexdump. Output is prsed to stdout.*/
        for (int i = 1; i < argc; i++)
        {
            input = fopen(argv[i], "rb");
            if (input == NULL)
            {
                return 1;
            }
            else
            {
                hexdump(input, stdout);
                fprintf(stdout, "\n");
                fclose(input);
            }
        }
    }
    return 0;
}
