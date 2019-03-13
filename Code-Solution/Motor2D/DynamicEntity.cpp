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

			App->render->DrawQuad(tile, 50, 100, 0, 200);
		}

	}

	SDL_Color color;
	if (selected)
	{
		color = { 255, 69, 25, 255 };
	}
	else
	{
		color = { 178, 34, 0, 255 };
	}
	SDL_Rect frame = { position.x,position.y,10,10 };
	App->render->DrawQuad(frame, color.r, color.g, color.b, color.a, true);
	
}

void DynamicEntity::Move(float dt)
{
	if (Path.size() > 0)
	{
		
		iPoint tile_next_pos = *Path.begin();
		iPoint world_next_pos = App->map->MapToWorld(tile_next_pos.x, tile_next_pos.y);
		fPoint aux = { (float)world_next_pos.x,(float)world_next_pos.y};


		speed = aux - position;
		iPoint int_pos = { (int)position.x,(int)position.y };
		if (world_next_pos == int_pos)
		{
			Path.erase(Path.begin());
		}

	}

	iPoint aux = { (int)(speed.x*dt*50),(int)(speed.y*dt*50) };
	position.x += (float)aux.x;
	position.y += (float)aux.y;

}
