#include "spring.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k)
{
	ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t));
	assert(spring);

	memset(spring, 0, sizeof(ncSpring_t));

	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;

	return spring;
}

void AddSpring(ncSpring_t* spring)
{
	assert(spring);

	//add element to linked list
	spring->prev = NULL;
	spring->next = ncSprings;

	if (ncSprings) ncSprings->prev = spring;

	//set head of element to new element
	ncSprings = spring;
}

void DestroySpring(ncSpring_t* spring)
{
	if (!spring) return;

	if (spring->prev) spring->prev->next = spring->next;
	if (spring->next) spring->next->prev = spring->prev;

	if (spring == ncSprings) ncSprings = spring->next;

	free(spring);
}

void DestoryAllSprings()
{

}

void ApplySpringForce(ncSpring_t* springs) 
{
	for (ncSpring_t* spring = springs; spring; spring = spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->body1->position, spring->body2->position);//<get direction vector from body2 to body1>
		if (direction.x == 0 && direction.y == 0) continue;

		float length = Vector2Length(direction); //<get length from direction>
		float x = length - spring->restLength; //<compute displacement from current length to resting length>;
		float force = (-spring->k) * x;//<(f = -kx) compute force using product of displacement and stiffness(k)>;

		Vector2 ndirection = Vector2Normalize(direction); //<get direction normal>

		ApplyForce(spring->body1, Vector2Scale(ndirection, force), FM_FORCE);
		ApplyForce(spring->body2, Vector2Scale(ndirection, -force), FM_FORCE);
	}
}