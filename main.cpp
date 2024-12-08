#include <iostream>
#include <raylib.h>
#include <vector>
#include <unordered_map>



const int WINDOW_WIDTH = 375;
const int WINDOW_HEIGHT = 700;
const int BLOCK_SIZE = 25;
const double FALL_SPEED=0.1;
const std::unordered_map<int,std::vector<int>> shapes = {
                                                        {1, {0, 1, 2, 3}}, 
                                                        {2, {0, 1, 2, 3}}, 
                                                        {3, {0, 1, 2, 3}}, 
                                                        {4, {0, 1, 2, 3}}, 
                                                        {5, {0, 1}},       
                                                        {6, {0, 1}},       
                                                        {7, {0}}      
                                                    };
const std::unordered_map<std::string, Color> colors = {
                                                        {"I", BLUE},       
                                                        {"O", YELLOW},     
                                                        {"T", PURPLE},     
                                                        {"S", GREEN},      
                                                        {"Z", RED},        
                                                        {"J", DARKBLUE},   
                                                        {"L", ORANGE}      
                                                    };


double lastUpdate=0;
double currentTime;


Color bgColour = {0,1,45,1};
Color whiteMask = {255,255,255,100};
bool visualise = true;




bool blockFallDelay(){
    currentTime = GetTime();
    if((currentTime-lastUpdate)>=FALL_SPEED){
        lastUpdate = currentTime;
        return true;
    }
    return false;
}

void Draw(std::vector<Rectangle>& blocks){
    for(Rectangle& block:blocks){
        DrawRectangleRec(block,WHITE);
    }

}

void visualieGrid(bool visualise){
    if(visualise)
    {        
        for(int x=0;x<WINDOW_WIDTH;x=x+BLOCK_SIZE){
            for(int y=0;y<WINDOW_HEIGHT;y+=BLOCK_SIZE){
                Rectangle cell = {
                    (float)x,
                    (float)y,
                    (float)BLOCK_SIZE,
                    (float)BLOCK_SIZE,   // since we are drawing a square width and height are the same i.e. BLOCK_SIZE
                };
                DrawRectangleLinesEx(cell,0.5,whiteMask);
            }
        }
    }
}

void updateBlocksTop(std::unordered_map<float,float>& top,std::vector<Rectangle>& blocks){
        for(Rectangle& block:blocks){
            if(((block.y+25)<=(top[block.x]))){
                block.y+=25;
            } 
            else{
                top[block.x]=block.y-25;                
            }
        }
}

void spawnBlock(std::vector<Rectangle>& blocks){
    Rectangle tempBlock;
    Vector2 mousePosition = GetMousePosition();


    tempBlock.height=BLOCK_SIZE;
    tempBlock.width=BLOCK_SIZE;

    tempBlock.x = 25 * floor(mousePosition.x/25);
    tempBlock.y = 25 * floor(mousePosition.y/25);


    blocks.insert(blocks.end(),tempBlock);
}


int main(){
    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"Tetris");
    SetTargetFPS(60);
    
    std::vector<Rectangle> blocks;
    std::unordered_map<float,float> top;
    for(int i=0;i<WINDOW_WIDTH;i=i+25){
        top[(float)i]=WINDOW_HEIGHT-25;
    }

    while(WindowShouldClose()==false){
        BeginDrawing();

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            spawnBlock(blocks);
        }


        if(blockFallDelay()){
            updateBlocksTop(top,blocks);
        }


        visualieGrid(visualise);


        Draw(blocks);

        ClearBackground(bgColour);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
