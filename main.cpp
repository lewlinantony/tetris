#include <iostream>
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <cstdlib>  





const int WINDOW_WIDTH = 375;
const int WINDOW_HEIGHT = 700;
const float BLOCK_SIZE = 25.0;
const double Y_SPEED=0.1;
const double X_SPEED=0.05;
Color BG_COLOR = {0,1,45,1};
Color WHITE_MASK = {255,255,255,100};

enum shapeType {
    I_SHAPE,
    O_SHAPE,
    T_SHAPE,
    L_SHAPE,
    J_SHAPE,
    S_SHAPE,
    Z_SHAPE
};

const std::unordered_map<shapeType, std::vector<std::vector<std::pair<int, int>>>> shapes = {

        {shapeType::I_SHAPE, {
            {{0, 0}, {1, 0}, {2, 0}, {3, 0}}, // Horizontal
            {{0, 0}, {0, 1}, {0, 2}, {0, 3}}  // Vertical
        }},

        {shapeType::O_SHAPE, {
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}}  // Single rotation
        }},

        {shapeType::T_SHAPE, {
            {{1, 0}, {0, 1}, {1, 1}, {2, 1}}, // T-up
            {{1, 0}, {1, 1}, {2, 1}, {1, 2}}, // T-right
            {{0, 1}, {1, 1}, {2, 1}, {1, 2}}, // T-down
            {{1, 0}, {0, 1}, {1, 1}, {1, 2}}  // T-left
        }},

        {shapeType::L_SHAPE, {
            {{0, 0}, {0, 1}, {1, 1}, {2, 1}}, // L-up
            {{1, 0}, {2, 0}, {1, 1}, {1, 2}}, // L-right
            {{0, 1}, {1, 1}, {2, 1}, {2, 2}}, // L-down
            {{1, 0}, {1, 1}, {1, 2}, {0, 2}}  // L-left
        }},

        {shapeType::J_SHAPE, {
            {{2, 0}, {0, 1}, {1, 1}, {2, 1}}, // J-up
            {{1, 0}, {1, 1}, {1, 2}, {2, 2}}, // J-right
            {{0, 1}, {1, 1}, {2, 1}, {0, 2}}, // J-down
            {{1, 0}, {1, 1}, {1, 2}, {0, 0}}  // J-left
        }},

        {shapeType::S_SHAPE, {
            {{1, 0}, {2, 0}, {0, 1}, {1, 1}}, // S-horizontal
            {{0, 0}, {0, 1}, {1, 1}, {1, 2}}  // S-vertical
        }},

        {shapeType::Z_SHAPE, {
            {{0, 0}, {1, 0}, {1, 1}, {2, 1}}, // Z-horizontal
            {{1, 0}, {0, 1}, {1, 1}, {0, 2}}  // Z-vertical
        }}
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




double lastUpdateX=0;
double lastUpdateY=0;
double currentTime=GetTime();
bool spawn = true;
struct Block{
    Color color;
    int x, y;
};



float Scale25(float num){
    return (BLOCK_SIZE * floor(num/BLOCK_SIZE));
}

class Shape{
    public:

        float mid;
        Color color;
        shapeType type;
        int rotation;
        std::vector<Block> blocks;


        Shape() {

            type = static_cast<shapeType>(rand() % 7);
            rotation = rand() % shapes.at(type).size();
            mid = Scale25(WINDOW_WIDTH / 2);
            color = COLORS.at(type);

            initializeBlocks();
        }

        void initializeBlocks() {
            blocks.clear(); 
            float xStart = mid;
            float yStart = 50.0f;

            for (const auto& offset : shapes.at(type)[rotation]) {
                Block block;
                block.x = static_cast<int>(xStart + offset.first * BLOCK_SIZE);
                block.y = static_cast<int>(yStart + offset.second * BLOCK_SIZE);
                block.color = color;
                blocks.push_back(block);
            }
        }


};









class Blocks{
    public:
        Shape activeShape;
        std::unordered_map<int,std::unordered_map<int,Block>> staticBlocks;
        std::unordered_map<float,float> top;

        Blocks(){
            for(int i=0;i<WINDOW_WIDTH;i=i+BLOCK_SIZE){
                top[(float)i]=WINDOW_HEIGHT-BLOCK_SIZE;
            }
        }

        void spawnBlock(){
            activeShape = Shape();
        }        

