#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma peça do Tetris
typedef struct {
    char name[2];  // Nome da peça (string de 1 caractere + '\0')
    int id;        // Identificador único da peça
} Piece;

#define MAX 5  // Capacidade máxima da fila circular (quantidade de peças visíveis)

// Estrutura que representa a fila circular de peças
typedef struct {
    Piece itens[MAX];  // Vetor que armazena as peças da fila
    int start;         // Índice do início da fila (primeira peça disponível)
    int end;           // Índice da próxima posição de inserção no fim da fila
    int total;         // Quantidade de peças atualmente na fila
    int next_id;       // Próximo ID único a ser atribuído a uma nova peça
} Queue;

// Estrutura que representa a pilha de peças reservadas
typedef struct {
    Piece itens_r[MAX]; // Vetor que armazena as peças reservadas
    int top;            // Índice do topo da pilha (última peça reservada)
} Pile;

// Insere uma nova peça no fim da fila circular
void InsertIntoQueue(Queue *q, Piece new) {
    q->itens[q->end] = new;                 // Insere a peça na posição 'end'
    q->end = (q->end + 1) % MAX;            // Incrementa 'end' circularmente

    if (q->total < MAX) {
        q->total++;                          // Aumenta total se não estiver cheia
    } else {
        q->start = (q->start + 1) % MAX;    // Se estiver cheia, move 'start' (descarta a peça mais antiga)
    }
}

// Gera uma peça aleatória com um ID único
Piece PieceGenerator(int id) {
    Piece new;
    char piece[8] = {'T', 'Z', 'U', 'I', 'O', 'P', 'L', 'R'}; // Tipos de peças disponíveis
    new.name[0] = piece[rand() % 8];  // Escolhe uma peça aleatória
    new.name[1] = '\0';               // Finaliza a string
    new.id = id;                       // Define o ID da peça
    return new;
}

// Inicializa a fila e a pilha de reservas
void Start(Queue *q, Pile *p) {
    q->start = 0;
    q->end = 0;
    q->total = 0;
    q->next_id = 1;  // Começa IDs a partir de 1
    p->top = -1;     // Pilha inicialmente vazia

    // Preenche a fila com peças iniciais
    for (int i = 0; i < MAX; i++) {
        Piece new = PieceGenerator(q->next_id++);
        InsertIntoQueue(q, new);
    }

    // Pilha inicia com 2 peças
    for (int i = 0; i < 2; i++) {
        Piece new = PieceGenerator(q->next_id++);
        p->itens_r[++p->top] = new;
    }
}

// Exibe o estado atual da fila e da pilha
void UpdateQueue(Queue *q, Pile *p) {
    printf("\n========== ESTADO ATUAL ==========\n");

    // Exibe fila
    printf("\nFila de Pecas (Frente -> Fim):\n   ");
    for (int i = 0; i < q->total; i++) {
        int idx = (q->start + i) % MAX;  // Calcula índice circular
        printf("[%s %d] ", q->itens[idx].name, q->itens[idx].id);
    }

    // Exibe pilha
    printf("\n\nPilha de Reserva (Topo -> Base):\n   ");
    for (int i = p->top; i >= 0; i--) {   // Do topo para a base
        printf("[%s %d] ", p->itens_r[i].name, p->itens_r[i].id);
    }

    printf("\n=================================\n");
}

// Reserva a última peça da fila, enviando-a para a pilha de reservas
void ReservePiece(Queue *q, Pile *p) {
    if (p->top >= MAX - 1) {  // Verifica se a pilha está cheia
        printf("\nA pilha de reservas esta cheia!\n");
        return;
    }
    if (q->total == 0) {      // Verifica se há peça na fila
        printf("\nNao ha peca na fila para reservar!\n");
        return;
    }

    int lastIndex = (q->end - 1 + MAX) % MAX; // Calcula índice da última peça
    p->itens_r[++p->top] = q->itens[lastIndex]; // Move para a pilha

    q->end = lastIndex;  // Remove da fila
    q->total--;          // Atualiza total

    Piece new = PieceGenerator(q->next_id++);  // Gera nova peça
    InsertIntoQueue(q, new);                   // Adiciona ao fim da fila
}

// Usa uma peça reservada, inserindo-a no fim da fila e removendo a primeira
void UseReservedPiece(Queue *q, Pile *p) {
    if (p->top < 0) {  // Verifica se há peças reservadas
        printf("\nNenhuma peca reservada disponivel.\n");
        return;
    }

    q->itens[q->end] = p->itens_r[p->top--]; // Move topo da pilha para a fila
    q->end = (q->end + 1) % MAX;             // Incrementa fim circularmente
    if (q->total < MAX) q->total++;          // Atualiza total

    q->start = (q->start + 1) % MAX;         // Remove a primeira peça da fila
}

// Troca a peça do topo da pilha com a da frente da fila
void ExchangePiece(Queue *q, Pile *p) {
    if (p->top < 0) {  // Pilha vazia
        printf("\nNao há peca na pilha para trocar!\n");
        return;
    }
    if (q->total == 0) { // Fila vazia
        printf("\nNao ha peca na fila para trocar!\n");
        return;
    }

    Piece temp = p->itens_r[p->top];  // Guarda peça da pilha
    p->itens_r[p->top] = q->itens[q->start]; // Troca com peça da fila
    q->itens[q->start] = temp;

    printf("\nTroca realizada entre o topo da pilha e a frente da fila!\n");
}

// Troca as 3 primeiras peças da fila com as 3 do topo da pilha
void Exchange3(Queue *q, Pile *p){
    if (p->top >= 2){ // Verifica se há pelo menos 3 peças na pilha
        for (int i = 0; i < 3; i++){
            Piece temp = p->itens_r[p->top - i];      // Acessa do topo para baixo
            p->itens_r[p->top - i] = q->itens[q->start + i]; // Troca com fila
            q->itens[q->start + i] = temp;           // Finaliza troca
        }
    } else {
        printf("\nE preciso ter pelomenos 3 pecas na pilha primeiro.\n"); // Mensagem de erro
    }
}

// Menu principal do jogo
void Menu(Queue *q, Pile *p) {
    int menu_choice;
    Piece temp;

    do {
        UpdateQueue(q, p); // Exibe estado atual

        printf("\n---------- MENU TETRIS ----------\n");
        printf("1) Jogar nova peca (gera e adiciona)\n");
        printf("2) Reservar peca da fila\n");
        printf("3) Usar peca reservada\n");
        printf("4) Trocar topo da pilha com frente da fila\n");
        printf("5) Trocar 3 primeiros da fila por 3 primeiros da pilha\n");
        printf("0) Sair\n");
        printf("--------------------------------\n-> ");
        scanf("%d", &menu_choice);

        switch (menu_choice) {
            case 1:
                printf("\nJogando nova peca...\n");
                temp = PieceGenerator(q->next_id++);
                InsertIntoQueue(q, temp);
                break;
            case 2:
                ReservePiece(q, p);
                break;
            case 3:
                UseReservedPiece(q, p);
                break;
            case 4:
                ExchangePiece(q, p);
                break;
            case 5:
                Exchange3(q, p);
                break;
            case 0:
                printf("\nSaindo...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                break;
        }
    } while (menu_choice != 0);
}

int main() {
    Queue q;
    Pile p;

    Start(&q, &p);  // Inicializa fila e pilha
    Menu(&q, &p);   // Executa menu interativo
    return 0;
}
