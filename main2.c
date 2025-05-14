#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int ID;
    int Prioridade;
    int Ciclos_espera; // dentro da prioridade atual
    int Ciclos_executados;
    int Ciclos_necessarios;
    char Tipo; // 'b' para background, 'i' para interativo
} Processo;

typedef struct {
    Processo heap[MAX];
    int tamanho;
} FilaPrioridade;

FilaPrioridade criar_fila_prioridade() {
    FilaPrioridade fila;
    fila.tamanho = 0; // inicializa a fila ja com tamanho 0
    return fila;
}

void swap(Processo *a, Processo *b) {
    Processo temp = *a;
    *a = *b;
    *b = temp;
}

void inserir(FilaPrioridade *fila, int ID, int Prioridade, char tipo, int ciclos_necessarios) {
    int i = fila->tamanho;
    
    fila->heap[i].ID = ID;
    fila->heap[i].Prioridade = Prioridade;
    fila->heap[i].Ciclos_espera = 0;
    fila->heap[i].Ciclos_executados = 0;
    fila->heap[i].Ciclos_necessarios = ciclos_necessarios;
    fila->heap[i].Tipo = tipo;

    while (i != 0) {
        int dad = (i - 1) / 2;

        // se o processo pai tem prioridade idx_maior que o inserido, nao faz nada 
        if (fila->heap[dad].Prioridade > fila->heap[i].Prioridade) break;

        // se a prioridade do pai e do inserido sao iguais, sobe o do tipo 'i'
        if (fila->heap[dad].Prioridade == fila->heap[i].Prioridade &&
            fila->heap[dad].Tipo == 'i') break; // dad tem prioridade igual e é interativo, não troca

        // Caso contrário, faz a troca
        swap(&fila->heap[i], &fila->heap[dad]);
        i = dad;
    }
    fila->tamanho++;
}   


Processo remover(FilaPrioridade *fila) {
    Processo max = fila->heap[0];
    fila->heap[0] = fila->heap[fila->tamanho];
    fila->tamanho--;
    int i = 0;
    while (2 * i + 1 < fila->tamanho) {
        int idx_maior = i;
        int idx_esq = 2 * i + 1;
        int idx_dir = 2 * i + 2;

        // checa se o filho da idx_esquerda tem prioridade idx_maior
        //se as prioridades forem iguais, checa o tipo
        if (fila->heap[idx_esq].Prioridade > fila->heap[idx_maior].Prioridade || 
        (fila->heap[idx_esq].Prioridade == fila->heap[idx_maior].Prioridade && fila->heap[idx_esq].Tipo == 'i' && fila->heap[idx_maior].Tipo != 'i')) 
        {
            idx_maior = idx_esq;
        }
        // checa se o filho da idx_direita existe, se ele tem idx_maior prioridade
        //se a prioridade deles forem iguais, checa o tipo
    if (idx_dir < fila->tamanho && (fila->heap[idx_dir].Prioridade > fila->heap[idx_maior].Prioridade || 
        (fila->heap[idx_dir].Prioridade == fila->heap[idx_maior].Prioridade && fila->heap[idx_dir].Tipo == 'i' && fila->heap[idx_maior].Tipo != 'i'))) 
        {
            idx_maior = idx_dir;
        }
        if (idx_maior == i) break;
        swap(&fila->heap[i], &fila->heap[idx_maior]);
        i = idx_maior;
    }
    return max;
}

void aging(FilaPrioridade *fila) {
    for (int i = 0; i < fila->tamanho; i++) {
        fila->heap[i].Ciclos_espera++;

        if (fila->heap[i].Tipo == 'i' && fila->heap[i].Ciclos_espera >= 2) {
            fila->heap[i].Prioridade++;
            fila->heap[i].Ciclos_espera = 0;
        } 
        else if (fila->heap[i].Tipo == 'b' && fila->heap[i].Ciclos_espera >= 5) {
            fila->heap[i].Prioridade++;
            fila->heap[i].Ciclos_espera = 0;
        }
    }
}

void print_heap(FilaPrioridade *fila) {
    printf("\n Estado Atual da Fila de Prioridade:\n");
    for (int i = 0; i < fila->tamanho; i++) {
        printf("ID: %d | Prioridade: %d | Tipo: %s | Espera: %d | Executados: %d/%d\n",
               fila->heap[i].ID,
               fila->heap[i].Prioridade,
               fila->heap[i].Tipo == 'i' ? "Interativo" : "Background",
               fila->heap[i].Ciclos_espera,
               fila->heap[i].Ciclos_executados,
               fila->heap[i].Ciclos_necessarios);
    }
    printf("\n--------------------------\n");
}

void executar_processo(FilaPrioridade *fila) {
    if (fila->tamanho == 0) return;

    Processo *p = &fila->heap[0];
    p->Ciclos_executados++;

    printf("\n Executando Processo ID #%d | Prioridade: %d | Ciclo %d/%d\n",
           p->ID, p->Prioridade, p->Ciclos_executados, p->Ciclos_necessarios);

    // Aumento de prioridade automatico se faltando 1 ciclo para terminar
    if (p->Ciclos_necessarios - p->Ciclos_executados <= 1) {
        if (p->Tipo == 'i' && p->Prioridade < 10) {
            printf("Processo Interativo ID #%d prestes a finalizar! Prioridade elevada para 10.\n", p->ID);
            p->Prioridade = 10;
        } else if (p->Tipo == 'b' && p->Prioridade < 9) {
            printf("Processo Background ID #%d prestes a finalizar! Prioridade elevada para 9.\n", p->ID);
            p->Prioridade = 9;
        }
    }
    // Se finalizou, remove da fila
    if (p->Ciclos_executados >= p->Ciclos_necessarios) {
        printf("Processo ID #%d concluido e removido da fila.\n", p->ID);
        remover(fila);
    }
}

int main() {
    FilaPrioridade fila = criar_fila_prioridade();
    int ciclos = 0;
    // processos inseridos na fila com suas configuracoes predefinidas
    inserir(&fila, 1, 3, 'i', 4);
    inserir(&fila, 2, 2, 'b', 5);
    inserir(&fila, 3, 4, 'i', 2);

    print_heap(&fila);

    printf("\n Iniciando Agendamento de Processos...\n");

    while (fila.tamanho > 0) {
        ciclos++;
        printf("Ciclo numero %d\n", ciclos);

        // Simula chegada de novos processos
        if (ciclos % 4 == 0) {
            int novo_id = 100 + ciclos;
            int nova_prioridade = rand() % 10 + 1;
            char tipo = (rand() % 2 == 0) ? 'i' : 'b';
            int ciclos_necessarios = (rand() % 5) + 1; // 1 a 5 ciclos
            printf("\n Novo processo chegou! (ID #%d, Prioridade %d, Ciclos Necessarios %d)\n",
                   novo_id, nova_prioridade, ciclos_necessarios);
            inserir(&fila, novo_id, nova_prioridade, tipo, ciclos_necessarios);
        }

        // Executa o processo no topo (sem removê-lo idx_diretamente)
        executar_processo(&fila);

        // Aging dos processos restantes
        aging(&fila);

        // Exibe a fila
        print_heap(&fila);
    }

    printf("\n Todos os processos foram executados.\n");
    return 0;
}
