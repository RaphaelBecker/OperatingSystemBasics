
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFERSIZE 16
unsigned char buffer[BUFFERSIZE];

void hexdump(FILE *input, FILE *output)
{

    size_t bytes_read = 0;
    unsigned int lines = 0, pufOffset = 0, i, tab;

    while ((bytes_read = fread(buffer, sizeof(unsigned char), BUFFERSIZE, input)) > 0)
    {
        // 6 character buffer offset with leading zeros
        pufOffset = lines * BUFFERSIZE;
        fprintf(output, "%06x : ", pufOffset);
        lines++;
        /* Output of the 16 next bytes in the buffer formatted in 2 characters with leading 0 in hexadecimal format,
where two consecutive outputs are separated by exactly one space character*/

        // prints 1 space, 1 colon, 1 space in output
        for (i = 0; i < bytes_read; i++)
        {
            fprintf(output, "%02x ", buffer[i]);
        }
        for (tab = 0; tab < (BUFFERSIZE - bytes_read); tab++)
        {
            fprintf(output, "   ");
        }
        fprintf(output, "  ");

        /*16 times the output of the previously hexadecimal displayed bytes as characters, if this is "printable*/
        for (i = 0; i < bytes_read; i++)
        {
            if (isprint(buffer[i]))
            {
                fprintf(output, "%c", buffer[i]);
            }
            else
            {
                fprintf(output, ".");
            }
        }
        fprintf(output, "\n");
        if (feof(input))
            break;
    }
}