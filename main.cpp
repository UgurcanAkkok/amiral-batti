#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
using namespace std;

struct Players {
    // Ships:
    // -1 in coordinates means it has taken damage
    int mine_ship[2][2];
    int frigate[3][2];
    int submarine[3][2];
    int cruiser[4][2];
    int airplane_carrier[5][2];
    // Map:
    // 0 -> No ship or unknown
    // 1 -> There is a ship, either destroyed or known
    // 2 -> We attacked but there was no ship
    int field[10][10] = {0};
    int game_map[10][10] = {0};
};

void placeShips(int ship[][2],int field[10][10], int size);
void drawMap(int user_field[10][10],int enemy_field[10][10],string message = "");
char getAnswer(char answ_list[], int size);
bool isDestroyed(int field[10][10]);
void getMove(int coor[2]); // Changes coor array because we cant return arrays
//Returns which ship has been taken damage,  changes has_fallen accordingly
string makeMove(int coor[],Players target, bool * has_fallen);
void compMove(int field[10][10], int coor[2]);
// First selects a random coordinate which it didnt choose already, then 
// calls makeMove() with that coor

int main(){
    Players computer;
    Players user;
    drawMap(computer.game_map,computer.game_map,"Welcome to Amiral Battı!, (p) to play, (q) to quit");
    char answ_list[] = {'p','q'};
    char answ = getAnswer(answ_list,2);
    if (answ == 'p'){
         
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
            drawMap(user.field,computer.game_map,"Enter your move");
            int coor[2];
            getMove(coor);

            bool has_fallen;
            string attacked_ship = makeMove(coor,computer,&has_fallen);
            string msg;
            if (has_fallen){
                msg = attacked_ship + " has been destroyed!";
            }
            else {
                msg = attacked_ship + " has taken damage.";
            }
            drawMap(user.field,computer.field,msg);
            drawMap(user.field,computer.game_map,"Computer's move..");

            compMove(user.game_map,coor);
            attacked_ship = makeMove(coor,user,&has_fallen);
            if (has_fallen){
                msg = "Your "+ attacked_ship + " has been destroyed!";
            }
            else {
                msg = "Your " + attacked_ship + " has taken damage.";
            }

            if (isDestroyed(user.field)){
                drawMap(user.field,computer.game_map,"You lost! What a shame.");
                game = false;
            }
            else if (isDestroyed(computer.field)){
                drawMap(user.field,computer.game_map,"Wow, you won. Congrulations!!");
                game = false;
            }
            else {
                continue;
            }
        }
    }
    else {
        drawMap(computer.game_map,computer.game_map,"Quitting...");
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
        direction = 1 + rand() % 4; // 1:+x 2:-x 3:+y 4:-y
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
            // 0 means we dont know what is there
            if (field[i][j] == 0){
                cout << '~';
            }
            // 1 means there was a ship, now destroyed
            else if (field[i][j] == 1){
                cout << 'x';
            }
            // 2 means attacked that coor but there was nothing
            else if (field[i][j] == 2){
                cout << '#';
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
    cout << endl << "-"<<"[Continue]";
    cin >> garbage;
}

char getAnswer(char ans_list[], int size){
    char ans;
    bool isValid = 0;
    while (!isValid){
        clearScreen();
        cin >> ans;
        for (int i = 0; i < size; i++){
            if (ans_list[i] == ans){
                isValid = 1;
            }
        }
        if (isValid) {
            return tolower(ans);
        }
    }
    return 1;
}

bool isDestroyed(int field[10][10]){
    bool destroyed = true;
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            if (field[i][j] == 1){
                destroyed = false;
            }
        }
    }
    return destroyed;
}

void getMove(int coor[2]){
    bool isValid = false;
    int coor_x, coor_y;
    do {
        clearScreen();
        cin >> coor_x >> coor_y;
        if (coor_x > 10 or coor_x < 0 or coor_y > 10 or coor_y < 0){
            isValid = false;
        }
        else {
            isValid = true;
        }
    }while (isValid == false);

    coor[0] = coor_x;
    coor[1] = coor_y;

}

// Returns whether attack succeed
bool attack(int ship[][2], int size, int x, int y, bool * has_fallen){
    bool success = false;
    *has_fallen = true;
    for (int i = 0; i < size; i++){
        if (ship[i][0] == x and ship[i][1] == y){
            success = true;
            ship[i][0] = -1;
            ship[i][1] = -1;
        }
        if (ship[i][0] != -1 and ship[i][1] != -1){
            *has_fallen = false;
        }
    }
    return success;
}

string makeMove(int coor[], Players target,bool * has_fallen ){
    string damaged_ship;
    int x = coor[0];
    int y = coor[1];

    // If there is a ship at that coordinate
    if (target.field[x][y] == 1){
        target.field[x][y] = 0;
        target.game_map[x][y] = 1;
        if (attack(target.mine_ship, 2, x, y, has_fallen)){
            damaged_ship = "Mine ship";
        }
        if (attack(target.frigate, 3, x, y, has_fallen)){
            damaged_ship = "Frigate";
        }
        if (attack(target.submarine, 3, x, y, has_fallen)){
            damaged_ship = "Submarine";
        }
        if (attack(target.cruiser, 4, x, y, has_fallen)){
            damaged_ship = "Cruiser";
        }
        if (attack(target.airplane_carrier, 5, x, y, has_fallen)){
            damaged_ship = "Airplane carrier";
        }
        return damaged_ship;
    }
    else {
        damaged_ship = "No ship";
        *has_fallen = false;
        target.game_map[x][y] = 2;
        return damaged_ship;
    }
}

void compMove(int user_map[10][10], int coor[2]){
    srand(time(NULL));
    int x,y;
    do{
        x = rand() % 10;
        y = rand() % 10;
    }while(user_map[x][y] == 2);
    coor[0] = x;
    coor[1] = y;
}

