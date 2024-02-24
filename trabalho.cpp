#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
// #include <omp.h>
#include <time.h>

int comparar(const void *a, const void *b);
int particionar(int v[], int baixo, int alto);
void quicksort(int v[], int baixo, int alto);
void sequencial(int tamanhoVetor);
void paralelo(int argc, char **argv, int tamanhoVetor);

int main(int argc, char **argv) {
    int escolha = 0; 
    int tamanhosVetor[] = {512, 256, 128};

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("Escolha o modo de execução:\n");
        printf("1. Sequencial (sem MPI)\n");
        printf("2. Paralelo (com MPI)\n");
        scanf(" %d", &escolha);
    }

    MPI_Bcast(&escolha, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (escolha == 1 || escolha == 2) {
        for (int i = 0; i < 3; i++) {
            if (escolha == 1) {
                sequencial(tamanhosVetor[i]);
            } else {
                paralelo(argc, argv, tamanhosVetor[i]);
            }
        }
    } else {
        if (rank == 0) {
            printf("Escolha inválida.\n");
        }
    }

    MPI_Finalize();
    return 0;
}

void sequencial(int tamanhoVetor) {
    clock_t inicio, fim;
    double tempo_cpu;

    int *vetor = (int*)malloc(tamanhoVetor * sizeof(int));
    for (int i = 0; i < tamanhoVetor; i++) {
        vetor[i] = rand() % 100;
    }

    inicio = clock();
    qsort(vetor, tamanhoVetor, sizeof(int), comparar);
    fim = clock();

    tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo de execução sequencial para vetor de tamanho %d: %f segundos\n", tamanhoVetor, tempo_cpu);

    free(vetor);
}

void paralelo(int argc, char **argv, int tamanhoVetor) {
    int rank, size;
    double inicio, fim;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *vetor;
    if (rank == 0) {
        vetor = (int*)malloc(tamanhoVetor * sizeof(int));
        for (int i = 0; i < tamanhoVetor; i++) {
            vetor[i] = rand() % 100;
        }
    }

    MPI_Bcast(&tamanhoVetor, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        vetor = (int*)malloc(tamanhoVetor * sizeof(int));
    }

    MPI_Bcast(vetor, tamanhoVetor, MPI_INT, 0, MPI_COMM_WORLD);

    int tam = tamanhoVetor / size;
    int *subvetor = (int*)malloc(tam * sizeof(int));

    MPI_Scatter(vetor, tam, MPI_INT, subvetor, tam, MPI_INT, 0, MPI_COMM_WORLD);

    inicio = MPI_Wtime();
    quicksort(subvetor, 0, tam - 1);
    fim = MPI_Wtime();

    MPI_Gather(subvetor, tam, MPI_INT, vetor, tam, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        qsort(vetor, tamanhoVetor, sizeof(int), comparar);
        
        printf("Vetor Ordenado de tamanho %d: \n", tamanhoVetor);
        printf("Tempo de execução paralela (MPI) para vetor de tamanho %d: %f segundos\n", tamanhoVetor, fim - inicio);
        free(vetor);
    }
    free(subvetor);
}

int comparar(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int particionar(int v[], int baixo, int alto) {
    int pivo = v[alto]; 
    int i = (baixo - 1), aux;

    for (int j = baixo; j <= alto - 1; j++) {
        if (v[j] < pivo) {
            i++;
            aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }
    }
    aux = v[i + 1];
    v[i+1] = v[alto];
    v[alto] = aux;
    return (i + 1);
}

void quicksort(int v[], int baixo, int alto) {
    if (baixo < alto) {
        int pi = particionar(v, baixo, alto);

        #pragma omp task
        quicksort(v, baixo, pi - 1);
        #pragma omp task
        quicksort(v, pi + 1, alto);

        #pragma omp taskwait
    }
}
