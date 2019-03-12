#include "DynamicEntity.h"
#include "j1App.h"
#include "j1Render.h"
//#include "SDL\include\SDL_rect.h
DynamicEntity::DynamicEntity(fPoint position, SDL_Texture * tex, entities_types type):j1Entity(position,tex,type)
{

}
void DynamicEntity::Draw(float dt)
{
	App->render->Blit(texture, position.x, position.y,&Anim[state][diretion].GetCurrentFrame(dt));
}