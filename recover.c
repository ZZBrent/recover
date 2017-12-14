
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];

    // Setup buffer
    unsigned char buffer[512];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    bool currentlyCopying = false;
    int currentFile = 0;
    char fileName[8] = "000.jpg";
    FILE *outptr;

    fread(buffer, 512, 1, inptr);

    while(fread(buffer, 512, 1, inptr) > 0)
    {
        if(buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            //If currently copying, close the file
            if(currentlyCopying) {
                fclose(outptr);

                //Get new file name
                currentFile++;

                // convert current file number to string
                sprintf(fileName, "%03d.jpg", currentFile);
            }

            currentlyCopying = true;

            // open output file
            outptr = fopen(fileName, "w");

            // write characters to outfile
            fwrite(buffer, sizeof(buffer), 1, outptr);
        }
        else {
            if(currentlyCopying) {
            // write characters to outfile
                fwrite(buffer, sizeof(buffer), 1, outptr);
            }
        }
    }

    fclose(outptr);

    fclose(inptr);

    // success
    return 0;
}