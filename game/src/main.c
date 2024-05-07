#include "body.h"
#include "mathf.h"
#include "world.h"
#include "raylib.h"
#include "raymath.h"
#include "integrator.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 10000

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	// Initliaze World
	ncGravity = (Vector2){ 0, 30 };

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		if (IsMouseButtonDown(0))
		{
			for (int i = 0; i < 50; i++)
			{
				ncBody* body = CreateBody();
				body->position = position;
				body->mass = GetRandomFloatValue(5, 10);
				body->inverseMass = 1 / body->mass;
				body->type = BT_KINEMATIC;
				body->damping = 0.5f;
				body->gravityScale = 10;
				body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);
				float angle = GetRandomFloatValue(0, 360);
				Vector2 force = Vector2Scale(GetVector2FromAngle(angle + GetRandomFloatValue(-30, 30) * DEG2RAD), GetRandomFloatValue(1000, 2000));
				ApplyForce(body, force, FM_IMPULSE);
			}
		}

		// Apply Force
		ncBody* body = ncBodies;
		while (body) {
			//ApplyForce(body, CreateVector2(0, -500), FM_FORCE);
			body = body->next;
		}

		// update bodies
		body = ncBodies;
		while (body) {
			Step(body, dt);
			body = body->next;
		}

		// render
		BeginDrawing();
		ClearBackground(BLACK);

		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 20, YELLOW);

		// draw bodies
		body = ncBodies;
		while (body) {
			DrawCircle(body->position.x, body->position.y, body->mass, body->color);
			body = body->next;
		}

		EndDrawing();
	}

	CloseWindow();

	free(ncBodies);

	return 0;
}