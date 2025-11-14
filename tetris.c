#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 5

/* struct territorio */
typedef struct {
    char nome[30];
    char cor[20];
    int tropas;
} Territorio;

/* remove \n */
void tiraEnter(char *s) {
    s[strcspn(s, "\n")] = 0;
}

/* imprime mapa */
void mostrar(const Territorio *t) {
    printf("\n=== MAPA ===\n");
    for (int i = 0; i < MAX; i++) {
        printf("%d) %s | %s | Tropas: %d\n",
               i+1, t[i].nome, t[i].cor, t[i].tropas);
    }
}

/* ataque */
void atacar(Territorio *a, Territorio *d) {
    int da = rand() % 6 + 1;
    int dd = rand() % 6 + 1;

    printf("\nAtaque %s(%d) x %s(%d)\n",
           a->nome, da, d->nome, dd);

    if (da >= dd) {       /* atacante vence empate */
        d->tropas--;
        printf("Defensor perdeu 1 tropa!\n");
    } else {
        printf("Defensor resistiu!\n");
    }

    if (d->tropas <= 0) {
        d->tropas = 0;
        printf("%s conquistou o territorio!\n", a->nome);
    }
}

/* verifica missao */
int missao_ok(int tipo, const Territorio *t) {
    if (tipo == 1) {
        /* destruir exercito verde */
        for (int i = 0; i < MAX; i++)
            if (strcmp(t[i].cor, "Verde") == 0 &&
                t[i].tropas > 0)
                return 0;
        return 1;
    }

    if (tipo == 2) {
        /* conquistar 3 */
        int conta = 0;
        for (int i = 0; i < MAX; i++)
            if (t[i].tropas == 0) conta++;
        return (conta >= 3);
    }

    return 0;
}

int main() {
    srand(time(NULL));

    /* NIVEL NOVATO + AVENTUREIRO:
       antes era vetor estático, agora calloc */
    Territorio *t = calloc(MAX, sizeof(Territorio));

    printf("=== CADASTRO INICIAL ===\n");
    for (int i = 0; i < MAX; i++) {
        printf("\nTerritorio %d:\n", i+1);

        printf("Nome: ");
        fgets(t[i].nome, 30, stdin);
        tiraEnter(t[i].nome);

        printf("Cor: ");
        fgets(t[i].cor, 20, stdin);
        tiraEnter(t[i].cor);

        printf("Tropas: ");
        scanf("%d", &t[i].tropas);
        getchar();
    }

    /* NIVEL MESTRE: missão aleatória */
    int missao = rand() % 2 + 1;  /* 1 ou 2 */

    /* menu */
    while (1) {
        int op;
        mostrar(t);

        printf("\nMISSAO: ");
        if (missao == 1) printf("Destruir o exercito VERDE.\n");
        else             printf("Conquistar 3 territorios.\n");

        printf("\n1 - Atacar\n2 - Verificar Missao\n0 - Sair\nOpcao: ");
        scanf("%d", &op);

        if (op == 0) break;

        if (op == 1) {
            int a, d;
            printf("Atacante (1-5): ");
            scanf("%d", &a);
            printf("Defensor (1-5): ");
            scanf("%d", &d);

            a--; d--;
            if (a >= 0 && a < MAX && d >= 0 && d < MAX)
                atacar(&t[a], &t[d]);
        }

        if (op == 2) {
            if (missao_ok(missao, t))
                printf("\n*** MISSAO CUMPRIDA! ***\n");
            else
                printf("\nAinda nao concluiu.\n");
        }
    }

    free(t);
    return 0;
}
