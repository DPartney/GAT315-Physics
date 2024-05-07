#pragma once
#include "body.h"

extern ncBody* ncBodies;
extern int ncBodyCount;
extern Vector2 ncGravity;

ncBody* CreateBody();
void DestroyBody(ncBody* body);
void DestroyAllBodies();