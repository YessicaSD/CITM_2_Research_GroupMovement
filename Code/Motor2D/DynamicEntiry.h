#ifndef __DYNAMIC_ENTITY_H_
#define __DYNAMIC_ENTITY_H_
#include "j1Entity.h"
#include "Animation.h"
enum Unit_Direction
{
	UnitDirection_Up,
	UnitDirection_Down,
	UnitDirection_Left,
	UnitDirection_Right,
	UnitDirection_UpLeft,
	UnitDirection_UpRight,
	UnitDirection_DownLeft,
	UnitDirection_DownRight,
	UnitDirection_MaxDirections,
};

enum Unit_State
{
	IDLE,
	WALK,
	DEAD,
	MAX_STATE
};
class DynamicEntity: public j1Entity
{
	Unit_State state = IDLE;
	fPoint speed = { 0.0F,0.0F };
	Animation Anim[MAX_STATE];
	void Draw(float dt) override;
};

#endif // __DYNAMIC_ENTITY_H_