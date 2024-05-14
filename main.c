#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"
/*
typedef struct Hitbox {
    Vector2 pos; //Position of the hitbox
    Rectangle area; //Area of the hitbox
    float damage; //Damage the hitbox does
    bool knockdown; //Does the hitbox induce a knockdown state
} Hitbox;

typedef struct Hurtbox {
    Vector2 pos; //Position of the hurtbox
    Rectangle area; //Area of the hurtbox
    bool invul; //true: hurtbox is in invulnerability state / false: hurtbox is not in invulnerability state
} Hurtbox;

typedef struct Projectile {
    int hitbox_amount; //Amount of hitboxes a projectile has (MAX: 32)
    Hitbox hboxes[32];
    Vector2 movement; //Movement by second of the position of any given projectile
    float time_active; //Amount of time the projectile has
    int belonging; //What player does the hitbox belong to (1,2)
} Projectile;

typedef struct Motion_Inputs {
    int hitbox_amount; //Amount of hitboxes a move has (MAX: 32)
    Hitbox hboxes[32];
    Vector2 movement; //If action changes the position of the character, by how much or to where
    float special_cost; //If action requires special meter to be used; how much
    int bullet_amount; //Does the action generate projectiles; if so, how many
    int chain[8]; //Inputs required to perform the given action
} Motion_Inputs;

typedef struct Character {
    char name[25];
    float max_hp;
    Vector2 sizebox; //Max size of any given character
    int hurtbox_amount; //Amount of hurtboxes a character has (MAX: 32)
    Rectangle hurtbox[32]; //Positions of all the hurtboxes a character has
    float gravity; //How fast does the character return to the ground
    float speed_front; //Speed for character when moving forward
    float speed_back; //Speed for character when moving backwards
    int jumps; // Amount of jumps a character has (0, 1, 2) [pls don't do 0. Ever]
    bool dash; //true: Character is able to dash / false: player is unable to dash
    bool air_dash; //true: Character is able to dash / false: player is unable to dash
    int input_amount; //Amount of inputs a character has (INCLUDES BASE INPUTS)
    Motion_Inputs actions[64]; // Moves that a character has
    //(INCLUDES BASE INPUTS; these should be at the beginning because of how the log-checker works)
} Character;

typedef struct Player {
    Character active;
    float hp;
    Vector2 position; //Position of player relative to the map; it is centered along the characters sizebox
    bool front; //true: player is facing to the front (right) / false: player is not facing to the front (left)
    float special; //Special bar meter. Pretty standard stuff for a Fighting game (0 to 4)
    bool hit; // true: player is in hitstun / false: player is not in hitstun
    bool blocking; //true: player is blocking / false: player is not blocking
    bool knockdown; //true: player is in knockdown state / false: player is not in knockdown state
    int input_log[25]; //Keeps a log of the inputs performed by the player to check for special moves (Stack data structure)
} Player;
*/

typedef struct Player {
    float hp;
    Vector2 position; //Position of player relative to the map; it is centered along the characters sizebox
    bool front; //true: player is facing to the front (right) / false: player is not facing to the front (left)
    float special; //Special bar meter. Pretty standard stuff for a Fighting game (0 to 4)
    bool hit; // true: player is in hitstun / false: player is not in hitstun
    bool blocking; //true: player is blocking / false: player is not blocking
    bool knockdown; //true: player is in knockdown state / false: player is not in knockdown state
    int input_log[25]; //Keeps a log of the inputs performed by the player to check for special moves (Stack data structure)
} Player;
/*
int characters_loader(Character *roster){


    return 1; //Characters file loaded succesfully
}
*/

void player_startup(Player *p, int play, int sizer, int width, int height){
    int i;
    p->blocking= false;
    if(play == 1){
        p->front = true;
        p->position = (Vector2){(width/3) + (sizer/2), height};
    }else {
        p->front = false;
        p->position = (Vector2){width - (width/3) - (sizer/2), height};
    }
    p->hit = false;
    p->knockdown = false;
    p->special = 0;
    p->hp = 100;
    for(i=0;i<25;i++)
        p->input_log[i] = 0;
    return;
}

Rectangle test_rectangle(Player p, float sizer){
    return (Rectangle){p.position.x - (sizer/2), p.position.y - sizer, sizer, sizer};
}

void UpdateCameraCenter(Camera2D *camera, Vector2 center, int width, int height);

