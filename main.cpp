#include <iostream>
#include <raylib.h>

int windowWidth = 380;
int windowHeight = 700;
Color bg_colour = {0,1,45,1};

int main(){
    InitWindow(windowWidth,windowHeight,"Tetris");
    SetTargetFPS(60);

    while(WindowShouldClose()==false){
        BeginDrawing();
        ClearBackground(bg_colour);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}