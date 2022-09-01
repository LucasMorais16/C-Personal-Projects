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

void randomPointSpots(){
    char point = 'o';
    int i, j, numOfPoints;

    if(M>=N){
        numOfPoints = M/4;

    }else{
        numOfPoints = N/4;
    }

    for(int a = 1; a<=numOfPoints; a++){

        i = rand()% M;
        j = rand()% N;

        if(map[i][j] != ' '){
            do{
                i = rand()% M;
                j = rand()% M;

            }while(map[i][j] != ' ');
        }
        map[i][j] = point;
    }
}

int main(){
    int x, y;
    char ready, keyboard1, keyboard2;
    srand(time(NULL));
    gameOpening();
    printf("Ready?(Y/N): ");
    scanf("%c", &ready);

    if(ready == 'Y' || ready == 'y'){
        gameMap();
        randomPointSpots();
        printMap();
        x = (M/2)-1; y = (N/2)-1; //initial coordinates

        map[x][y] = 'S';
        while(checkCollision() != 1){
            system("cls");
            printMap();

            if(_kbhit()){
                switch(_getch()){
                    case 'w':
                        if(keyboard1 != 's' && keyboard1 != 'w'){
                            keyboard1 = 'w';
                            goto W;
                        }
                        break;
                    case 'a':
                        if(keyboard1 != 'd' && keyboard1 != 'a'){
                            keyboard1 = 'a';
                            goto A;
                        }
                        break;
                    case 's':
                        if(keyboard1 != 'w' && keyboard1 != 's'){
                            keyboard1 = 's';
                            goto S;
                        }
                        break;
                    case 'd':
                        if(keyboard1 != 'a' && keyboard1 != 'd'){
                            keyboard1 = 'd';
                            goto D;
                        }
                        break;
                }
            }else{
                if(keyboard1 == 'w'){
                    W:
                    map[x][y] = ' ';
                    x--;
                    map[x][y] = 'S';
                }
                if(keyboard1 == 'a'){
                    A:
                    map[x][y] = ' ';
                    y--;
                    map[x][y] = 'S';
                }
                if(keyboard1 == 's'){
                    S:
                    map[x][y] = ' ';
                    x++;
                    map[x][y] = 'S';
                }
                if(keyboard1 == 'd'){
                    D:
                    map[x][y] = ' ';
                    y++;
                    map[x][y] = 'S';
                }
            }
            Sleep(100);
        }

    }

    return 0;
}
