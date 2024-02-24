# Paralelismo com MPI e QuickSort

Este projeto demonstra a implementação e comparação de algoritmos de ordenação QuickSort em dois modos distintos: sequencial e paralelo. Utiliza-se a biblioteca MPI (Message Passing Interface) para distribuir o processamento de ordenação de vetores em múltiplos processadores de forma paralela, visando uma redução significativa no tempo de execução em comparação ao modo sequencial tradicional. O objetivo é explorar e avaliar o potencial de aceleração que a computação paralela oferece para problemas de ordenação de grande escala.

## Funcionalidades

- **Ordenação Sequencial**: Utiliza o algoritmo QuickSort padrão para ordenar vetores de diferentes tamanhos de forma sequencial.
- **Ordenação Paralela com MPI**: Distribui partes do vetor a ser ordenado entre diferentes processos utilizando MPI, onde cada processo executa o QuickSort de forma independente em sua parcela de dados. Posteriormente, os resultados parciais são reunidos e ordenados para formar o vetor final ordenado.
- **Medição de Desempenho**: Calcula e compara o tempo de execução para os modos sequencial e paralelo, permitindo uma análise quantitativa do ganho de desempenho obtido com a paralelização.

## Dependências

Para compilar e executar este projeto, você precisará:

- Um compilador C, como GCC.
- A biblioteca MPI instalada em seu sistema. Esta biblioteca é essencial para a execução do código em modo paralelo.
- (Opcional) OpenMP para a versão paralelizada do QuickSort, caso deseje explorar a paralelização dentro dos próprios processos MPI.

## Como Compilar

Para compilar o programa, você pode usar o seguinte comando no terminal, assegurando-se de que o `mpicc` esteja disponível em seu ambiente:

```bash
mpicc -o quicksort_mpi quicksort_mpi.c -lm -fopenmp
```

Se decidir incluir OpenMP para paralelização interna dos processos (não demonstrado diretamente neste código, mas sugerido através dos comentários no QuickSort), o comando de compilação seria algo como:

```bash
mpicc -o quicksort_mpi quicksort_mpi.c -lm -fopenmp
```

## Como Executar

Após a compilação, o programa pode ser executado em um ambiente com MPI configurado, utilizando o comando mpirun ou mpiexec, dependendo da sua instalação MPI. Por exemplo, para executar com 4 processos, use:

```bash
mpirun -np 4 ./quicksort_mpi
```
ou
```bash
mpiexec -np 4 ./quicksort_mpi
```

## Estrutura do Código

O código é dividido em várias funções principais:

- main(): Inicializa o ambiente MPI, coleta a escolha do usuário entre execução sequencial ou paralela, e executa a ordenação para vetores de tamanhos pré-definidos.
- sequencial(): Executa a ordenação de um vetor de forma sequencial usando qsort da biblioteca padrão.
- paralelo(): Distribui a carga de trabalho entre processos MPI para ordenar partes do vetor em paralelo, e então ordena o vetor reunido.
- quicksort(), particionar(), comparar(): Implementam o algoritmo QuickSort e funções auxiliares para a ordenação dos vetores

#Notas

- O desempenho paralelo depende significativamente da arquitetura do sistema e da implementação MPI utilizada.
A eficiência da paralelização pode variar com o tamanho do vetor e o número de processos utilizados.
- Este projeto é uma base para exploração e pode ser estendido para incluir outras formas de paralelismo, como o uso de threads com OpenMP em conjunto com MPI.

Projeto feito em parceria com: [jvictorifran](https://github.com/Jvictorifran)!
