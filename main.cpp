#include <iostream>
#include <raylib.h>

int windowWidth = 375;
int windowHeight = 700;
int cellSize = 25;
Color bgColour = {0,1,45,1};
bool visualiseGrid = true;

int main(){
    InitWindow(windowWidth,windowHeight,"Tetris");
    SetTargetFPS(60);

    while(WindowShouldClose()==false){
        BeginDrawing();

        if(visualiseGrid)
        {        
            for(int x=0;x<windowWidth;x=x+cellSize){
                for(int y=0;y<windowHeight;y+=cellSize){
                    Rectangle cell = {
                        (float)x,
                        (float)y,
                        (float)cellSize,
                        (float)cellSize,   // since we are drawing a square width and height are the same i.e. cellsize
                    };
                    DrawRectangleLinesEx(cell,0.5,WHITE);
                }
            }
        }

        ClearBackground(bgColour);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
