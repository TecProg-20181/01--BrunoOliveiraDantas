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


int maior_numero(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int menor_numero(int a, int b) {
    if (a < b)
        return a;
    return b;
}

Image ler_imagem (Image img) {
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

Image printar_imagem(Image img) {
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

Image escala_de_cinza(Image img) {

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

Image filtro_sepia( Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            unsigned short int pixel[RGB];
            pixel[RED] = img.pixel[i][j][RED];
            pixel[GREEN] = img.pixel[i][j][GREEN];
            pixel[BLUE] = img.pixel[i][j][BLUE];

            int p =  pixel[RED] * .393 + pixel[GREEN] * .769 + pixel[BLUE] * .189;
            int menor_r = menor_numero(255, p);
            img.pixel[i][j][RED] = menor_r;

            p =  pixel[RED] * .349 + pixel[GREEN] * .686 + pixel[BLUE] * .168;
            menor_r = menor_numero(255, p);
            img.pixel[i][j][GREEN] = menor_r;

            p =  pixel[RED] * .272 + pixel[GREEN] * .534 + pixel[BLUE] * .131;
            menor_r = menor_numero(255, p);
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
            
            int menor_height = menor_numero(img.height - 1, i + size/2);
            int min_width = menor_numero(img.width - 1, j + size/2);

            for(int x = maior_numero(0, i - size/2) ; x <= menor_height; ++x) {
                for(int y = maior_numero(0, j - size/2) ; y <= min_width; ++y) {
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

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
        for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][RED] = img.pixel[x][y][RED];
            rotacionada.pixel[i][j][GREEN] = img.pixel[x][y][GREEN];
            rotacionada.pixel[i][j][BLUE] = img.pixel[x][y][BLUE];
        }
    }

    return rotacionada;
}

Image quantidade_de_rotacao(Image img) {
    int quantas_vezes = 0;
    scanf("%d", &quantas_vezes);
    quantas_vezes %= 4;
    for (int i = 0; i < quantas_vezes; ++i) {
        img = rotacionar90direita(img);
    }
    return img;
}

Image espelhamento(Image img) {
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

Image inverter_cores(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j][RED] = 255 - img.pixel[i][j][RED];
            img.pixel[i][j][GREEN] = 255 - img.pixel[i][j][GREEN];
            img.pixel[i][j][BLUE] = 255 - img.pixel[i][j][BLUE];
        }
    }
    return img;
}

Image cortar_imagem(Image img) {
    int x, y;
    scanf("%d %d", &x, &y);
    int new_width, new_height;
    scanf("%d %d", &new_width, &new_height);

    Image cortada;

    cortada.width = new_width;
    cortada.height = new_height;

    for(int i = 0; i < new_height; ++i) {
        for(int j = 0; j < new_width; ++j) {
            cortada.pixel[i][j][RED] = img.pixel[i + y][j + x][RED];
            cortada.pixel[i][j][GREEN] = img.pixel[i + y][j + x][GREEN];
            cortada.pixel[i][j][BLUE] = img.pixel[i + y][j + x][BLUE];
        }
    }

    return cortada;
}

int main() {

    Image img;
    img = ler_imagem(img);

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = filtro_sepia(img);
                break;
            }
            case 3: { // Blur
                img = blur(img);
                break;
            }
            case 4: { // Rotacao
                img = quantidade_de_rotacao(img);
                break;
            }
            case 5: { // Espelhamento
                img = espelhamento(img);
                break;
            }
            case 6: { // Inversao de Cores
                img = inverter_cores(img);
                break;
            }
            case 7: { // Cortar Imagem
                img = cortar_imagem(img);
                break;
            }
        }
    }

    img = printar_imagem(img);

    return 0;
}
