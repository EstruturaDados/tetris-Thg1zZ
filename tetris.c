#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define FILA_CAP 5
#define PILHA_CAP 3

typedef struct {
    char tipo; // peça tipo: I O T S Z L J (usei 7 tipos)
    int id;
} Peca;

// ----------- Fila circular -------------
typedef struct {
    Peca dados[FILA_CAP];
    int inicio; // index of first element
    int tamanho;
    int next_id;
} Fila;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->tamanho = 0;
    f->next_id = 1;
}

int filaVazia(Fila *f) { return f->tamanho == 0; }
int filaCheia(Fila *f) { return f->tamanho == FILA_CAP; }

Peca gerarPeca(Fila *f) {
    const char tipos[] = "IOTSZLJ"; // 7 tipos possiveis
    Peca p;
    p.tipo = tipos[rand() % (sizeof(tipos)-1)];
    p.id = f->next_id++;
    return p;
}

void mostrarFila(Fila *f) {
    printf("Fila (frente -> tras) [tamanho=%d]: ", f->tamanho);
    if (filaVazia(f)) {
        printf("<vazia>");
    } else {
        for (int i = 0; i < f->tamanho; ++i) {
            int idx = (f->inicio + i) % FILA_CAP;
            printf("(%c-%d)%s", f->dados[idx].tipo, f->dados[idx].id, (i==f->tamanho-1) ? "" : " ");
        }
    }
    printf("\n");
}

int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    int idx = (f->inicio + f->tamanho) % FILA_CAP;
    f->dados[idx] = p;
    f->tamanho++;
    return 1;
}

int dequeue(Fila *f, Peca *out) {
    if (filaVazia(f)) return 0;
    *out = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % FILA_CAP;
    f->tamanho--;
    return 1;
}

// ensure queue is filled up to capacity (used at start and after removals)
void reporFila(Fila *f) {
    while (!filaCheia(f)) {
        Peca p = gerarPeca(f);
        enqueue(f, p);
    }
}

// ----------- Pilha (stack) -------------
typedef struct {
    Peca dados[PILHA_CAP];
    int topo; // points to next free slot (0 when empty)
} Pilha;

void inicializarPilha(Pilha *p) { p->topo = 0; }
int pilhaVazia(Pilha *p) { return p->topo == 0; }
int pilhaCheia(Pilha *p) { return p->topo == PILHA_CAP; }

int push(Pilha *p, Peca item) {
    if (pilhaCheia(p)) return 0;
    p->dados[p->topo++] = item;
    return 1;
}

int pop(Pilha *p, Peca *out) {
    if (pilhaVazia(p)) return 0;
    *out = p->dados[--p->topo];
    return 1;
}

void mostrarPilha(Pilha *p) {
    printf("Pilha (topo -> base) [tamanho=%d]: ", p->topo);
    if (pilhaVazia(p)) {
        printf("<vazia>");
    } else {
        for (int i = p->topo - 1; i >= 0; --i) {
            printf("(%c-%d)%s", p->dados[i].tipo, p->dados[i].id, (i==0) ? "" : " ");
        }
    }
    printf("\n");
}

// ---------- Operacoes Mestre (trocas) ------------
// troca frente da fila com topo da pilha (op 4)
int trocarFrenteTopo(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("Impossivel trocar: fila vazia.\n");
        return 0;
    }
    if (pilhaVazia(p)) {
        printf("Impossivel trocar: pilha vazia.\n");
        return 0;
    }
    int idx_frente = f->inicio;
    // topo da pilha e p->topo - 1
    Peca temp = f->dados[idx_frente];
    f->dados[idx_frente] = p->dados[p->topo - 1];
    p->dados[p->topo - 1] = temp;
    return 1;
}

// troca 3 primeiros da fila com 3 da pilha (op 5)
int trocarTres(Fila *f, Pilha *p) {
    if (p->topo != 3) {
        printf("Impossivel trocar: pilha deve ter exatamente 3 elementos (tem %d).\n", p->topo);
        return 0;
    }
    if (f->tamanho < 3) {
        printf("Impossivel trocar: fila deve ter ao menos 3 elementos (tem %d).\n", f->tamanho);
        return 0;
    }
    for (int i = 0; i < 3; ++i) {
        int idx_fila = (f->inicio + i) % FILA_CAP;
        Peca temp = f->dados[idx_fila];
        f->dados[idx_fila] = p->dados[i];
        p->dados[i] = temp;
    }
    return 1;
}

// ----------- Interface / Menu -----------
void mostrarEstado(Fila *f, Pilha *p) {
    mostrarFila(f);
    mostrarPilha(p);
    printf("----\n");
}

int menu() {
    printf("\nMenu:\n");
    printf("1 - Jogar peça (remover da frente)\n");
    printf("2 - Enviar peça da fila para a reserva (pilha)\n");
    printf("3 - Usar peça da reserva (remover do topo da pilha)\n");
    printf("4 - Trocar peça da frente da fila com topo da pilha\n");
    printf("5 - Trocar 3 primeiros da fila com os 3 da pilha\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    int op;
    if (scanf("%d", &op) != 1) {
        // clear stdin
        int c; while ((c = getchar()) != EOF && c != '\n');
        return -1;
    }
    return op;
}

int main() {
    srand(1234); // seed fixed to make tests deterministic

    Fila fila; Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    reporFila(&fila);

    printf("=== Tetris Stack - fila (5) e pilha (3) ===\n");
    mostrarEstado(&fila, &pilha);

    int op;
    while (1) {
        op = menu();
        if (op == -1) {
            printf("Entrada invalida, tente novamente.\n");
            continue;
        }
        if (op == 0) {
            printf("Saindo...\n");
            break;
        } else if (op == 1) {
            Peca jogada;
            if (!dequeue(&fila, &jogada)) {
                printf("Nao ha peças para jogar.\n");
            } else {
                printf("Jogou peça (%c-%d).\n", jogada.tipo, jogada.id);
                Peca novo = gerarPeca(&fila);
                enqueue(&fila, novo);
            }
            mostrarEstado(&fila, &pilha);
        } else if (op == 2) {
            if (pilhaCheia(&pilha)) {
                printf("Nao foi possivel enviar: pilha cheia.\n");
            } else {
                Peca front;
                if (!dequeue(&fila, &front)) {
                    printf("Fila vazia.\n");
                } else {
                    push(&pilha, front);
                    printf("Enviou para reserva (%c-%d).\n", front.tipo, front.id);
                    Peca novo = gerarPeca(&fila);
                    enqueue(&fila, novo);
                }
            }
            mostrarEstado(&fila, &pilha);
        } else if (op == 3) {
            Peca usado;
            if (!pop(&pilha, &usado)) {
                printf("Pilha vazia: nao ha peça para usar.\n");
            } else {
                printf("Usou peça da reserva (%c-%d).\n", usado.tipo, usado.id);
            }
            mostrarEstado(&fila, &pilha);
        } else if (op == 4) {
            if (trocarFrenteTopo(&fila, &pilha)) {
                printf("Troca entre frente da fila e topo da pilha realizada.\n");
            }
            mostrarEstado(&fila, &pilha);
        } else if (op == 5) {
            if (trocarTres(&fila, &pilha)) {
                printf("Troca dos 3 primeiros da fila com os 3 da pilha realizada.\n");
            }
            mostrarEstado(&fila, &pilha);
        } else {
            printf("Opcao invalida.\n");
        }
    }

    return 0;
}
