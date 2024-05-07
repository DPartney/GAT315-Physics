#include "body.h"
#include "integrator.h"
#include "world.h"

void Step(ncBody* body, float timestep)
{
	body->force = Vector2Add(body->force, Vector2Scale(Vector2Scale(ncGravity, body->gravityScale), body->mass));
	body->acceleration = Vector2Scale(body->force, body->inverseMass);

	SemiImplicitEuler(body, timestep);

	// Apply Damping
	float damping = 1 / (1 + (body->damping * timestep));
	body->velocity = Vector2Scale(body->velocity, damping);

	ClearForce(body);
}

void ApplyGravitation(ncBody* bodies, float strength)
{
	for (ncBody* body1 = bodies; body1; body1 = body1->next)
	{
		for (ncBody* body2 = bodies; body2; body2 = body2->next)
		{
			if (body1 == body2) continue;

			Vector2 direction = Vector2Subtract(body1->position, body2->position); //< calculate direction(body position - body position)
			float distance = Vector2Length(body1->position, body2->position); // <get length of vector>

			distance = fmaxf(distance, 1);
			float force = (body1->mass * body2->mass / (distance * distance)) * strength;

			direction = Vector2Normalize(Vector2Scale(direction, force)); //<scale normalized direction vector by force>;

			ApplyForce(body1, direction, FM_FORCE);
			ApplyForce(body2, direction, FM_FORCE);
		}
	}
}