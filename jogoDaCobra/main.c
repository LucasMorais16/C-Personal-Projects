#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#define M 15 //
#define N 31

char snake = 'S';
char map[M][N];

void gameOpening(){
    printf("//-----------------\\\\\n");
    printf("|----Snake Game-----|\n");
    printf("\\\\-----------------//\n\n");
    printf("By: Lucas Morais\n");
    Sleep(000);
}

void gameMap(){
    for(int i = 0; i<M; i++){
        for(int j = 0; j<N; j++){
            if(j == 0 || j == N - 1){
                map[i][j] = '#';
            }
            if((i == 0 && (j != 0 || j != N - 1)) || (i == M - 1 && (j != 0 || j != N - 1))){
                map[i][j] = '#';
            }
            if(i != 0 && i != M - 1 && j != 0 && j != N - 1){
                map[i][j] = ' ';
            }
        }
    }
}

void printMap(){
     for(int i = 0; i<M; i++){
        for(int j = 0; j<N; j++){
            printf("%c", map[i][j]);
            if(j == N - 1){
                printf("\n");
            }
        }
    }

}

int checkCollision(){
    for(int i = 0; i<M; i++){
        for(int j = 0; j<N; j++){
            if((map[i][0] != '#' || map[i][N-1] != '#') || (map[0][j] != '#' || map[M-1][j] != '#')){
                return 1;
            }
        }
    }
}

int main(){
    int x, y;
    char ready, keyboard;

    gameOpening();
    printf("Ready?(Y/N): ");
    scanf("%c", &ready);

    if(ready == 'Y' || ready == 'y'){
        gameMap();
        printMap();
        x = (M/2)-1; y = (N/2)-1; //initial coordinates

        map[x][y] = 'S';
        while(checkCollision() != 1){
            system("cls");
            printMap();

            if(_kbhit()){
                switch(_getch()){
                    case 'w':
                        map[x][y] = ' ';
                        x--;
                        map[x][y] = 'S';
                        keyboard = 'w';
                        break;
                    case 'a':
                        map[x][y] = ' ';
                        y--;
                        map[x][y] = 'S';
                        keyboard = 'a';
                        break;
                    case 's':
                        map[x][y] = ' ';
                        x++;
                        map[x][y] = 'S';
                        keyboard = 's';
                        break;
                    case 'd':
                        map[x][y] = ' ';
                        y++;
                        map[x][y] = 'S';
                        keyboard = 'd';
                        break;
                }
            }else{
                if(keyboard == 'w'){
                    map[x][y] = ' ';
                    x--;
                    map[x][y] = 'S';
                }
                if(keyboard == 'a'){
                    map[x][y] = ' ';
                    y--;
                    map[x][y] = 'S';
                }
                if(keyboard == 's'){
                    map[x][y] = ' ';
                    x++;
                    map[x][y] = 'S';
                }
                if(keyboard == 'd'){
                    map[x][y] = ' ';
                    y++;
                    map[x][y] = 'S';
                }
            }
            Sleep(50);
        }

    }

    return 0;
}
