#include "body.h"
#include "world.h"
#include "integrator.h"

void Step(ncBody* body, float timestep) 
{
	body->force = Vector2Add(body->force, Vector2Scale(Vector2Scale(ncGravity, body->gravityScale), body->mass));

	body->accleration = Vector2Scale(body->force, body->inverseMass);

	SemiImplicitEuler(body, timestep);

	// apply damping
	float damping = 1 / (1 + (body->damping * timestep));
	body->velocity = Vector2Scale(body->velocity, damping);

	ClearForce(body);
}