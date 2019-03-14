#include "j1App.h"
#include "j1EntityManager.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

#include <assert.h>



j1EntityManager::j1EntityManager()
{
	name = "entitiesManager";
}

j1EntityManager::~j1EntityManager()
{

}


bool j1EntityManager::Awake(pugi::xml_node &node)
{
	bool ret = true;
	uint numEntity = 0;
	//Loading Textures
	for (pugi::xml_node TexNodes = node.child("Entities_Textures").child("Tex"); TexNodes;TexNodes=TexNodes.next_sibling())
	{
		Allied_Info.path = TexNodes.attribute("path").as_string();
		++numEntity;
	}

	for (pugi::xml_node animNode = node.child("Animations").child("Allied_Unit").child("frame"); animNode; animNode=animNode.next_sibling("frame"))
	{
		SDL_Rect frame;
		frame.x = animNode.attribute("x").as_int();
		frame.y = animNode.attribute("y").as_int();
		frame.w = animNode.attribute("w").as_int();
		frame.h = animNode.attribute("h").as_int();
		Allied_Info.AnimInfo.PushBack(frame);
	}
	return true;
}
bool j1EntityManager::Start()
{
	bool ret = true;
	Entities_Textures[ALLIED_INFANT] = App->tex->Load(Allied_Info.path.c_str());
	return ret;
}
bool j1EntityManager::PreUpdate(float dt)
{
	uint vec_size = list_Entities.size();
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter!= list_Entities.end(); ++iter)
	{
		Unit* u = (Unit*)(*iter);
		if ((*iter)->toDelete == true)
		{
			list_Entities.erase(iter);
		}
		else
		{
			PredictPossibleCollitions(); 
			(*iter)->PreUpdate(dt);
		
			
		}
	}

	return true;
}

bool j1EntityManager::Update(float dt)
{
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
	{
		(*iter)->Move(dt);
	}

	//Get mouse pos
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	static iPoint Pos_Mouse_ClickDown;
	static iPoint Pos_Mouse_ClickRepeat;
	
	static SDL_Rect selection_rect;
	//Here we get the right click position in click down ------------------------------------
	if (App->input->GetMouseButtonState(3)== j1KeyState::KEY_DOWN)
	{
		for (std::vector<j1Entity*>::iterator iter = selected_units.begin(); iter != selected_units.end(); ++iter)
		{
			(*iter)->selected = false;
		}
		selected_units.clear();
		App->input->GetMousePosition(Pos_Mouse_ClickDown.x, Pos_Mouse_ClickDown.y);
	
	}

	//Here we generete the seletion area --------------------------------------------------------------------------------------
	if (App->input->GetMouseButtonState(3) == j1KeyState::KEY_REPEAT)
	{
		Pos_Mouse_ClickRepeat = mouse_pos;
		selection_rect.x = (Pos_Mouse_ClickDown.x < Pos_Mouse_ClickRepeat.x) ? Pos_Mouse_ClickDown.x : Pos_Mouse_ClickRepeat.x;
		selection_rect.y = (Pos_Mouse_ClickDown.y < Pos_Mouse_ClickRepeat.y) ? Pos_Mouse_ClickDown.y : Pos_Mouse_ClickRepeat.y;
		selection_rect.w = abs(Pos_Mouse_ClickRepeat.x - Pos_Mouse_ClickDown.x);
		selection_rect.h = abs(Pos_Mouse_ClickRepeat.y - Pos_Mouse_ClickDown.y);
		App->render->DrawQuad(selection_rect, 0, 125, 175, 50, true);
		
	}

	//When we realese the right button from the mouse we select entites that are inside the selection area-------------------------

	if (App->input->GetMouseButtonState(3) == j1KeyState::KEY_UP)
	{
		selected_units.clear();
		
		for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
		{
			if ((*iter)->position.x >= selection_rect.x
				&& (*iter)->position.x <= selection_rect.x + selection_rect.w
				&& (*iter)->position.y >= selection_rect.y
				&& (*iter)->position.y <= selection_rect.y + selection_rect.h)
			{
				(*iter)->selected = true;
				Unit* aux = (Unit*)(*iter);
				selected_units.push_back(*iter);
				
			}

		}
		
	}
	
	
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		for (std::vector<j1Entity*>::iterator iter = selected_units.begin(); iter != selected_units.end(); ++iter)
		{
			(*iter)->selected = false;
		}
		selected_units.clear();
	}


	//Here we click the destination-----------------------------------------------------------------------------
	//TODO 2: Now for each entity find it's goal respecting original collocation
	
	if (App->input->GetMouseButtonState(1) == j1KeyState::KEY_DOWN && App->pathfinding->IsWalkable(App->input->GetMousePos_Tiles()))
	{
		Calculate_middle_Point();
		iPoint fl_mousePos_px = App->render->ScreenToWorld(mouse_pos.x, mouse_pos.y);
		iPoint Destination_Tile = App->map->WorldToMap(fl_mousePos_px.x, fl_mousePos_px.y);
		//Center it 
		
		iPoint origin;
		Unit* Leader = nullptr;
		bool createdLeader=false;
		//Calculete max objeset Ratius
		int max_offset_goal=(int)(selected_units.size()*0.5F);
		//max_offset_goal += 5;
		for (std::vector<j1Entity*>::iterator iter = selected_units.begin(); iter != selected_units.end(); ++iter)
		{
			iPoint Tile_goal;
			origin = App->map->WorldToMap((*iter)->position.x, (*iter)->position.y);
			iPoint distance_middlePoint = (*iter)->position.ReturniPoint() - middlePoint_Group.ReturniPoint();
			iPoint Pixels_goal = (distance_middlePoint + fl_mousePos_px).ReturniPoint();
			Tile_goal = App->map->WorldToMap((int)Pixels_goal.x, (int)Pixels_goal.y);
			if (Tile_goal.x >= Destination_Tile.x - max_offset_goal
				&& Tile_goal.x <= Destination_Tile.x + max_offset_goal
				&& Tile_goal.y >= Destination_Tile.y - max_offset_goal
				&& Tile_goal.y <= Destination_Tile.y + max_offset_goal)
			{
				if (App->pathfinding->IsWalkable(Tile_goal))
				{
					Unit* selectedUnit = (Unit*)(*iter);
					selectedUnit->goal = Tile_goal;
					selectedUnit->state = getPath;
					if (createdLeader == false)
					{
						Leader = selectedUnit;
						createdLeader = true;
					}
				}
			}
		}
		for (std::vector<j1Entity*>::iterator iter = selected_units.begin(); iter != selected_units.end(); ++iter)
		{
			Unit* selectedUnit = (Unit*)(*iter);
			if (Leader != nullptr)
			{
				if (selectedUnit->state != getPath)
				{
					selectedUnit->goal = Leader->goal;
					selectedUnit->state = getPath;
				}
			}
			else
			{
				selectedUnit->goal = Destination_Tile;
				selectedUnit->state = getPath; 
				Leader = selectedUnit;
			}
			
		}
	}
	
	
		
	return true;
}

