#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <cs50.h>


#define MEMORY_BLOCK 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // ensure there is enough argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover filename\n");
        return 1;
    }

    // open file and ensure it is valid
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Unsupported file format\n");
        return 2;
    }

    // counter for jpg's filename
    int file_counter = 0;

    // loop through the memory
    bool end_of_file = false;
    while (!feof(inptr) && !end_of_file)
    {
        BYTE buffer[MEMORY_BLOCK];
        fread(&buffer, MEMORY_BLOCK, 1, inptr);

        // when detect that start of the new JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff
            && (buffer[3] & 0xf0) == 0xe0)
        {

            // get filename
            char *filename = malloc(9);
            sprintf(filename, "%03i.jpg", file_counter);
            FILE *outptr = fopen(filename, "w");

            // loop through all the block till the start of new jpg
            bool end_of_image = false;
            while (!end_of_image)
            {

                // write the buffer into jpg
                fwrite(&buffer, sizeof(buffer), 1, outptr);

                // read new block of memory
                fread(&buffer, MEMORY_BLOCK, 1, inptr);

                // check if a new block is a new jpg
                if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff
                    && (buffer[3] & 0xf0) == 0xe0)
                {
                    fseek(inptr, -MEMORY_BLOCK, SEEK_CUR);
                    end_of_image = true;
                }

                // detect if the end of file
                if (feof(inptr))
                {
                    end_of_file = true;
                    end_of_image = true;
                }
            }

            // increment counter for new filename and free the old filename
            file_counter += 1;
            free(filename);

            // close file
            fclose(outptr);

        }
    }

    // close file
    fclose(inptr);
}