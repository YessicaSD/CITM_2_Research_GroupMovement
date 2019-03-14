#include "Unit.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Input.h"
//#include "SDL\include\SDL_rect.h
Unit::Unit(fPoint position, SDL_Texture * tex, entities_types type):j1Entity(position,tex,type)
{
	posCollider.radio = 3;
	posCollider.center = position;
}
void Unit::Draw(float dt)
{
	


	SDL_Color color;
	if (selected)
	{
		color = { 255, 69, 25, 255 };
	}
	else
	{
		color = { 178, 34, 0, 255 };
	}
	SDL_Rect frame = { position.x-5,position.y-5,10,10 };
	App->render->DrawQuad(frame, color.r, color.g, color.b, color.a, true);
	
}

void Unit::Move(float dt)
{
	UnitStateMachine(dt);
}

void Unit::DebugDraw()
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
}

void Unit::OnCollision(Collider * c1, Collider * c2)
{
}
bool Unit::MoveOfTheWayOf(Unit* u)
{
	return true;
}
void Unit::UnitStateMachine(float dt)
{
	switch (state)
	{
	case idle:
		break;
	case waiting:
		/*if (waitTime.ReadSec() > 0.1)
		{*/
		state = IncrementWaypoint;
		/*}*/
		break;
	case getPath:
	{
		int result = App->pathfinding->CreatePath(TilePos, goal);
		if (result != -1)
		{
			Path.clear();
			Path = *App->pathfinding->GetLastPath();
			Path.erase(Path.begin());
			next_Goal = *Path.begin();
			state = IncrementWaypoint;
			
		}
	}

	break;
	case ReachedGoal:
	{

	}
	break;
	case IncrementWaypoint:
	{
		if (Path.size() > 0)
		{

			
			iPoint world_next_pos = App->map->MapToWorld(next_Goal.x , next_Goal.y );
			fPoint aux = { (float)world_next_pos.x + App->map->data.tile_width*0.5F,(float)world_next_pos.y + App->map->data.tile_height*0.5F };


			speed = aux - position;
			iPoint int_pos = { (int)position.x,(int)position.y };
			//Check if we are on the center of the tile
			if (posCollider.IsPointIn(aux))
			{
				Path.erase(Path.begin());
				next_Goal = *Path.begin();
				TilePos = next_Goal;

			}

		}

		iPoint aux = { (int)(speed.x*dt*20 ),(int)(speed.y*dt*20) };
		position.x += (float)aux.x;
		position.y += (float)aux.y;
		posCollider.center = position;
	}
	break;
	case max_state:
	{

	}
	break;
	default:
		break;
	}

}

void Unit::SetUnitDirection(Unit_Direction unitDirection)
{
	state_diretion = unitDirection;
	switch (unitDirection) {

	case UnitDirection_Idle:

		direction.x = 0.0f;
		direction.y = 0.0f;
		break;

	case UnitDirection_Up:

		direction.x = 0.0f;
		direction.y = -1.0f;
		break;

	case UnitDirection_Down:

		direction.x = 0.0f;
		direction.y = 1.0f;
		break;

	case UnitDirection_Left:

		direction.x = -1.0f;
		direction.y = 0.0f;
		break;

	case UnitDirection_Right:

		direction.x = 1.0f;
		direction.y = 0.0f;
		break;

	case UnitDirection_UpLeft:

		direction.x = -1.0f;
		direction.y = -1.0f;
		break;

	case UnitDirection_UpRight:

		direction.x = 1.0f;
		direction.y = -1.0f;
		break;

	case UnitDirection_DownLeft:

		direction.x = -1.0f;
		direction.y = 1.0f;
		break;

	case UnitDirection_DownRight:

		direction.x = 1.0f;
		direction.y = 1.0f;
		break;
	}
}

Unit_Direction Unit::GetUnitDirection()
{
	return state_diretion;
}

void Unit::SetUnitDirectionByValue(fPoint unitDirection)
{
	direction = unitDirection;
	if (direction.x > 0.0f) {

		if (direction.y > 0.0f)
			state_diretion = UnitDirection_DownRight;

		else if (direction.y < 0.0f)
			state_diretion = UnitDirection_UpRight;

		else
			state_diretion = UnitDirection_Right;
	}
	else if (direction.x < 0.0f) {

		if (direction.y > 0.0f)
			state_diretion = UnitDirection_DownLeft;

		else if (direction.y < 0.0f)
			state_diretion = UnitDirection_UpLeft;

		else
			state_diretion = UnitDirection_Left;
	}
	else {

		if (direction.y > 0.0f)
			state_diretion= UnitDirection_Down;

		else if (direction.y < 0.0f)
			state_diretion= UnitDirection_Up;

		else
			state_diretion= UnitDirection_Idle;
	}

}

fPoint Unit::GetUnitDirectionByValue() const
{
	return direction;
}

SingleUnit * Unit::GetSingleUnit() const
{
	return singleUnit;
}
