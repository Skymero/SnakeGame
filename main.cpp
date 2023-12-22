#include <iostream>
#include <raylib.h>

// Data structure that allows you to add & remove elements from both ends
#include <deque>

//allows for vector math
#include <raymath.h>


using namespace std;

Color green = {173, 204, 96,255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;

double lastUpdateTime = 0;

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }

    return false;
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = {1,0};


    void Draw()
    {
        // Draw each segment of the body (3 segments)
        //  Loop through all the body elements 
        for(unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{x*cellSize, y*cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5,6, darkGreen); // why cellsize, cellsize?
        }
    }

    void Update()
    {
        body.pop_back(); // removes las element from the back of the container
        body.push_front(Vector2Add(body[0],direction)); // moves the last element that was pop_back'd to the new desired position
    }

    
};



class Food{
    
    public:
        Vector2 position;
        Texture2D texture;

        // Contructor - to initialize an object of the same class name. automatically called when an instance of the class is created
        Food()
        {
            Image image = LoadImage("Graphics/food.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image); // unload image to free some memory
            position = GenerateRandomPos();

        }

        //destructor - releasing resources obj gathered during its life time
        ~Food()
        {
            UnloadTexture(texture);
        }

        void Draw()
        {
            //DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, darkGreen);
            DrawTexture(texture, position.x *cellSize, position.y*cellSize, WHITE);
        }

        Vector2 GenerateRandomPos()
        {
            
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            
            return Vector2{x, y};

        }

    private:

};


// Game class is a container for all the elements of the game
//  holds methods that update the game's logic (ie position, collision checks, etc)
class Game
{
    public:
        Snake snake = Snake();
        Food food = Food();

        void Draw()
        {
            food.Draw();
            snake.Draw();
        }

        void Update()
        {
            snake.Update();
        }
    private:
};


int main () {

    cout << " Starting the game.... " << endl;
    InitWindow(cellCount*cellSize,cellCount*cellSize,"Retro Snake");
    SetTargetFPS(60); // Setting the frame rate

    Food food = Food(); //creates food object
    Snake snake = Snake();

    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        if(eventTriggered(0.2))
        {
            snake.Update();
        }

        if(IsKeyPressed(KEY_UP) && snake.direction.y != 1)
        {
            snake.direction = {0, -1}; // indicating that the snake should upwards
            // the Vector2 struct represents a 2D vector with x & y components
            // origin is on the top left corner
        }

        if(IsKeyPressed(KEY_DOWN) && snake.direction.y != -1)
        {
            snake.direction = {0, 1};
        }

        if(IsKeyPressed(KEY_LEFT) && snake.direction.x != 1)
        {
            snake.direction = {-1, 0};
        }

        if(IsKeyPressed(KEY_RIGHT) && snake.direction.x != -1)
        {
            snake.direction = {1, 0};
        }


        //Drawing 
        ClearBackground(green);
        food.Draw(); // calls draw method of food object
        snake.Draw();



        EndDrawing();

    }



    CloseWindow();
    return 0;
}