#include "body.h"
#include "mathf.h"
#include "world.h"
#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 10000

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	/*
	Body* bodies = (Body*)malloc(sizeof(Body) * MAX_BODIES);
	assert(bodies != NULL);
	int bodyCount = 0;
	*/

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		if (IsMouseButtonDown(0))
		{
			/*
			bodies[bodyCount].position = position;
			bodies[bodyCount].velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
			bodyCount++;
			*/
			CreateBody();
			bodies->position = position;
			bodies->velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
		}

		// render
		BeginDrawing();
		ClearBackground(BLACK);
		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 20, YELLOW);

		// update bodies / draw bodies
		Body* body = bodies;
		while (body) {
			body->position = Vector2Add(body->position, body->velocity);
			DrawCircle(body->position.x, body->position.y, 10, RED);
			body = body->next;
		}

		EndDrawing();
	}

	CloseWindow();

	free(bodies);

	return 0;
}