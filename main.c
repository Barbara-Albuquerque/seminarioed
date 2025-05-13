#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int ID;
    int Prioridade;
    int Ciclos_espera;
} Processo;

typedef struct {
    Processo heap[MAX];
    int tamanho;
} FilaPrioridade;

FilaPrioridade criar_fila_prioridade() {
    FilaPrioridade fila;
    fila.tamanho = 0;
    return fila;
}


void swap(Processo *a, Processo *b) {
    Processo temp = *a;
    *a = *b;
    *b = temp;
}

void inserir(FilaPrioridade *fila, int ID, int Prioridade) {
    int i = fila->tamanho++;
    fila->heap[i].ID = ID;
    fila->heap[i].Prioridade = Prioridade;
    fila->heap[i].Ciclos_espera = 0;

    while (i != 0 && fila->heap[(i - 1) / 2].Prioridade < fila->heap[i].Prioridade) {
        swap(&fila->heap[i], &fila->heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}


Processo remover(FilaPrioridade *fila) {
    Processo max = fila->heap[0];
    fila->heap[0] = fila->heap[--fila->tamanho];

    int i = 0;
    while (2 * i + 1 < fila->tamanho) {
        int maior = i;
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;

        if (fila->heap[esq].Prioridade > fila->heap[maior].Prioridade) maior = esq;
        if (dir < fila->tamanho && fila->heap[dir].Prioridade > fila->heap[maior].Prioridade) maior = dir;

        if (maior == i) break;
        swap(&fila->heap[i], &fila->heap[maior]);
        i = maior;
    }

    return max;
}

void aging(FilaPrioridade *fila) {
    for (int i = 0; i < fila->tamanho; i++) {
        fila->heap[i].Ciclos_espera++;
        if (fila->heap[i].Ciclos_espera >= 3) {
            fila->heap[i].Prioridade++;
            fila->heap[i].Ciclos_espera = 0;
        }
    }
}

void print_heap(FilaPrioridade *fila) {
    printf("\nEstado Atual da Fila de Prioridade:\n");
    for (int i = 0; i < fila->tamanho; i++) {
        printf("ID: %d | Prioridade: %d | Tempo de Espera: %d\n",
               fila->heap[i].ID, fila->heap[i].Prioridade, fila->heap[i].Ciclos_espera);
    }
    printf("--------------------------\n");
}

int main() {
    
    FilaPrioridade fila_de_processos = criar_fila_prioridade();

    // Inserção inicial de processos
    inserir(&fila_de_processos, 1, 5);
    inserir(&fila_de_processos, 2, 4);
    inserir(&fila_de_processos, 3, 3);

    printf("🔧 Iniciando Escalonamento de Processos...\n");

    int ciclos = 0;
    while (fila_de_processos.tamanho > 0) {
        ciclos++;
        if (ciclos % 3 == 0) {  // A cada 3 ciclos, chega um novo processo
            int novo_id = 100 + ciclos;
            int nova_prioridade = rand() % 10 + 1;
            printf("\nNovo processo chegou! (ID %d, Prioridade %d)\n", novo_id, nova_prioridade);
            inserir(&fila_de_processos, novo_id, nova_prioridade);
        }
        // Executa o processo de maior prioridade
        Processo p = remover(&fila_de_processos);
        printf("\nExecutando Processo ID: %d | Prioridade: %d\n", p.ID, p.Prioridade);

        // Aging para processos restantes
        aging(&fila_de_processos);

        // Exibe o estado da fila após a execução
        print_heap(&fila_de_processos);
    }

    printf("\nTodos os processos foram executados.\n");
    return 0;
}
