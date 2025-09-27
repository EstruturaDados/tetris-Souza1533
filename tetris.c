#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma peça do Tetris
typedef struct{
    char name[2];  // Nome da peça (string de 1 caractere + '\0')
    int id;        // Identificador único da peça
}Piece;

#define MAX 5  // Capacidade máxima da fila circular (quantidade de peças visíveis)

// Estrutura que representa a fila circular de peças
typedef struct{
    Piece itens [MAX]; // Vetor que armazena as peças
    int start;         // Índice do início da fila
    int end;           // Índice da próxima posição de inserção
    int total;         // Quantidade de elementos atuais na fila
    int next_id;       // Próximo ID a ser atribuído a uma peça
}Queue;

// Insere uma nova peça na fila circular
void InsertIntoQueue(Queue *q, Piece new){
    q->itens[q->end] = new;              // Insere na posição "end"
    q->end = (q->end + 1) % MAX;         // Avança o índice de forma circular

    if(q->total < MAX){
        q->total++;                      // Incrementa se ainda não chegou no limite
    } else {
        q->start = (q->start + 1) % MAX; // Se a fila está cheia, descarta a peça mais antiga
    }
}

// Gera uma peça aleatória com base em um conjunto fixo de caracteres
Piece PieceGenerator(int id){
    Piece p;
    char piece[8] = {'T', 'Z', 'U', 'I', 'O', 'P', 'L', 'R'}; // Conjunto de tipos possíveis
    p.name[0] = piece[rand() % 8];  // Sorteia uma letra da lista
    p.name[1] = '\0';               // Fecha a string
    p.id = id;                      // Define ID único
    return p;
}

// Inicializa a fila já com 5 peças aleatórias
void StartQueue(Queue *q){
    q->start = 0;
    q->end = 0;
    q->total = 0;
    q->next_id = 1;

    for(int i = 0; i < MAX; i++){
        Piece new = PieceGenerator(q->next_id++); // Gera peça com ID sequencial
        InsertIntoQueue(q, new);                  // Insere na fila
    }
}

// Mostra a fila de peças no estado atual
void UpdateQueue(Queue *q){
    int idx;
    printf("\n---------->  Fila Atual  <----------\n\n   ");
    for (int i = 0; i < q->total; i++){
        idx = (q->start + i) % MAX; // Calcula índice circular
        printf("[%s.%d] ", q->itens[idx].name, q->itens[idx].id);
    }
    printf("\n");
}

// Mostra a fila de peças destacando a próxima a ser jogada
void UpdateQueue2(Queue *q){
    int idx;
    printf("\n----------->  Fila Atual  <----------\n\n");
    printf("\n                              v\n    "); // Seta visual
    for (int i = 0; i < q->total; i++){
        idx = (q->start + i) % MAX;
        printf("[%s.%d] ", q->itens[idx].name, q->itens[idx].id);
    }
    printf("\n");
}

// Menu principal do jogo
void Menu(Queue *q){
    Piece temp;        // Peça temporária gerada mas ainda não inserida
    int created = 0;   // Flag para saber se já existe peça criada
    int menu_choice;   // Opção escolhida pelo jogador

    do{
        // Exibe opções do menu
        printf("\n----------(  Menu TETRIS  )---------\n\n");
        printf("1) Inserir nova peca no final da fila\n");
        printf("2) Jogar (remover) a peca da frente\n");
        printf("3) Visualizar estado atual da fila\n");
        printf("\n0) Sair do Jogo.\n");
        printf("------------------------------------\n\n-> ");

        scanf("%d", &menu_choice);

        switch (menu_choice){
        case 1: { // Criar uma nova peça
                temp = PieceGenerator(q->next_id++); // Gera e guarda peça temporária
                created = 1;                         // Marca que já existe peça criada
                printf("\n---------->  Peca Gerada  <---------\n");
                printf("\n             >  [%s.%d]  <\n", temp.name, temp.id);
                break;
            }
        case 2: // Inserir a peça gerada na fila
            if (created == 0){
                printf("--------------( ERRO! )-------------\n");
                printf("\nERRO 007 james bonde");
                printf("\nPeca nao gerada ainda, use a opcao 1 primeiro.\n");
            }else{
                printf("\n---------->  Jogando Peca Gerada  <---------\n");
                InsertIntoQueue(q, temp); // Adiciona peça temporária na fila
                UpdateQueue2(q);          // Mostra a fila após inserção
            }
        break;

        case 3: // Visualizar estado atual da fila
            UpdateQueue(q);
        break;
        
        default: // Tratamento para opção inválida
            if(menu_choice != 0)
                printf("\n\nX Opcao Invalida!\n");
        break;
        }
    }while(menu_choice != 0); // Loop até a opção "0 - Sair"
}

// Função principal
int main(){
    Queue q;         // Declara fila de peças
    StartQueue(&q);  // Inicializa a fila com 5 peças
    Menu(&q);        // Inicia menu interativo
}
