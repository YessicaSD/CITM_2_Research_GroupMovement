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
	MAX_STATE
};
class DynamicEntity: public j1Entity
{
public:
	DynamicEntity(fPoint position, SDL_Texture * tex, entities_types type);
	Unit_State state = IDLE;
	Unit_Direction diretion;
	fPoint speed = { 0.0F,0.0F };
	SDL_Texture* Tex_Idle;
	SDL_Texture* Tex_Selected;
	void Draw(float dt) override;
	bool selected= false;
};

#endif // __DYNAMIC_ENTITY_H_