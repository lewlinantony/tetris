#include <iostream>
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <cstdlib>  



const int WINDOW_WIDTH = 375;
const int WINDOW_HEIGHT = 700;
const int BLOCK_SIZE = 25;
const double FALL_SPEED=0.1;
const std::unordered_map<int,std::vector<int>> shapes = {
                                                        {1, {0, 1, 2, 3}}, 
                                                        {2, {0, 1, 2, 3}}, 
                                                        {3, {0, 1, 2, 3}}, 
                                                        {4, {0, 1}}, 
                                                        {5, {0, 1}},       
                                                        {6, {0, 1}},       
                                                        {7, {0}}      
                                                    };
const std::unordered_map<int, Color> colors = {
                                                        {0, BLUE},       
                                                        {1, YELLOW},     
                                                        {2, PURPLE},     
                                                        {3, GREEN},      
                                                        {4, RED},        
                                                        {5, DARKBLUE},   
                                                        {6, ORANGE}      
                                                    };

struct Block{
    int x, y;
    Color color;
};

double lastUpdate=0;
double currentTime;





Color bgColour = {0,1,45,1};
Color whiteMask = {255,255,255,100};

class Blocks{
    public:
        std::vector<Block> activeBlocks;
        std::unordered_map<int,std::unordered_map<int,Color>> staticBlocks;
        std::unordered_map<float,float> top;

        Blocks(){
            for(int i=0;i<WINDOW_WIDTH;i=i+25){
                top[(float)i]=WINDOW_HEIGHT-25;
            }
        }
        bool blockFallDelay(){
            currentTime = GetTime();
            if((currentTime-lastUpdate)>=FALL_SPEED){
                lastUpdate = currentTime;
                return true;
            }
            return false;
        }

        void Draw(){
            for (auto& cell: activeBlocks){
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
            DrawText(std::to_string(activeBlocks.size()).c_str(),20,20,10,WHITE);

        }


        void updateBlocksTop(std::unordered_map<float,float>& top){
            // Create a copy of activeBlocks to iterate over
            std::vector<Block> blocksToUpdate = activeBlocks;
            activeBlocks.clear();
            Vector2 mousePosition = GetMousePosition();

            for(Block& block: blocksToUpdate){

                float nextY = block.y + 25;
                block.x= 25 * floor(mousePosition.x/25);
                if (block.x>WINDOW_WIDTH)
                    block.x=350;
                else if (block.x<0)
                    block.x=0;

                if(nextY <= top[block.x]){
                    block.y = nextY;
                    activeBlocks.push_back(block);
                }
                else {
                    staticBlocks[block.x][block.y] = block.color;
                    top[block.x] = block.y-25;
                }
            }
        }



        void spawnBlock(){

            Vector2 mousePosition = GetMousePosition();


            float x = 25 * floor(mousePosition.x/25);
            float y = 25.0;

            if(((y)<=(top[x]))){ //checking if above top

                Block block;
                block.x=x;
                block.y=y;
                block.color = colors.at(rand() % 7);
                activeBlocks.push_back(block);
            } 


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


        if(game.blocks.blockFallDelay()){
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
