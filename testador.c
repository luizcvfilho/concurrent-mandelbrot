// Salve como: testador.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para ler um arquivo inteiro para a memória
unsigned char* read_file_to_buffer(const char* filename, long* file_size) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    *file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char* buffer = malloc(*file_size);
    if (!buffer) {
        fprintf(stderr, "Erro ao alocar memória para o buffer do arquivo.\n");
        fclose(f);
        return NULL;
    }

    if (fread(buffer, 1, *file_size, f) != *file_size) {
        fprintf(stderr, "Erro ao ler o arquivo para o buffer.\n");
        fclose(f);
        free(buffer);
        return NULL;
    }

    fclose(f);
    return buffer;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <numero de threads para o teste>\n", argv[0]);
        return 1;
    }

    char command[256];
    const char* default_out_name = "mandelbrot.ppm";
    const char* seq_out_name = "mandelbrot_seq.ppm";
    const char* conc_out_name = "mandelbrot_conc.ppm";

    printf("--- INICIANDO TESTE DE CORRETUDE ---\n");

    // 1. Executar a versão sequencial
    printf("1. Executando a versão sequencial para gerar a referência...\n");
    if (system("./sequential-version.exe") != 0) {
        fprintf(stderr, "Erro ao executar o programa sequencial.\n");
        return 1;
    }
    rename(default_out_name, seq_out_name); // Renomeia a saída

    // 2. Executar a versão concorrente
    printf("2. Executando a versão concorrente com %s threads...\n", argv[1]);
    sprintf(command, "./concurrent-version.exe %s", argv[1]);
    if (system(command) != 0) {
        fprintf(stderr, "Erro ao executar o programa concorrente.\n");
        remove(seq_out_name); // Limpeza
        return 1;
    }
    rename(default_out_name, conc_out_name); // Renomeia a saída

    // 3. Ler os dois arquivos para a memória
    printf("3. Lendo os arquivos de saída para comparação...\n");
    long seq_size, conc_size;
    unsigned char* seq_buffer = read_file_to_buffer(seq_out_name, &seq_size);
    unsigned char* conc_buffer = read_file_to_buffer(conc_out_name, &conc_size);

    if (!seq_buffer || !conc_buffer) {
        fprintf(stderr, "Não foi possível ler os arquivos para os buffers.\n");
        free(seq_buffer);
        free(conc_buffer);
        remove(seq_out_name);
        remove(conc_out_name);
        return 1;
    }

    // 4. Comparar os resultados
    printf("4. Comparando os resultados...\n");
    int test_passed = 0;
    if (seq_size != conc_size) {
        printf("\n❌ \033[0;31mTESTE FALHOU:\033[0m Os tamanhos dos arquivos são diferentes! (%ld vs %ld bytes)\n", seq_size, conc_size);
    } else if (memcmp(seq_buffer, conc_buffer, seq_size) != 0) {
        printf("\n❌ \033[0;31mTESTE FALHOU:\033[0m O conteúdo dos arquivos é diferente!\n");
    } else {
        printf("\n✅ \033[0;32mTESTE PASSOU:\033[0m As saídas são idênticas.\n");
        test_passed = 1;
    }

    // 5. Limpeza
    free(seq_buffer);
    free(conc_buffer);
    remove(seq_out_name);
    remove(conc_out_name);

    return test_passed ? 0 : 1; // Retorna 0 em caso de sucesso, 1 em caso de falha
}