bool j1EntityManager::PostUpdate(float dt)
{
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
	{
		(*iter)->DebugDraw();
	}
	
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
		{
			(*iter)->Draw(dt);
		}
	
	if (selected_units.size() > 0)
	{
		App->render->DrawCircle(middlePoint_Group.x, middlePoint_Group.y, 2, 255, 100, 100, 255);
	}
	return true;
}

bool j1EntityManager::CleanUp()
{
	LOG("Freeing all enemies");
	
	if (list_Entities.size() > 0)
		DestroyAllEntities();

	return true;
}

void j1EntityManager::PredictPossibleCollitions()
{
	//TODO 3 predict collitons ------------------------------------------------------------------------------------------------
	Unit* u;
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
	{
		u = (Unit*)*iter;
		if (u->state == followPath)
		{
			for (std::vector<j1Entity*>::iterator jiter = list_Entities.begin(); jiter != list_Entities.end(); ++jiter)
			{
				Unit* otherU = (Unit*)*jiter;
				if (*iter != *jiter)
				{
					//TODO 3--------------------------------------------------------------------------------------------------------------
					//if the position where other entitie is, is the entities next tile objetive------------------------------------------
					if (otherU->TilePos == u->next_Goal)
					{
						if (otherU->state == idle)
						{
							if (u->next_Goal == u->goal)
							{
								u->state = idle;
							}
							else
							{
								if (otherU->MoveOfTheWayOf(u) == true)
								{
									u->SetToWaiting(otherU);
								}
								else
								{
									u->state = idle;
								}
							}
						}
					}
					//TODO 4--------------------------------------------------------------------------------------------------------
					if (otherU->next_Goal == u->next_Goal)
					{
						if (otherU->state == waiting || u->state == waiting)
						{

						}
						else
						{
							if (otherU->next_Goal == otherU->goal)
							{
								otherU->SetToWaiting(u);
							}
							else if (u->next_Goal == u->goal)
							{
								u->SetToWaiting(otherU);
							}
							else
							otherU->SetToWaiting(u);
						}
					}

				}
			}
		}
	}
}



j1Entity* j1EntityManager::AddEntity(entities_types type, fPoint pos)
{
	j1Entity* newEntity = nullptr;
	static_assert(MAX_ENTITIES >= 0, "code need update");
	switch (type)
	{
	case ALLIED_INFANT:
	{
		newEntity = new Unit(pos, Entities_Textures[type], type);
		Unit* DynPointer = (Unit*)newEntity;
		DynPointer->TilePos = App->map->WorldToMap((int)pos.x, (int)pos.y);
		DynPointer->goal = DynPointer->TilePos;
	}
		
	break;

	default: 
		assert(!"The default case of AddEntity switch was reached.");
		break;
	}

	if(newEntity!=nullptr)
		list_Entities.push_back(newEntity);

	return nullptr;
}

bool j1EntityManager::DestroyEntity(std::vector<j1Entity*>::iterator entity)
{
	bool ret = true;
	(*entity)->toDelete = false;
	RELEASE(*entity);
	list_Entities.erase(entity);
	return ret;
}

void j1EntityManager::DestroyAllEntities()
{
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
	{
		delete *iter;

	}
	list_Entities.clear();
}

j1Entity* j1EntityManager::InThisTile_IsUnits(iPoint tile)
{
	j1Entity* ret = nullptr;
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
	{
		iPoint TilePos = App->map->WorldToMap((*iter)->position.x, (*iter)->position.y);
		if (TilePos == tile)
		return ret=*iter;
	}
	return ret;
}

fPoint j1EntityManager::Calculate_middle_Point()
{
	middlePoint_Group.SetToZero();
	for (std::vector<j1Entity*>::iterator iter = selected_units.begin(); iter != selected_units.end(); ++iter)
	{
		middlePoint_Group += (*iter)->position;
	}
	middlePoint_Group /= selected_units.size();
	return middlePoint_Group;
}
