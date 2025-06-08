#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <complex.h>

// Alterar esse valores conforme a necessidade
// para testar o desempenho do programa com diferentes valores
#define WIDTH 1920
#define HEIGHT 1080
#define MAX_ITER 1000
#define X_MIN   -2.0
#define X_MAX    2.0
#define Y_MIN   -1.0
#define Y_MAX    1.0

void save_image(const char *filename, unsigned char image[HEIGHT][WIDTH][3]) {
    FILE *f = fopen(filename, "wb");
    fprintf(f, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(image, 1, WIDTH * HEIGHT * 3, f);
    fclose(f);
}

int main(int argc, char *argv[]) {
    struct timeval start, end;
    printf("Executando em modo sequencial...\n");
    printf("Imagem gerada será na resolução %dx%d\n", WIDTH, HEIGHT);
    printf("Número máximo de iterações: %d\n", MAX_ITER);
    printf("Intervalo de valores: x = [%.2f, %.2f], y = [%.2f, %.2f]\n", X_MIN, X_MAX, Y_MIN, Y_MAX);

    unsigned char image[HEIGHT][WIDTH][3];

    gettimeofday(&start, NULL);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double real = (x - WIDTH / 2.0) * (X_MAX - X_MIN) / WIDTH;
            double imag = (y - HEIGHT / 2.0) * (Y_MAX - Y_MIN) / HEIGHT;
            double complex c = real + imag * I;
            double complex z = 0.0 + 0.0 * I;
            int iter;
            for (iter = 0; iter < MAX_ITER; iter++) {
                z = z * z + c;
                if (cabs(z) > 2.0)
                    break;
            }
            if (iter == MAX_ITER) {
                image[y][x][0] = 0;
                image[y][x][1] = 0;
                image[y][x][2] = 0;
            } else {
                int r = (int)(255.0 * sin(0.016 * iter + 4));
                int g = (int)(255.0 * sin(0.013 * iter + 2));
                int b = (int)(255.0 * sin(0.01 * iter + 1));
                image[y][x][0] = r < 0 ? 0 : r > 255 ? 255 : r;
                image[y][x][1] = g < 0 ? 0 : g > 255 ? 255 : g;
                image[y][x][2] = b < 0 ? 0 : b > 255 ? 255 : b;
            }
        }
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6;
    printf("Tempo de execução (sequencial): %.2f segundos\n", elapsed);

    save_image("mandelbrot.ppm", image);
    printf("Imagem salva como mandelbrot.ppm\n");
    return 0;
}
