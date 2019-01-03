#include <iostream>
#include <string>
using namespace std;

// Ships will be 2 3 3 4 5 long , 5 ship. 
// Each plane will be 10x10
// Legend of map:
//  1to5 ships
//  0 no information
//  -1 to -5 there is a wrecked ship
//  9 nothing here
void placeShips(int ship[], int size);
void drawMap(string message = "");
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
        int mine_ship[2]; // Code:1
        int frigate[3]; //Code:2
        int submarine[3]; //Code:3
        int cruiser[4]; //Code:4
        int airplane_carrier[5]; //Code:5
        // Map:
        int field[10][10] = {0};
        // Other informations:
        int score = 0;
    };
    drawMap("Welcome to Amiral Battı!, (p) to play, (q) to quit");
    char answ_list[] = {'p','q'};
    char answ = getAnswer(answ_list,2);
    if (answ == 'p'){
         
        Players computer;
        Players user;
        //Initializing  ships' locations
        placeShips(computer.mine_ship,2);
        placeShips(computer.frigate,3);
        placeShips(computer.submarine,3);
        placeShips(computer.cruiser,4);
        placeShips(computer.airplane_carrier,5);

        placeShips(user.mine_ship,2);
        placeShips(user.frigate,3);
        placeShips(user.submarine,3);
        placeShips(user.cruiser,4);
        placeShips(user.airplane_carrier,5);

        bool game = true;
        while (game){
            drawMap("Enter your move");
            int coor[2];
            makeMove(coor,computer.field);
            drawMap("Computer's move..");
            compMove(user.field);
            if (isDestroyed(user.field)){
                drawMap("You lost! What a shame.");
                game = false;
            }
            else if (isDestroyed(computer.field)){
                drawMap("Wow, you won. Congrulations!!");
                game = false;
            }
            else {
                continue;
            }
             
        }

    }
    else {
        drawMap("Quitting...");
        return 0;
    }


}


