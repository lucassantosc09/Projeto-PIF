#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para representar a posição de um ponto na tela
typedef struct {
    int x;
    int y;
} Position;

// Estrutura para representar a cobra
typedef struct {
    Position head;
    Position tail;
    int length;
} Snake;

// Estrutura para representar a comida
typedef struct {
    Position position;
} Food;

// Estrutura para representar o estado do jogo
typedef struct {
    Snake snake;
    Food food;
    int score;
} Game;

// Função para inicializar o jogo
void init_game(Game* game) {
    // Inicializar a cobra com uma posição aleatória
    game->snake.head.x = rand() % 20;
    game->snake.head.y = rand() % 20;
    game->snake.tail.x = game->snake.head.x;
    game->snake.tail.y = game->snake.head.y;
    game->snake.length = 1;

    // Inicializar a comida com uma posição aleatória
    game->food.position.x = rand() % 20;
    game->food.position.y = rand() % 20;

    // Inicializar o score
    game->score = 0;
}

// Função para atualizar o jogo
void update_game(Game* game) {
    // Verificar se a cobra comeu a comida
    if (game->snake.head.x == game->food.position.x && game->snake.head.y == game->food.position.y) {
        // Incrementar o score
        game->score++;
        // Gerar uma nova comida
        game->food.position.x = rand() % 20;
        game->food.position.y = rand() % 20;
    }

    // Verificar se a cobra bateu em uma parede
    if (game->snake.head.x < 0 || game->snake.head.x >= 20 || game->snake.head.y < 0 || game->snake.head.y >= 20) {
        // Fim do jogo
        printf("Game Over!\n");
        exit(0);
    }

    // Atualizar a posição da cobra
    game->snake.head.x++;
    game->snake.head.y++;

    // Verificar se a cobra comeu a si mesma
    if (game->snake.head.x == game->snake.tail.x && game->snake.head.y == game->snake.tail.y) {
        // Fim do jogo
        printf("Game Over!\n");
        exit(0);
    }

    // Atualizar a posição da cauda da cobra
    game->snake.tail.x = game->snake.head.x;
    game->snake.tail.y = game->snake.head.y;
}

// Função para imprimir o estado do jogo
void print_game(Game* game) {
    // Imprimir a cobra
    for (int i = 0; i < game->snake.length; i++) {
        printf("(%d, %d) ", game->snake.head.x - i, game->snake.head.y);
    }
    printf("\n");

    // Imprimir a comida
    printf("(%d, %d)\n", game->food.position.x, game->food.position.y);

    // Imprimir o score
    printf("Score: %d\n", game->score);
}

// Função para ler o estado do jogo a partir de um arquivo
void read_game(Game* game, FILE* file) {
    // Ler o score
    fscanf(file, "%d", &game->score);

    // Ler a posição da cobra
    fscanf(file, "%d %d", &game->snake.head.x, &game->snake.head.y);
    fscanf(file, "%d %d", &game->snake.tail.x, &game->snake.tail.y);

    // Ler a posição da comida
    fscanf(file, "%d %d", &game->food.position.x, &game->food.position.y);
}

// Função para escrever o estado do jogo em um arquivo
void write_game(Game* game, FILE* file) {
    // Escrever o score
    fprintf(file, "%d\n", game->score);

    // Escrever a posição da cobra
    fprintf(file, "%d %d\n", game->snake.head.x, game->snake.head.y);
    fprintf(file, "%d %d\n", game->snake.tail.x, game->snake.tail.y);

    // Escrever a posição da comida
    fprintf(file, "%d %d\n", game->food.position.x, game->food.position.y);
}

int main() {
    // Inicializar o jogo
    Game* game = malloc(sizeof(Game));
    init_game(game);

    // Loop principal do jogo
    while (1) {
        // Atualizar o jogo
        update_game(game);

        // Imprimir o estado do jogo
        print_game(game);

        // Ler a entrada do usuário
        char input;
        printf("Enter a direction (W/A/S/D): ");
        scanf(" %c", &input);

        // Atualizar a posição da cobra com base na entrada do usuário
        switch (input) {
            case 'w':
                game->snake.head.y--;
                break;
            case 'a':
                game->snake.head.x--;
                break;
            case 's':
                game->snake.head.y++;
                break;
            case 'd':
                game->snake.head.x++;
                break;
        }

        // Escrever o estado do jogo em um arquivo
        FILE* file = fopen("game.txt", "w");
        write_game(game, file);
        fclose(file);
    }

    free(game);
    return 0;
}