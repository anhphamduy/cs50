/**
 * Whodunit @ Problem Set 4
 * Anh Pham
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // check for proper usage
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 1;
    }

    char *infile = argv[1];
    char *outfile = argv[2];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER and BITMAPINFOHEADER
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure the file is a BMP file
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER and BITMAPINFOHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine the padding
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels and change the pixel
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary variable
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            if (triple.rgbtRed == 0xff)
            {
                triple.rgbtBlue = 0xff;
                triple.rgbtGreen = 0xff;
            }

            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip padding and add it back
        fseek(inptr, padding, SEEK_CUR);
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    fclose(inptr);
    fclose(outptr);
    return 0;

}