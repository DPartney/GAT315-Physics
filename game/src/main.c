#include "body.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"
#include "force.h"
#include "render.h"
#include "editor.h"
#include "spring.h"
#include "collision.h"
#include "contact.h"
#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;

	InitWindow(1280, 720, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	float fixedTimestep = 1.0f / 60;
	float timeAccumulator = 0.0f;

	//game loop
	while (!WindowShouldClose())
	{
		ncGravity = (Vector2){ 0, -ncEditorData.GravityValue };

		//update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();
		timeAccumulator += dt;

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);
		UpdateEditor(position);

		selectedBody = GetBodyIntersect(ncBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass) + 5, YELLOW);
		}

		if (!ncEditorIntersect) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)))
			{
				ncBody* body = CreateBody(ConvertScreenToWorld(position), ncEditorData.MassValue, ncEditorData.BodyTypeActive);

				body->damping = ncEditorData.DampingValue; // 2.5f;
				body->gravityScale = ncEditorData.GravityScaleValue;
				body->color = GREEN;
				body->restitution = 0.6;

				AddBody(body);
			}

			//connectspring
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);

			if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
			{
				if (selectedBody && selectedBody != connectBody)
				{
					ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), ncEditorData.kValue);
					AddSpring(spring);
				}
			}
		}

		while (timeAccumulator >= fixedTimestep)
		{
			timeAccumulator -= fixedTimestep;
			//applyForce
			ApplyGravitation(ncBodies, ncEditorData.GravitationValue);
			ApplySpringForce(ncSprings);

			//update bodies
			for (ncBody* body = ncBodies; body; body = body->next)
			{
				Step(body, dt);
			}

			//collision
			ncContact_t* contacts = NULL;
			CreateContacts(ncBodies, &contacts);
			SeparateContacts(contacts);
			ResolveContacts(contacts);
		}

		//render
		BeginDrawing();
		ClearBackground(BLACK);

		//stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("Frame: %.4f", dt), 10, 30, 20, LIME);

		// draw bodies
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
		}

		// draw contacts
		for (ncContact_t* contact = contact; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
		}

		//draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}

		DrawEditor(position);

		EndDrawing();
	}

	CloseWindow();

	free(ncBodies);

	return 0;
}