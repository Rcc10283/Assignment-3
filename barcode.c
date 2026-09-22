#include "barcode.h"

#include <stdio.h>
#include <stdlib.h>


static const char* encodings[10] = {
    "1110010", // 0
    "1100110", // 1
    "1101100", // 2
    "1010000", // 3
    "1011100", // 4
    "1001110", // 5
    "1000010", // 6
    "1000100", // 7
    "1001000", // 8
    "1110100"  // 9
};


void fill_column(struct image* img, int x, int white) {

    for (int y = 0; y < img->height; y++) {

        int index = y * img->width + x;

        if (white) {
            img->pixels[index].r = 255;
            img->pixels[index].g = 255;
            img->pixels[index].b = 255;
        } else {
            img->pixels[index].r = 0;
            img->pixels[index].g = 0;
            img->pixels[index].b = 0;
        }
    }
}


struct image* barcode(char* data, int width, int height) {

    struct image* img = malloc(sizeof(struct image));

    img->width = width;
    img->height = height;

    img->pixels = malloc(width * height * sizeof(struct pixel));

    int x = 0;

    /* Left quiet zone: 9 white columns */
    for (int i = 0; i < 9; i++) {
        fill_column(img, x, 1);
        x++;
    }

    /* Start pattern: 010 */
    fill_column(img, x++, 0);
    fill_column(img, x++, 1);
    fill_column(img, x++, 0);

    /* Encode first 6 digits */
    for (int i = 0; i < 6; i++) {

        int digit = data[i] - '0';
        const char* pattern = encodings[digit];

        for (int j = 0; j < 7; j++) {

            if (pattern[j] == '1') {
                fill_column(img, x++, 1);
            } else {
                fill_column(img, x++, 0);
            }
        }
    }

    /* Middle pattern: 10101 */
    fill_column(img, x++, 1);
    fill_column(img, x++, 0);
    fill_column(img, x++, 1);
    fill_column(img, x++, 0);
    fill_column(img, x++, 1);

    /* Encode last 6 digits using inverted pattern */
    for (int i = 6; i < 12; i++) {

        int digit = data[i] - '0';
        const char* pattern = encodings[digit];

        for (int j = 0; j < 7; j++) {

            if (pattern[j] == '1') {
                fill_column(img, x++, 0);
            } else {
                fill_column(img, x++, 1);
            }
        }
    }

    /* End pattern: 010 */
    fill_column(img, x++, 0);
    fill_column(img, x++, 1);
    fill_column(img, x++, 0);

    /* Right quiet zone: 9 white columns */
    for (int i = 0; i < 9; i++) {
        fill_column(img, x, 1);
        x++;
    }

    return img;
}