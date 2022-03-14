#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct Player {
    PhysicsBody body;
    float speed = 15;
    int conid;
} Player;

#endif