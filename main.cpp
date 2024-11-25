#include <iostream>
#include <raylib.h>

int windowWidth = 375;
int windowHeight = 700;
int cellSize = 25;
double lastUpdate=0;
double currentTime;
double fallSpeed=0.1;
Color bgColour = {0,1,45,1};
Color whiteMask = {255,255,255,100};
bool visualiseGrid = true;




bool blockFallDelay(){
    currentTime = GetTime();
    if((currentTime-lastUpdate)>=fallSpeed){
        lastUpdate = currentTime;
        return true;
    }
    return false;
}

int main(){
    InitWindow(windowWidth,windowHeight,"Tetris");
    SetTargetFPS(60);

    Rectangle block = {
        0.0f,
        0.0f,
        25,
        25
    };


    while(WindowShouldClose()==false){
        BeginDrawing();

        if(blockFallDelay()){
            if((block.y+25)<=(windowHeight-25)) block.y+=25;
        }

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
                    DrawRectangleLinesEx(cell,0.5,whiteMask);
                }
            }
        }

        DrawRectangleRec(block,WHITE);

        ClearBackground(bgColour);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
