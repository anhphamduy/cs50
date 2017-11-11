/**
 * Resize a BMP file
 * Anh Pham
 */

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure the proper input
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // variables to store the resize size, infile filename and outfile filename
    int ratio = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // ensure that the ratio must be within 0 and 100
    if (ratio > 100 || ratio <= 0)
    {
        fprintf(stderr, "The resize ratio must be within 0 and 100");
        return 1;
    }

    // open the files and ensure that they are not NULL
    FILE *inptr = fopen(infile, "r");
    FILE *outptr = fopen(outfile, "w");

    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }
    else if (outptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", outfile);
        return 1;
    }

    // read bitmap file header and info file header
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;

    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure the file is a legal bitmap file
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 1;
    }

    // saves the old information of the old file
    int old_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int old_width = bi.biWidth;
    int old_height = bi.biHeight;

    // new file width and new file height
    bi.biWidth = bi.biWidth * ratio;
    bi.biHeight = bi.biHeight * ratio;

    // new padding
    int new_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // new file size
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + new_padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BIMAPFILEHEADER and BITMAPINFOHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over scanlines and pixels with the new ratio
    for (int i = 0; i < abs(old_height); i++)
    {
        for (int k = 0; k < ratio; k++)
        {
            for (int j = 0; j < old_width; j++)
            {
                // temprorary variable for RGBTRIPLE
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                for (int h = 0; h < ratio; h++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }

            }

            // put padding in the new BMG file
            for (int a = 0; a < new_padding; a++)
            {
                fputc(0x00, outptr);
            }

            // send the cursor back
            int seek = old_width * sizeof(RGBTRIPLE);

            if (k != ratio - 1)
                fseek(inptr, -seek, SEEK_CUR);


        }

        // skip over padding in the old BMP file
        fseek(inptr, old_padding, SEEK_CUR);
    }

    fclose(outptr);
    fclose(inptr);

}