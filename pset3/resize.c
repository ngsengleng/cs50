// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

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
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // check for numerical input
    float factor;

    if (isdigit(argv[1][0]) || (argv[1][0] == '.' && isdigit(argv[1][1])))
    {
        factor = atof(argv[1]);
        if (factor > 1)
        {
            factor = round(factor);
        }
    }
    else
    {
        printf("Usage: ./resize f infile outfile\n");
        return 5;
    }
    if (factor > 100)
    {
        printf("factor is (0.00, 100.00]\n");
        return 6;
    }

    // copy of infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bfT = bf;

    // copy of infile's BITMAPINFOHEADER
    BITMAPINFOHEADER biT = bi;

    // scaling
    biT.biWidth *= factor;
    biT.biHeight *= factor;
    int paddingT = (4 - (biT.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    biT.biSizeImage = (paddingT + (biT.biWidth * sizeof(RGBTRIPLE))) * abs(biT.biHeight);
    bfT.bfSize = biT.biSizeImage + 54;
    printf("%d_%d\n", bfT.bfSize, biT.biSizeImage);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfT, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biT, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 1, biHeight = abs(biT.biHeight); i < biHeight + 1; i++)
    {
         for (int j = 1; j < biT.biWidth + 1; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            int posW = ceil(j/factor);
            int posH = ceil(i/factor);

            // skip metadata and previous scanlines(if any);
            fseek(inptr, 54 + (posH - 1) * ((bi.biWidth  * sizeof(RGBTRIPLE)) + padding), SEEK_SET);

            // read RGB triple from infile
            fseek(inptr,(posW - 1) * 3, SEEK_CUR);
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }
        for (int k = 0; k < paddingT; k++)
        {
            fputc(0x00, outptr);
        }
    }
    printf("%d_%d\n", bfT.bfSize, biT.biSizeImage);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
