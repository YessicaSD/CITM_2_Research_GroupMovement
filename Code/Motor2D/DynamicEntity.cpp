#include "DynamicEntity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"
//#include "SDL\include\SDL_rect.h
DynamicEntity::DynamicEntity(fPoint position, SDL_Texture * tex, entities_types type):j1Entity(position,tex,type)
{

}
void DynamicEntity::Draw(float dt)
{
	SDL_Rect frame = { position.x,position.y,10,10 };
	App->render->DrawQuad(frame, 125, 0, 125, 255, true);

}