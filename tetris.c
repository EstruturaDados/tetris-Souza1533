#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma peça do Tetris
typedef struct {
    char name[2];  // Nome da peça (string de 1 caractere + '\0')
    int id;        // Identificador único da peça
} Piece;

#define MAX 5  // Capacidade máxima da fila circular (quantidade de peças visíveis)

// Estrutura que representa a fila circular de peças (próximas peças do jogo)
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
    q->itens[q->end] = new;              // Insere peça na posição "end"
    q->end = (q->end + 1) % MAX;         // Avança o índice de forma circular

    if (q->total < MAX) {
        q->total++;                      // Incrementa a contagem se houver espaço
    } else {
        q->start = (q->start + 1) % MAX; // Se cheia, descarta a peça mais antiga (início da fila)
    }
}

// Gera uma peça aleatória com um ID único
Piece PieceGenerator(int id) {
    Piece new;
    char piece[8] = {'T', 'Z', 'U', 'I', 'O', 'P', 'L', 'R'}; // Tipos de peças possíveis
    new.name[0] = piece[rand() % 8];  // Seleciona aleatoriamente uma letra
    new.name[1] = '\0';               // Finaliza string
    new.id = id;                      // Define ID da peça
    return new;
}

// Inicializa a fila de peças e a pilha de reservas
void Start(Queue *q, Pile *p) {
    q->start = 0;
    q->end = 0;
    q->total = 0;
    q->next_id = 1; // Começa IDs a partir de 1
    p->top = -1;    // Pilha inicialmente vazia

    // Preenche a fila com MAX peças iniciais
    for (int i = 0; i < MAX; i++) {
        Piece new = PieceGenerator(q->next_id++);
        InsertIntoQueue(q, new);
    }
}

// Exibe o estado atual da fila e da pilha de reservas
void UpdateQueue(Queue *q, Pile *p) {
    int idx;
    printf("\n---------->  Fila Atual  <----------\n\n   ");
    
    // Mostra todas as peças da fila em ordem
    for (int i = 0; i < q->total; i++) {
        idx = (q->start + i) % MAX; // Índice circular da peça
        printf("[%s.%d] ", q->itens[idx].name, q->itens[idx].id);
    }

    // Mostra as peças reservadas (pilha)
    printf("\n\n           ( Reservadas )\n\n   ");
    for (int i = 0; i <= p->top; i++) {
        printf("[%s,%d] ", p->itens_r[i].name, p->itens_r[i].id);
    }
    printf("\n");
}

// Reserva a última peça da fila, enviando-a para a pilha de reservas
void ReservePiece(Queue *q, Pile *p) {
    if (p->top < MAX - 1) { // Verifica se há espaço na pilha

        int lastIndex = (q->end - 1 + MAX) % MAX; // último índice válido da fila
        p->itens_r[++p->top] = q->itens[lastIndex]; // move para topo da pilha

        // Remove a peça da fila
        q->end = lastIndex;
        q->total--;


        // Gera uma nova peça e insere no fim da fila
        Piece new = PieceGenerator(q->next_id++);
        InsertIntoQueue(q, new);

    } else {
        printf("\nA pilha de reservas esta cheia!\n");
    }
}

// Usa uma peça reservada, inserindo-a no fim da fila e removendo a primeira da fila
void UseReservedPiece(Queue *q, Pile *p) {
    q->itens[q->end] = p->itens_r[p->top]; // Insere peça reservada no fim da fila
    p->top--; // Remove do topo da pilha

    q->end = (q->end + 1) % MAX; // Avança posição de inserção
    q->total++;

    // Mantém a fila no tamanho máximo removendo a primeira peça
    q->start = (q->start + 1) % MAX;
    q->total--;
}

// Exibe menu principal do jogo
void Menu(Queue *q, Pile *p) {
    Piece temp;        // Peça temporária
    int menu_choice;   // Opção escolhida pelo jogador

    do {
        // Atualiza estado da fila e pilha
        UpdateQueue(q, p);
        
        // Exibe opções
        printf("\n----------(  Menu TETRIS  )---------\n\n");
        printf("1) Jogar peca\n");
        printf("2) Reservar peca\n");
        printf("3) Usar peca reservada\n");
        printf("\n0) Sair.\n\n-> ");
        scanf("%d", &menu_choice);
        printf("------------------------------------\n\n");

        switch (menu_choice) {
        case 1: // Jogar uma peça normal
            printf("\nJogando peca...\n");
            temp = PieceGenerator(q->next_id++); // Gera nova peça
            InsertIntoQueue(q, temp);            // Adiciona no fim da fila
            break;

        case 2: // Reservar peça
            ReservePiece(q, p);
            break;

        case 3: // Usar peça reservada
            if (p->top >= 0) {
                UseReservedPiece(q, p);
            } else {
                printf("\nNenhuma peca reservada disponivel.\n");
            }
            break;
        
        default: // Opção inválida
            if (menu_choice != 0)
                printf("\n\n Opcao invalida!\n");
            break;
        }
    } while (menu_choice != 0); // Continua até escolher "0 - Sair"
}

// Função principal
int main() {
    Queue q;
    Pile p;

    // Inicializa a fila e a pilha
    Start(&q, &p);

    // Executa o menu interativo
    Menu(&q, &p);
}
