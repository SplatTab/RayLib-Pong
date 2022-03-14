#include "raylib.h"
#define PHYSAC_IMPLEMENTATION
#include "physac.h"

#include <iostream>
#include <string>
#include <vector>

#include "game.h"
#include "network.h"


using namespace std;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "GameLib");

    InitPhysics();
    InitConnect("127.0.0.1");

    PhysicsBody wallTop = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2.0f, -5 }, (float)screenWidth, 10, 10);
    PhysicsBody wallBottom = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2.0f, screenHeight/0.99f }, (float)screenWidth, 10, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -5, screenHeight/2.0f }, 10, (float)screenHeight, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ (float)screenWidth + 5, screenHeight/2.0f }, 10, (float)screenHeight, 10);
    wallTop->enabled = false;
    wallBottom->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;

    int cons = stoi(NetworkSend("C"));
    Player player;
    if (cons == 1)
    {
        player = {CreatePhysicsBodyRectangle((Vector2){ screenWidth, screenHeight/1.9f }, 20, 100, 1), 5.5, cons};
    } else {
        player = {CreatePhysicsBodyRectangle((Vector2){ 0, screenHeight/1.9f }, 20, 100, 1), 5.5, cons};
    }
    
    player.body->freezeOrient = true;
    player.body->useGravity = false;

    vector<Player> peers;
    // Make peers upon joining if any
    if (cons > 1) {
        for (size_t i = 1; i < cons; i++)
        {
            Player peer;
            peer.body = CreatePhysicsBodyRectangle((Vector2){ screenWidth, screenHeight/1.9f }, 20, 100, 1);
            peer.conid = cons - i;
            peer.body->enabled = false;
            peers.push_back(peer);
        }
    }

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdatePhysics();

        // If new player joins make a peer
        if (cons < stoi(NetworkSend("GC")))
        {
            cons = stoi(NetworkSend("GC"));
            for (size_t i = 0; i < cons - 1; i++)
            {
                Player peer;
                peer.body = CreatePhysicsBodyRectangle((Vector2){ 0, 0 }, 20, 100, 1);
                peer.conid = cons;
                peer.body->enabled = false;
                peers.push_back(peer);
            }
        }

        // Update peers position
        for (size_t i = 0; i < peers.size(); i++)
        {
            string getpos = "GP" + to_string(peers.at(i).conid - 1);
            string pos = NetworkSend((char*)getpos.c_str());
            size_t ypos = pos.find("Y");
            Vector2 netPos = (Vector2){stoi(pos.substr(0, ypos)), stoi(pos.substr(ypos + 1))};
            if (peers.at(i).body->position.x != netPos.x || peers.at(i).body->position.y != netPos.y)
            {
                peers.at(i).body->position = netPos;
            }
        }
        
        string posstring;

        if (IsKeyDown(KEY_W)) {
            player.body->position.y += -player.speed;
            posstring = 'P' + to_string(player.conid - 1) + ' ' + to_string(player.body->position.x) + "Y" + to_string(player.body->position.y);
            NetworkSend((char*)posstring.c_str());
        } else if (IsKeyDown(KEY_S)) {
            player.body->position.y += player.speed;
            posstring = 'P' + to_string(player.conid - 1) + ' ' + to_string(player.body->position.x) + "Y" + to_string(player.body->position.y);
            NetworkSend((char*)posstring.c_str());
        }

        BeginDrawing();

            ClearBackground(BLACK);

            int bodiesCount = GetPhysicsBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
            {
                PhysicsBody body = GetPhysicsBody(i);
                int vertexCount = GetPhysicsShapeVerticesCount(i);
                for (int j = 0; j < vertexCount; j++)
                {
                    Vector2 vertexA = GetPhysicsShapeVertex(body, j);
                    int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);
                    Vector2 vertexB = GetPhysicsShapeVertex(body, jj);
                    DrawLineV(vertexA, vertexB, GREEN);
                }
            }

            string context = "Connections: " + to_string(cons);
            DrawText(context.c_str(), 10, 10, 10, WHITE);

        EndDrawing();
    }

    CloseConnect();
    ClosePhysics();
    CloseWindow();

    return 0;
}