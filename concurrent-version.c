#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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


typedef struct {
    int start_pixel;
    int end_pixel;
    unsigned char (*image)[WIDTH][3];
} ThreadData;

void save_image(const char *filename, unsigned char image[HEIGHT][WIDTH][3]) {
    FILE *f = fopen(filename, "wb");
    fprintf(f, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(image, 1, WIDTH * HEIGHT * 3, f);
    fclose(f);
}

void *mandelbrot_worker(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start_pixel; i < data->end_pixel; i++) {
        int y = i / WIDTH;
        int x = i % WIDTH;
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
            data->image[y][x][0] = 0;
            data->image[y][x][1] = 0;
            data->image[y][x][2] = 0;
        } else {
            int r = (int)(255.0 * sin(0.016 * iter + 4));
            int g = (int)(255.0 * sin(0.013 * iter + 2));
            int b = (int)(255.0 * sin(0.01 * iter + 1));
            data->image[y][x][0] = r < 0 ? 0 : r > 255 ? 255 : r;
            data->image[y][x][1] = g < 0 ? 0 : g > 255 ? 255 : g;
            data->image[y][x][2] = b < 0 ? 0 : b > 255 ? 255 : b;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int num_threads;
    struct timeval start, end;
    if (argc < 2) {
        printf("Uso: %s <número de threads>\n", argv[0]);
        return 1;
    }
    num_threads = atoi(argv[1]);
    if (num_threads < 1) {
        printf("Número de threads deve ser pelo menos 1.\n");
        return 1;
    }

    printf("Executando com %d threads...\n", num_threads);
    printf("Imagem gerada será na resolução %dx%d\n", WIDTH, HEIGHT);
    printf("Número máximo de de iterações: %d\n", MAX_ITER);
    printf("Intervalo de valores: x = [%.2f, %.2f], y = [%.2f, %.2f]\n", X_MIN, X_MAX, Y_MIN, Y_MAX);

    unsigned char image[HEIGHT][WIDTH][3];
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadData *thread_data = malloc(num_threads * sizeof(ThreadData));

    int total_pixels = WIDTH * HEIGHT;
    int pixels_per_thread = total_pixels / num_threads;
    int remainder = total_pixels % num_threads;

    gettimeofday(&start, NULL);

    int pixel_start = 0;
    for (int i = 0; i < num_threads; i++) {
        int pixel_end = pixel_start + pixels_per_thread;
        if (i == num_threads - 1)
            pixel_end += remainder;
        thread_data[i].start_pixel = pixel_start;
        thread_data[i].end_pixel = pixel_end;
        thread_data[i].image = image;
        pthread_create(&threads[i], NULL, mandelbrot_worker, &thread_data[i]);
        pixel_start = pixel_end;
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&end, NULL);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6;
    printf("Tempo de execução com %d threads: %.2f segundos\n", num_threads, elapsed);

    save_image("mandelbrot.ppm", image);
    printf("Imagem salva como mandelbrot.ppm\n");
    free(threads);
    free(thread_data);
    return 0;
}