        bool blockMovementXDelay(){
            currentTime = GetTime();
            if((currentTime-lastUpdateX)>=X_SPEED){
                lastUpdateX = currentTime;
                return true;
            }
            return false;
        }



        bool blockMovementYDelay(){
            currentTime = GetTime();
            if((currentTime-lastUpdateY)>=Y_SPEED){
                lastUpdateY = currentTime;
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
                        (float)cellC.second.x,
                        (float)cellC.second.y,
                        (float)BLOCK_SIZE,
                        (float)BLOCK_SIZE,  
                    };
                    DrawRectangleRec(block,cellC.second.color);
                }
            }
    

      
  

        }

        bool fallOrNot(std::vector<Block>& blocksToUpdate){

            for(Block& block: blocksToUpdate){
                if ((top[block.x]-block.y)==0){
                    return false;
                }
            }
            return true;
        }

        void updateBlocksY(std::unordered_map<float,float>& top) {

            bool fall = fallOrNot(activeShape.blocks);

            for (Block& block : activeShape.blocks) {

                float nextY = std::clamp(block.y + BLOCK_SIZE, 50.0f, static_cast<float>(WINDOW_HEIGHT - BLOCK_SIZE));


                if (fall){         
                    block.y = nextY;                 
                } 
                else{
                    staticBlocks[block.x][block.y] = block;
                    top[block.x] = std::min(top[block.x],block.y - BLOCK_SIZE);
                    spawn = true;          
                }           

            }


            // std::string displayText2 = std::to_string(activeShape.yMax+BLOCK_SIZE);
            // DrawText(displayText2.c_str(), 10, 10, 15, WHITE);              

        }



        void updateBlocksX() {        

            Vector2 mousePosition = GetMousePosition();
            float mouseX = std::clamp(mousePosition.x, 0.0f, static_cast<float>(WINDOW_WIDTH - BLOCK_SIZE));

            float updateX = Scale25(mouseX - activeShape.mid);


            // Clamp updateX to ensure the entire shape stays within the window
            for (Block& block : activeShape.blocks) {
                float potentialNextX = block.x + updateX;
                
                if (potentialNextX < 0) {
                    updateX = -block.x;
                }
                if (potentialNextX > (WINDOW_WIDTH - BLOCK_SIZE)) {
                    updateX = (WINDOW_WIDTH - BLOCK_SIZE) - block.x;
                }
            }
            
            float sumX = 0;
            

            for (Block& block : activeShape.blocks) {
                float nextX = block.x + updateX;
                block.x = nextX;
                
                sumX += block.x;
            }

            activeShape.mid = sumX / activeShape.blocks.size();
        }

        


};

class Game{
    public:
        Blocks blocks = Blocks();
        bool visualise = true;
        bool gameOver = false;

        void checkGameOver() {
            for (auto& rowX : blocks.staticBlocks) {
                for (auto& cell : rowX.second) {
                    if (cell.second.y <= (BLOCK_SIZE*3)) {
                        gameOver = true;
                        return;
                    }
                }
            }
        }    
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
                        DrawRectangleLinesEx(cell,0.5,WHITE_MASK);
                    }
                }
            }
        }
        void Reset() {
            blocks = Blocks();  
            gameOver = false;
            spawn = true;
        }        


};




int main(){
    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"Tetris");
    SetTargetFPS(60);
    
    Game game = Game();

    while(WindowShouldClose()==false){
        BeginDrawing();


        if(!game.gameOver){
            if(spawn){
                game.blocks.spawnBlock();
                spawn = false;
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                while(!spawn)
                    game.blocks.updateBlocksY(game.blocks.top);
            }

            if ((!game.blocks.activeShape.blocks.empty()) && (game.blocks.blockMovementXDelay()))
                game.blocks.updateBlocksX();

            if((game.blocks.blockMovementYDelay()) && (!game.blocks.activeShape.blocks.empty()))
                game.blocks.updateBlocksY(game.blocks.top);    

            game.checkGameOver();
        }
        else{
            if (IsKeyPressed(KEY_R)) {
                game.Reset();
            }
        }

        std::string displayText2 = std::to_string(game.gameOver);
        DrawText(displayText2.c_str(), 15, 15, 10, WHITE); 

        game.visualieGrid();
        game.Draw();

        ClearBackground(BG_COLOR);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
