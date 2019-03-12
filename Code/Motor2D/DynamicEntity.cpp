#include "DynamicEntiry.h"
#include "j1App.h"
#include "j1Render.h"
//#include "SDL\include\SDL_rect.h
void DynamicEntity::Draw(float dt)
{
	App->render->Blit(texture, position.x, position.y,&Anim[state].GetCurrentFrame(dt));
}