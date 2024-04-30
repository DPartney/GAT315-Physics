#include "world.h"

#include <stdlib.h>
#include <assert.h>

Body* bodies = NULL;
int bodyCount = 0;

Body* CreateBody()
{
	Body* body = (Body*)malloc(sizeof(Body));

	if (body != NULL)
	{
		body->prev = NULL;
		body->next = bodies;

		if (bodies != NULL)	bodies->prev = body;

		bodies = body;
		bodyCount++;
		return body;
	}
}

void DestroyBody(Body* body)
{
	assert(body != NULL);

	if (body->prev != NULL) body->prev->next = body->next;
	if (body->next != NULL)	body->next->prev = body->prev;
	if (body == bodies)	bodies = body->next;

	bodyCount--;
	free(body);
}