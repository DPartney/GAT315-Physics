#include "world.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity;

ncBody* CreateBody()
{
	ncBody* body = (ncBody*)malloc(sizeof(ncBody));
	assert(body);

	memset(body, 0, sizeof(ncBody));

	if (body != NULL)
	{
		body->prev = NULL;
		body->next = ncBodies;

		if (ncBodies != NULL)	ncBodies->prev = body;

		ncBodies = body;
		ncBodyCount++;
	}

	return body;
}

void DestroyBody(ncBody* body)
{
	assert(body != NULL);

	if (body->prev != NULL) body->prev->next = body->next;
	if (body->next != NULL)	body->next->prev = body->prev;
	if (body == ncBodies)	ncBodies = body->next;

	ncBodyCount--;
	free(body);
}