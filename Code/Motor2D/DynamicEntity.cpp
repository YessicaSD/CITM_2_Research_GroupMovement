#include "DynamicEntity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
//#include "SDL\include\SDL_rect.h
DynamicEntity::DynamicEntity(fPoint position, SDL_Texture * tex, entities_types type):j1Entity(position,tex,type)
{

}
void DynamicEntity::Draw(float dt)
{
	if (Path.size() > 0)
	{
		for (std::vector<iPoint>::iterator iter = Path.begin(); iter != Path.end(); ++iter)
		{
			SDL_Rect tile = { 0,0,10,10 };
			iPoint pos = App->map->MapToWorld((*iter).x, (*iter).y);
			tile.x = pos.x;
			tile.y = pos.y;

			App->render->DrawQuad(tile, 100, 100, 0, 200);
		}

	}

	SDL_Rect frame = { position.x,position.y,10,10 };
	App->render->DrawQuad(frame, 125, 0, 125, 125, true);
	if (selected==true)
	{
		App->render->DrawCircle(position.x, position.y, 20, 255, 255, 255, 255);
	}
	
}

void DynamicEntity::Move(float dt)
{
	
}
