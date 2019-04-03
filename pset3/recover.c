#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // check for correct input
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // open file for reading
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr,"could not open file %s", argv[1]);
        return 2;
    }

    unsigned char buffer[512];
    int counter = 0;
    char filename[8] = "000.jpg";
    FILE *outptr = fopen(filename, "w");

    while (fread(buffer, sizeof(buffer), 1, inptr) == 1)
    {
        // confirm start of jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (0xf0 - buffer[3]) <= 16)
        {
            fclose(outptr);
            sprintf(filename, "%03d.jpg", counter);
            outptr = fopen(filename, "w");
            counter++;
        }
        fwrite(buffer, sizeof(buffer), 1, outptr);
    }

    fclose(inptr);
}