int main(){
    //--------------------------------------------------------------------------------------
    //Character *roster;
    const int screenWidth = 1200;
    const int screenHeight = 600;
    int sizer = 40;
    int gamepad1 = 0, gamepad2 = 1;//IDs for connected controllers
    int scr = 0;//Active screen, used for differentiating screen visuals and logic
    Player p1 = { 0 };
    Player p2 = { 0 };
    player_startup(&p1, 1, sizer, screenWidth, screenHeight);
    player_startup(&p2, 2, sizer, screenWidth, screenHeight);



    Vector2 centered = {(p1.position.x + p2.position.x)/2, (p1.position.y + p2.position.y)/2};

    InitWindow(screenWidth, screenHeight, "SquidEngine ᔦꙬᔨ");
    Camera2D camera = { 0 };

    camera.target = centered;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;


    SetTargetFPS(120);
    //--------------------------------------------------------------------------------------

    // Main game loop

    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdatePlayer(&p1, 0, sizer, screenWidth);
        UpdatePlayer(&p2, -1, sizer, screenWidth);
        centered = (Vector2){(p1.position.x + p2.position.x)/2, (p1.position.y + p2.position.y)/2};

        UpdateCameraCenter(&camera, centered, screenWidth, screenHeight);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(LIGHTGRAY);

            BeginMode2D(camera);

            DrawRectangleRec(test_rectangle(p1, sizer), RED);
            DrawCircle(p1.position.x, p1.position.y, 5, GOLD);

            DrawRectangleRec(test_rectangle(p2, sizer), BLUE);
            DrawCircle(p2.position.x, p2.position.y, 5, GOLD);

            /*
                for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);

                Rectangle playerRect = { player1.position.x - 20, player1.position.y - 40, 40, 40 };
                DrawRectangleRec(playerRect, RED);

                DrawCircle(player1.position.x, player1.position.y, 5, GOLD);
            */
            EndMode2D();

            DrawText(TextFormat("Position p1: %.2f %.2f", p1.position.x, p1.position.y), 10, 10, 30, MAROON);
            DrawText(TextFormat("Position p2: %.2f %.2f", p2.position.x, p2.position.y), 10, 40, 30, MAROON);
            DrawText(TextFormat("Center: %.2f %.2f", centered.x, centered.y), 10, 70, 30, MAROON);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void UpdatePlayer(Player *player, Player *rel,int controller, int sizer, int width)
{
    if(controller>=0){
        if(GetGamepadAxisMovement(controller, GAMEPAD_AXIS_LEFT_X) < 0)//Movement to the right
            if((player->position.x - 5 - (sizer/2)) > 0) player->position.x -= 5;

        if(GetGamepadAxisMovement(controller, GAMEPAD_AXIS_LEFT_X) > 0)//Movement to the left
            if((player->position.x + 5 + (sizer/2)) < width) player->position.x += 5;

    }else{
        if(IsKeyDown(KEY_A))//Movement to the right
            if((player->position.x - 5 - (sizer/2)) >= 0) player->position.x -= 5;

        if(IsKeyDown(KEY_D))//Movement to the left
            if((player->position.x + 5 + (sizer/2)) <= width) player->position.x += 5;
    }
    /*
    if ((GetGamepadAxisMovement(controller, GAMEPAD_AXIS_LEFT_Y)<0) && player->canJump)
    {
        player->speed = -PLAYER_JUMP_SPD;
        player->canJump = false;
    }

    bool hitObstacle = false;
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        Vector2 *p = &(player->position);
        if (ei->blocking){
                //Checks if object has collision quality
            if(ei->rect.x <= p->x&&ei->rect.x + ei->rect.width >= p->x&&ei->rect.y + ei->rect.height >= p->y &&
            ei->rect.y <= p->y + player->speed*delta)
            {
            hitObstacle = true;
            player->speed = 0.0f;
            p->y = ei->rect.y;
            break;
            }
        }
    }

    if (!hitObstacle)
    {
        player->position.y += player->speed*delta;
        player->speed += G*delta;
        player->canJump = false;
    }
    else player->canJump = true;
    */
}

void UpdateCameraCenter(Camera2D *camera, Vector2 center, int sizer, int width, int height)
{
    camera->target = center;
    camera->offset = (Vector2){ center.x, center.y };
    //float zoomer = Vector2Length(Vector2Divide((Vector2){width, height}, center));
    //camera->zoom =
    /*
    float minX = 0, minY = 0, maxX = width, maxY = height;//Camera limits, they work kind of how one would expect them to.

    Vector2 max = GetWorldToScreen2D((Vector2){ maxX, maxY }, *camera);
    Vector2 min = GetWorldToScreen2D((Vector2){ minX, minY }, *camera);

    if (max.x < width) camera->offset.x = width - (max.x - width/5);
    if (max.y < height) camera->offset.y = height - (max.y - height/5);
    if (min.x > 0) camera->offset.x = width/2 - min.x;
    if (min.y > 0) camera->offset.y = height/2 - min.y;
        */

}
