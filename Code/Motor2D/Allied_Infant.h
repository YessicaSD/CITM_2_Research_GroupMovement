#ifndef __DYNAMIC_ENTITY_H_
#define __DYNAMIC_ENTITY_H_
#include "j1Entity.h"
enum Unit_Direction
{
	UnitDirection_NoDirection,
	UnitDirection_Up,
	UnitDirection_Down,
	UnitDirection_Left,
	UnitDirection_Right,
	UnitDirection_UpLeft,
	UnitDirection_UpRight,
	UnitDirection_DownLeft,
	UnitDirection_DownRight
};

enum Unit_State
{
	NO_STATE,
	IDLE,
	WALK,
	DEAD,

};
class DynamicEntity: public j1Entity
{
	fPoint speed = { 0.0F,0.0F };
};

#endif // __DYNAMIC_ENTITY_H_