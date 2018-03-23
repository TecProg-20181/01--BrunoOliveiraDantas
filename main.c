#include <stdio.h>

#define RED 0
#define GREEN 1
#define BLUE 2
#define RGB 3


typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> red
    // 1 -> green
    // 2 -> blue
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;


int higher_number(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int smaller_number(int a, int b) {
    if (a < b)
        return a;
    return b;
}

Image read_image (Image img) {
     // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][RED],
                                 &img.pixel[i][j][GREEN],
                                 &img.pixel[i][j][BLUE]);

        }
    }
    return img;
}

Image print_image(Image img) {
    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][RED],
                                   img.pixel[i][j][GREEN],
                                   img.pixel[i][j][BLUE]);

        }
        printf("\n");
    }
    return img;
}

Image grey_scale(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int mean = img.pixel[i][j][RED] +
                        img.pixel[i][j][GREEN] +
                        img.pixel[i][j][BLUE];
            mean /= 3;
            img.pixel[i][j][RED] = mean;
            img.pixel[i][j][GREEN] = mean;
            img.pixel[i][j][BLUE] = mean;
        }
    }

    return img;
}

Image sepia_filter( Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            unsigned short int pixel[RGB];
            pixel[RED] = img.pixel[i][j][RED];
            pixel[GREEN] = img.pixel[i][j][GREEN];
            pixel[BLUE] = img.pixel[i][j][BLUE];

            int p =  pixel[RED] * .393 + pixel[GREEN] * .769 + pixel[BLUE] * .189;
            int menor_r = smaller_number(255, p);
            img.pixel[i][j][RED] = menor_r;

            p =  pixel[RED] * .349 + pixel[GREEN] * .686 + pixel[BLUE] * .168;
            menor_r = smaller_number(255, p);
            img.pixel[i][j][GREEN] = menor_r;

            p =  pixel[RED] * .272 + pixel[GREEN] * .534 + pixel[BLUE] * .131;
            menor_r = smaller_number(255, p);
            img.pixel[i][j][BLUE] = menor_r;
        }
    }
    return img;
}

Image blur(Image img) {
    int size = 0;
    scanf("%d", &size);

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel mean = {0, 0, 0};

            int menor_height = smaller_number(img.height - 1, i + size/2);
            int min_width = smaller_number(img.width - 1, j + size/2);

            for(int x = higher_number(0, i - size/2) ; x <= menor_height; ++x) {
                for(int y = higher_number(0, j - size/2) ; y <= min_width; ++y) {
                    mean.red += img.pixel[x][y][RED];
                    mean.green += img.pixel[x][y][GREEN];
                    mean.blue += img.pixel[x][y][BLUE];
                }
            }

            mean.red /= size * size;
            mean.green /= size * size;
            mean.blue /= size * size;

            img.pixel[i][j][RED] = mean.red;
            img.pixel[i][j][GREEN] = mean.green;
            img.pixel[i][j][BLUE] = mean.blue;
        }
    }
    return img;
}

Image rotate_90_right(Image img) {
    Image rotated;

    rotated.width = img.height;
    rotated.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated.pixel[i][j][RED] = img.pixel[x][y][RED];
            rotated.pixel[i][j][GREEN] = img.pixel[x][y][GREEN];
            rotated.pixel[i][j][BLUE] = img.pixel[x][y][BLUE];
        }
    }

    return rotated;
}

Image amount_rotation(Image img) {
    int how_many_times = 0;
    scanf("%d", &how_many_times);
    how_many_times %= 4;
    for (int i = 0; i < how_many_times; ++i) {
        img = rotate_90_right(img);
    }
    return img;
}

Image mirror(Image img) {
    int horizontal = 0;
    scanf("%d", &horizontal);

    int width = img.width, height = img.height;

    if (horizontal == 1) width /= 2;
    else height /= 2;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int x = i, y = j;

            if (horizontal == 1) y = img.width - 1 - j;
            else x = img.height - 1 - i;

            Pixel aux1;
            aux1.red = img.pixel[i][j][RED];
            aux1.green = img.pixel[i][j][GREEN];
            aux1.blue = img.pixel[i][j][BLUE];

            img.pixel[i][j][RED] = img.pixel[x][y][RED];
            img.pixel[i][j][GREEN] = img.pixel[x][y][GREEN];
            img.pixel[i][j][BLUE] = img.pixel[x][y][BLUE];

            img.pixel[x][y][RED] = aux1.red;
            img.pixel[x][y][GREEN] = aux1.green;
            img.pixel[x][y][BLUE] = aux1.blue;
        }
    }
    return img;
}

Image invert_colors(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j][RED] = 255 - img.pixel[i][j][RED];
            img.pixel[i][j][GREEN] = 255 - img.pixel[i][j][GREEN];
            img.pixel[i][j][BLUE] = 255 - img.pixel[i][j][BLUE];
        }
    }
    return img;
}

Image crop_image(Image img) {
    int x, y;
    scanf("%d %d", &x, &y);
    int new_width, new_height;
    scanf("%d %d", &new_width, &new_height);

    Image cut;

    cut.width = new_width;
    cut.height = new_height;

    for(int i = 0; i < new_height; ++i) {
        for(int j = 0; j < new_width; ++j) {
            cut.pixel[i][j][RED] = img.pixel[i + y][j + x][RED];
            cut.pixel[i][j][GREEN] = img.pixel[i + y][j + x][GREEN];
            cut.pixel[i][j][BLUE] = img.pixel[i + y][j + x][BLUE];
        }
    }

    return cut;
}

int main() {

    Image img;
    img = read_image(img);

    int option_number;
    scanf("%d", &option_number);

    for(int i = 0; i < option_number; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Escala de Cinza
                img = grey_scale(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = sepia_filter(img);
                break;
            }
            case 3: { // Blur
                img = blur(img);
                break;
            }
            case 4: { // Rotacao
                img = amount_rotation(img);
                break;
            }
            case 5: { // Espelhamento
                img = mirror(img);
                break;
            }
            case 6: { // Inversao de Cores
                img = invert_colors(img);
                break;
            }
            case 7: { // Cortar Imagem
                img = crop_image(img);
                break;
            }
        }
    }

    img = print_image(img);

    return 0;
}
