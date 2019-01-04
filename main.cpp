#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
using namespace std;

// Ships will be 2 3 3 4 5 long , 5 ship. 
// Each field will be 10x10
// Legend of map:
//  1to5 ships
//  0 no information
//  -1 to -5 there is a wrecked ship
//  9 nothing here
void placeShips(int ship[][2],int field[10][10], int size);
void drawMap(int user_field[10][10],int enemy_field[10][10],string message = "");
char getAnswer(char answ_list[], int size);
bool isDestroyed(int field[10][10]);
void getMove(int coor[]); // Changes coor because we cant return arrays
int makeMove(int coor[],int field[10][10]);
//MakeMove: if there is a ship at coor;
//              see which ship is that, change the map accordingly, 
//              drawMap(wrecked ship's name) and waits for any input,
//              score++
//          else:
//              change the map so that player can see there is nothing.
void compMove(int field[10][10]);
// First selects a random coordinate which it didnt choose already, then 
// calls makeMove() with that coor

int main(){
    struct Players {
        // Ships:
        int mine_ship[2][2];
        int frigate[3][2];
        int submarine[3][2];
        int cruiser[4][2];
        int airplane_carrier[5][2];
        // Map:
        int field[10][10] = {0};
        // Other informations:
        int score = 0;
    };
    int enemy_field[10][10] = {0};
    drawMap(enemy_field,enemy_field,"Welcome to Amiral Battı!, (p) to play, (q) to quit");
    char answ_list[] = {'p','q'};
    char answ = getAnswer(answ_list,2);
    if (answ == 'p'){
         
        Players computer;
        Players user;
        //Initializing  ships' locations
        placeShips(computer.mine_ship,computer.field,2);
        placeShips(computer.frigate,computer.field,3);
        placeShips(computer.submarine,computer.field,3);
        placeShips(computer.cruiser,computer.field,4);
        placeShips(computer.airplane_carrier,computer.field,5);

        placeShips(user.mine_ship,user.field,2);
        placeShips(user.frigate,user.field,3);
        placeShips(user.submarine,user.field,3);
        placeShips(user.cruiser,user.field,4);
        placeShips(user.airplane_carrier,user.field,5);

        bool game = true;
        while (game){
            drawMap(user.field,enemy_field,"Enter your move");
            int coor[2];
            getMove(coor);
            makeMove(coor,computer.field);
            drawMap(user.field,enemy_field,"Computer's move..");
            compMove(user.field);
            if (isDestroyed(user.field)){
                drawMap(user.field,enemy_field,"You lost! What a shame.");
                game = false;
            }
            else if (isDestroyed(computer.field)){
                drawMap(user.field,enemy_field,"Wow, you won. Congrulations!!");
                game = false;
            }
            else {
                continue;
            }
        }
    }
    else {
        drawMap(enemy_field,enemy_field,"Quitting...");
        return 0;
    }
}

void placeShips(int ship[][2],int field[10][10],int size){
    srand(time(0));
    int x_head = rand() % 10;
    int y_head = rand() % 10;
    // check if we can place the ship in that location
    int direction;
    bool right_place = false;
    while (!right_place){ 
        direction = rand() % 4; // 1:+x 2:-x 3:+y 4:-y
        right_place = true;
        switch (direction){
            case 1:
                for (int i = 0; i < size; i++){
                    if (x_head + i >= 10){
                        right_place = false;
                    }
                    if (field[x_head + i][y_head] != 0){
                        right_place = false;
                    }
                }
                if (right_place) {
                    for (int i = 0; i < size; i++){
                        field[x_head + i][y_head] = 1;
                        ship[i][0] = x_head + i;
                        ship[i][1] = y_head;
                    }
                }
                break;
            case 2: 
                for (int i = 0; i < size; i++){
                    if (x_head - i < 0) {
                        right_place = false;
                    }
                    if (field[x_head - i][y_head] != 0){
                        right_place = false;
                    }
                }
                if (right_place) {
                    for (int i = 0; i < size; i++){
                        field[x_head - i][y_head] = 1;
                        ship[i][0] = x_head - i;
                        ship[i][1] = y_head;
                    }
                }
                break;
            case 3:
                for (int i = 0; i < size; i++){
                    if (y_head + i < 10) {
                        right_place = false;
                    }
                    if (field[x_head][y_head + i] != 0){
                        right_place = false;
                    }
                }
                if (right_place) {
                    for (int i = 0; i < size; i++){
                        field[x_head][y_head + i] = 1;
                        ship[i][0] = x_head;
                        ship[i][1] = y_head + i;
                    }
                }
                break;
            case 4:
                for (int i = 0; i < size; i++){
                    if (y_head - i < 0) {
                        right_place = false;
                    }
                    if (field[x_head][y_head - i] != 0){
                        right_place = false;
                    }
                }
                if (right_place) {
                    for (int i = 0; i < size; i++){
                        field[x_head][y_head - i] = 1;
                        ship[i][0] = x_head;
                        ship[i][1] = y_head - i;
                    }
                }
                break;
        }
        if (!right_place){
            x_head = rand() % 10;
            y_head = rand() % 10;
        }
    }
}

void clearScreen(){
    for (int i = 0; i < 100;i++){
        cout << "\n";
    }
}

void drawField(int field[10][10]){
    for (int i = 0; i < 10; i++){
        cout << "   ";
        for (int j = 0; j < 10; j++){
            if (field[i][j] == 0){
                cout << '~';
            }
            else if (field[i][j] == 1){
                cout << 'x';
            }
        }
        cout << endl;
    }
}

void drawMap(int user_field[10][10],int enemy_field[10][10],string msg){
    clearScreen();
    drawField(enemy_field);
    drawField(user_field);
    string garbage;
    for (int i = 0; i < 80; i++){
        cout << "-";
    }
    cout << endl << "-";
    cout << endl << "-" << msg;
    cout << endl << "-";
    cin >> garbage;
}
