#include <iostream>
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <cstdlib>  



const int WINDOW_WIDTH = 375;
const int WINDOW_HEIGHT = 700;
const int BLOCK_SIZE = 25;
const double Y_SPEED=0.5;
const double X_SPEED=0.5;
const std::unordered_map<int,std::vector<int>> SHAPES = {
                                                        {1, {0, 1, 2, 3}}, 
                                                        {2, {0, 1, 2, 3}}, 
                                                        {3, {0, 1, 2, 3}}, 
                                                        {4, {0, 1}}, 
                                                        {5, {0, 1}},       
                                                        {6, {0, 1}},       
                                                        {7, {0}}      
                                                    };
const std::unordered_map<int, Color> COLORS = {
                                                        {0, BLUE},       
                                                        {1, YELLOW},     
                                                        {2, PURPLE},     
                                                        {3, GREEN},      
                                                        {4, RED},        
                                                        {5, DARKBLUE},   
                                                        {6, ORANGE}      
                                                    };

struct Block{
    Color color;
    int x, y;
};

float Scale25(float num){
    return (25 * floor(num/25));
}

class Shape{
    public:
        std::vector<Block> blocks;
        float mid;
        float max;
        float min;
        Color color;
        int shape;
        int rotation;

        Shape(){
            shape = 4;
            rotation = 0;
            mid = Scale25(WINDOW_WIDTH/2);
            color = COLORS.at(rand() % 7);
            min = INT_MAX;
            max = INT_MIN;
            if(shape==4 && rotation==0){
                float xStart = mid - 50;

                for(int i=0;i<4;i++){                    
                    Block b;
                    b.x = xStart+(i*25);
                    if (b.x>max)    max = b.x;
                    if (b.x<min)    min = b.x;
                    b.y = 25.0;
                    b.color = color;
                    blocks.push_back(b);
                }
            }
        }

};


double lastUpdate=0;
double currentTime;





Color bgColour = {0,1,45,1};
Color whiteMask = {255,255,255,100};

class Blocks{
    public:
        Shape activeShape;
        std::unordered_map<int,std::unordered_map<int,Color>> staticBlocks;
        std::unordered_map<float,float> top;

        Blocks(){
            for(int i=0;i<WINDOW_WIDTH;i=i+25){
                top[(float)i]=WINDOW_HEIGHT-25;
            }
        }

        void spawnBlock(){

            activeShape = Shape();

        }        

        bool blockMovementXDelay(){
            currentTime = GetTime();
            if((currentTime-lastUpdate)>=X_SPEED){
                lastUpdate = currentTime;
                return true;
            }
            return false;
        }



        bool blockMovementYDelay(){
            currentTime = GetTime();
            if((currentTime-lastUpdate)>=Y_SPEED){
                lastUpdate = currentTime;
                return true;
            }
            return false;
        }

        void Draw(){
            for (auto& cell: activeShape.blocks){
                Rectangle block = {
                    (float)cell.x,
                    (float)cell.y,
                    (float)BLOCK_SIZE,
                    (float)BLOCK_SIZE,  
                };        
                DrawRectangleRec(block,cell.color);
            }
            for(auto& cellR:staticBlocks){
                for(auto& cellC: cellR.second){
                    Rectangle block = {
                        (float)cellR.first,
                        (float)cellC.first,
                        (float)BLOCK_SIZE,
                        (float)BLOCK_SIZE,  
                    };
                    DrawRectangleRec(block,cellC.second);
                }
            }

            Vector2 mousePosition = GetMousePosition();
            Rectangle block = {
                (float)Scale25(mousePosition.x),
                (float)Scale25(mousePosition.y),
                (float)BLOCK_SIZE,
                (float)BLOCK_SIZE,  
            };        
            DrawRectangleRec(block,WHITE);
            float updateX = Scale25(mousePosition.x - activeShape.mid);
            float begin = activeShape.min;
            float nextBegin = begin + updateX;
            float end = activeShape.max;
            float nextEnd = end + updateX;            
            std::string displayText = "Min: " + std::to_string(activeShape.min) + 
                                    ", Max: " + std::to_string(activeShape.max);
            DrawText(((nextBegin<0) || (nextEnd>WINDOW_WIDTH)) ? "true" : "false", 20, 35, 30, WHITE);                                    
            DrawText(displayText.c_str(), 20, 20, 15, WHITE);
        }

        void justFall(std::unordered_map<float,float>& top,std::vector<Block> blocksToUpdate){
            activeShape.blocks.clear();
            for(Block& block: blocksToUpdate){

                float nextY = block.y + 25;

                if(nextY <= top[block.x]){
                    block.y = nextY;                
                    activeShape.blocks.push_back(block);
                }
                else {
                    staticBlocks[block.x][block.y] = block.color;
                    top[block.x] = block.y-25;
                    continue;
                }
            }
        }
        void updateBlocksTop(std::unordered_map<float,float>& top){

            // Create a copy of activeShape to iterate over
            std::vector<Block> blocksToUpdate = activeShape.blocks;
            if (activeShape.blocks.empty()) {
                std::cout << "activeShape.blocks is empty!" << std::endl;
                return;
            }            
            activeShape.blocks.clear();

            Vector2 mousePosition = GetMousePosition();
            float updateX = Scale25(mousePosition.x - activeShape.mid);
            float begin = activeShape.min;
            float nextBegin = begin + updateX;

            float end = activeShape.max;
            float nextEnd = end + updateX;


            if ((nextBegin<0) || (nextEnd>(WINDOW_WIDTH-25))){
                updateX=0;
            }
            float sumX=0;

            float max = INT_MIN;
            float min = INT_MAX;

            for(Block& block: blocksToUpdate){

                float nextY = block.y + 25;
                float nextX = block.x + updateX;

                if(nextY <= top[block.x]){
                    block.y = nextY;  
                    block.x = nextX;
              
                }
                else {
                    staticBlocks[block.x][block.y] = block.color;
                    top[block.x] = block.y-25;
                    continue;
                }

                

                activeShape.blocks.push_back(block);
                sumX+=block.x;
                if (block.x>max)    max = block.x;
                if (block.x<min)    min = block.x;

            }

            activeShape.mid = sumX/4;
            activeShape.min = min;
            activeShape.max = max;
            
        }


};

class Game{
    public:
        Blocks blocks = Blocks();
        bool visualise = true;

    
        void Draw(){
            blocks.Draw();
        }

        void visualieGrid(){
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


};




int main(){
    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"Tetris");
    SetTargetFPS(60);
    
    Game game = Game();
    




    while(WindowShouldClose()==false){
        BeginDrawing();

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            game.blocks.spawnBlock();
        }


        if(game.blocks.blockMovementYDelay()){
            game.blocks.updateBlocksTop(game.blocks.top);
        }        


        game.visualieGrid();


        game.Draw();

        ClearBackground(bgColour);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
