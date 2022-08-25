#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

char tabela[3][3];
const char PLAYER = 'X';
const char PLAYER2 = 'O';

void resetaTabela(){
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            tabela[i][j] = ' ';
        }
    }
}

int espacosVazios(){
    int espacos = 9;

    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            if(tabela[i][j] != ' '){
                espacos--;
            }
        }
    }
    return espacos;
}

char verificarGanhador(){
    //linhas
    for(int i = 0; i<3; i++){
        if(tabela[i][0] == tabela[i][1] && tabela[i][0] == tabela[i][2]){
            return tabela[i][0];
        }
    }
    //colunas
    for(int i = 0; i<3; i++){
        if(tabela[0][i] == tabela[1][i] && tabela[0][i] == tabela[2][i]){
            return tabela[0][i];
        }
    }
    //diagonais
    if(tabela[0][0] == tabela[1][1] && tabela[0][0] == tabela[2][2]){
            return tabela[0][0];
    }
    if(tabela[0][2] == tabela[1][1] && tabela[0][2] == tabela[2][0]){
            return tabela[0][2];
    }
    return ' ';
}

void jogadaPlayer(){
    int x, y;
    printf("[P1]Qual posicao voce deseja?(linha(1->3) e coluna(1->3))\n");
    fflush(stdin);

    do{
        scanf("%d %d", &x, &y);
        x--; y--;
        if(tabela[x][y] == ' '){
            tabela[x][y] = PLAYER;
            escreveTabela();
            break;
        }else{
            printf("Posicao ja preenchida, escolha uma posicao valida! (linha(1->3) e coluna(1->3))\n");
            fflush(stdin);
        }

    }while(tabela[x][y] != ' ');
}

void jogadaPlayer2(){
    int x, y;
    printf("[P2]Qual posicao voce deseja?(linha(1->3) e coluna(1->3))\n");
    fflush(stdin);

    do{
        scanf("%d %d", &x, &y);
        x--; y--;
        if(tabela[x][y] == ' '){
            tabela[x][y] = PLAYER2;
            escreveTabela();
            break;
        }else{
            printf("Posicao ja preenchida, escolha uma posicao valida! (linha(1->3) e coluna(1->3))\n");
            fflush(stdin);
        }

    }while(tabela[x][y] != ' ');

    //system("pause");
}

void resultado(char ganhador){ //declara-se a variável carregada para dentro, para poder utilizá-la.
    escreveTabela();
    if(ganhador == PLAYER){
        printf("\nJogador 1 venceu!");
    }else if(ganhador == PLAYER2){
        printf("\nJogador 2 venceu!");
    }else{
        printf("\nEmpate!");
    }

}

void jogadas(){
    char ganhador = ' ';

    do{

        jogadaPlayer();
        ganhador = verificarGanhador();
        if(ganhador != ' ' || espacosVazios() == 0){
            break;
        }

        jogadaPlayer2();
        ganhador = verificarGanhador();
        if(ganhador != ' ' || espacosVazios() == 0){
            break;
        }
    }while(ganhador == ' ' && espacosVazios() != 0);

    resultado(ganhador); //coloca-se a variável local que deseja carregar dentro do argumento dessa outra função destinada.
}

void escreveTabela(){
    system("cls");
    printf(" %c | %c | %c \n", tabela[0][0], tabela[0][1], tabela[0][2]);
    printf("--------------\n");
    printf(" %c | %c | %c \n", tabela[1][0], tabela[1][1], tabela[1][2]);
    printf("--------------\n");
    printf(" %c | %c | %c \n", tabela[2][0], tabela[2][1], tabela[2][2]);
}

int main(){
    char loop;
    do{
        resetaTabela();
        escreveTabela();

        jogadas();

        printf("\nDeseja jogar novamente?(S/N)\n");
        fflush(stdin);
        scanf("%c", &loop);
    }while(loop == 'S' || loop == 's');
return 0;
}
