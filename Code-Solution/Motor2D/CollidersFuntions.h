#ifndef _COLLIDERS_FUNTIONS_H_
#define _COLLIDERS_FUNTIONS_H_
#include "p2Point.h"
struct Circle
{
	fPoint center;
	float radio;
	bool IsPointIn(fPoint point)
	{
		if (point.x > (center.x - radio)
			&& point.x< (center.x + radio)
			&& point.y >(center.y - radio)
			&& point.y < (center.y + radio))
			return true;
		else
			return false;
	}
};


#endif // !_COLLIDERS_FUNTIONS_H_

