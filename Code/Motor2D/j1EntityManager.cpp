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
		if ((*iter)->toDelete == true)
		{
			list_Entities.erase(iter);
		}
		else
		{
			(*iter)->PreUpdate(dt);
		}
	}

	return true;
}

bool j1EntityManager::Update(float dt)
{
	static iPoint Pos_Mouse_ClickDown;
	static iPoint Pos_Mouse_ClickRepeat;
	static j1KeyState last_righ_click_state;
	static SDL_Rect selection_rect;
	//Right Click
	if (App->input->GetMouseButtonState(3)== j1KeyState::KEY_DOWN)
	{
		App->input->GetMousePosition(Pos_Mouse_ClickDown.x, Pos_Mouse_ClickDown.y);
		last_righ_click_state = j1KeyState::KEY_DOWN;
	}
	if (App->input->GetMouseButtonState(3) == j1KeyState::KEY_REPEAT)
	{
		App->input->GetMousePosition(Pos_Mouse_ClickRepeat.x, Pos_Mouse_ClickRepeat.y);
		selection_rect.x = (Pos_Mouse_ClickDown.x < Pos_Mouse_ClickRepeat.x) ? Pos_Mouse_ClickDown.x : Pos_Mouse_ClickRepeat.x;
		selection_rect.y = (Pos_Mouse_ClickDown.y < Pos_Mouse_ClickRepeat.y) ? Pos_Mouse_ClickDown.y : Pos_Mouse_ClickRepeat.y;
		selection_rect.w = abs(Pos_Mouse_ClickRepeat.x - Pos_Mouse_ClickDown.x);
		selection_rect.h = abs(Pos_Mouse_ClickRepeat.y - Pos_Mouse_ClickDown.y);
		App->render->DrawQuad(selection_rect, 0, 125, 175, 50, true);
		
	}
	if (App->input->GetMouseButtonState(3) == j1KeyState::KEY_UP)
	{
		for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
		{
			if ((*iter)->position.x >= selection_rect.x
				&& (*iter)->position.x <= selection_rect.x + selection_rect.w
				&& (*iter)->position.y >= selection_rect.y
				&& (*iter)->position.y <= selection_rect.y + selection_rect.h)
			{
				(*iter)->selected = true;
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

	iPoint destination  = App->input->GetMousePos_Tiles();
	if (App->input->GetMouseButtonState(1) == j1KeyState::KEY_DOWN && App->pathfinding->IsWalkable(destination))
	{
		iPoint origin;
		
		for (std::vector<j1Entity*>::iterator iter = selected_units.begin(); iter != selected_units.end(); ++iter)
		{
			origin=App->map->WorldToMap((*iter)->position.x, (*iter)->position.y);
			if (App->pathfinding->CreatePath(origin, destination) != -1)
			{
				DynamicEntity* selectedUnit = (DynamicEntity*)(*iter);
				selectedUnit->Path.clear();
				selectedUnit->Path=*App->pathfinding->GetLastPath();
			}

		}
		
	}
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
	{
		(*iter)->Move(dt);
	}
	
	
		
	return true;
}

bool j1EntityManager::PostUpdate(float dt)
{
	
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
		{
			(*iter)->Draw(dt);
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


j1Entity* j1EntityManager::AddEntity(entities_types type, fPoint pos)
{
	j1Entity* newEntity = nullptr;
	static_assert(MAX_ENTITIES >= 0, "code need update");
	switch (type)
	{
	case ALLIED_INFANT:
	{
		newEntity = new DynamicEntity(pos, Entities_Textures[type], type);
		DynamicEntity* DynPointer = (DynamicEntity*)newEntity;
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

bool j1EntityManager::InThisTile_IsUnits(iPoint tile)
{
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
	{
		iPoint TilePos = App->map->WorldToMap((*iter)->position.x, (*iter)->position.y);
		if (TilePos == tile)
		return true;
	}
	return false;
}

/*
bool j1Entities::LoadAnimations(pugi::xml_node animNode) 
{
	bool ret = true;
	int numAnim = 0;
	for (pugi::xml_node thisanimNode = animNode; thisanimNode; thisanimNode = thisanimNode.next_sibling("animation"))
		++numAnim;

	Animation* anim_aux = new Animation[numAnim];

	SDL_Rect frameRect;
	int animArrayNum = 0;
	for (pugi::xml_node thisAnimNode = animNode; thisAnimNode; thisAnimNode = thisAnimNode.next_sibling("animation"))
	{
		anim_aux[animArrayNum].speed = thisAnimNode.attribute("anim_speed").as_float();
		for (pugi::xml_node frame = thisAnimNode.child("frame"); frame; frame=frame.next_sibling("frame"))
		{
			frameRect.x = frame.attribute("x").as_int();
			frameRect.y = frame.attribute("y").as_int();
			frameRect.w = frame.attribute("width").as_int();
			frameRect.h = frame.attribute("height").as_int();
		
			anim_aux[animArrayNum].PushBack(frameRect);
		}
		++animArrayNum;
	}
	entitiesAnimation.add(anim_aux);

	return ret;
}

*/


