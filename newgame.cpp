#include <raylib.h>
#include <iostream>
using namespace std;

Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20, 160, 133, 255 };
Color Light_Green = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };

int player_score = 0; //declaring to store points
int cpu_score = 0;

class Ball //ball class
{
public:
    float x, y;
    int speed_x, speed_y;//speed in x and y coordinates.
    int radius;

    void Draw() 
    {
        DrawCircle(x, y, radius, Yellow); //to draw a cicle. x and y coordinates then radius then its colour.
    }

    void Update() //to make the ball move.
    {
        x += speed_x; //adding 7 to speed in x and y coordinates to make the ball move in that direction by 7.
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) //to change the direction of ball after it collides with the edges of window
        {
            speed_y *= -1;
        }
        // Cpu wins
        if (x + radius >= GetScreenWidth()) 
        {
            cpu_score++;
            ResetBall();
        }

        if (x - radius <= 0) {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall() //function to reset the ball after the player or the cpu wins.
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = { -1, 1 };//randomly select the position.
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle //class to set the controls
{
protected: //function to stop the user paddle from going out of the window
    void LimitMovement() 
    {
        if (y <= 0) 
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) 
        {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw() //drawing paddle of user.
    {
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE); //lerounded used to make the edges of the paddle round
    }

    void Update() //setting controls. 
    {
        if (IsKeyDown(KEY_UP)) 
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle //a cpu class that will inherit variables from player
{
public:
    void Update(int ball_y) //this will move the paddle of cpu according to the ball's position.
    {
        if (y + height / 2 > ball_y) 
        {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y) //the paddle will move itself and follow the motion of the ball.
        {
            y = y + speed;
        }
        LimitMovement(); //protected function inherited from parent class that will limit the movement of the cpu's paddle and stop it from going out of the window.
    } //instead ot rewriting the code we inherited the function.
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main() 
{
    cout << "Starting the game" << std::endl;
    const int screen_width = 1280; //height
    const int screen_height = 800;//width
    InitWindow(screen_width, screen_height, "My Pong Game!"); //function used to create the window (width, height and the name of that window that will appear on top left corner)
    SetTargetFPS(60); //setting the frame rate.
    ball.radius = 20; //radius of ball.
    ball.x = screen_width / 2; //to place the ball at the centre when the game starts
    ball.y = screen_height / 2;// x=width/2 and y= height/2.
    ball.speed_x = 7; //speed in x direction
    ball.speed_y = 7;//speed in y direction.

    //to create the rectangle for player
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    while (WindowShouldClose() == false) 
    { //2nd step after creating window, while loop
        BeginDrawing();

        // Updating

        ball.Update(); //to make the ball move in the window, calling update function that will update its position.
        player.Update(); //upadte function for player
        cpu.Update(ball.y); //update function for cpu but this one will be different from the player's function because the computer will control it.

        // Checking for collisions
        if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { player.x, player.y, player.width, player.height })) 
        {
            ball.speed_x *= -1;
        }//pre defined check collision functions.

        if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { cpu.x, cpu.y, cpu.width, cpu.height })) 
        {
            ball.speed_x *= -1; //this will change the direction of ball after colliding it with the rectangle.
        }

        // Drawing
        ClearBackground(Dark_Green);// after update function when we start the game the previous positions of the ball still appear on the screen, to remove them clear backgound function will be used with the colour of background.
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);//to create rectangle 
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green); //create circle function to create the ball, dividing them by 2 because we want the ball to be at the centre of window when game starts.first coordinates then radius then colour. 
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE); //to create the line at the centre of window. first two are coordinates of starting point second two are coordinates of ending point and last one is colour.
        ball.Draw(); //ball draw function to draw the ball in window.
        cpu.Draw(); //draw the paddle of cpu
        player.Draw();//draw the paddle of player.
        DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
        //pre defined function in raylib to write text, textformat used to select font, then the text, x pos, y pos, fontsize and colour
         
        EndDrawing(); //close function to close the window.
    }

    CloseWindow();
    return 0;
}