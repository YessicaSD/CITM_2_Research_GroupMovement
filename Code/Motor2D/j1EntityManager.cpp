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
		Entities_Textures[numEntity]=App->tex->Load(TexNodes.attribute("path").as_string());
		++numEntity;
	}
	//Loading Animations
	uint state_num = 0;
	uint direction = 0;
	for (pugi::xml_node animNode = node.child("Animations").child("Allied_Unit").child("state"); animNode; animNode.next_sibling("state"))
	{
		for (pugi::xml_node directionNode = animNode.first_child();directionNode; directionNode=directionNode.next_sibling("direction"))
		{
			Allied_Info.AnimInfo[state_num][direction];
		}
		++state_num;
		++direction;
	}
	return true;
}
bool j1EntityManager::Start()
{
	bool ret = true;
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
		newEntity = new DynamicEntity(pos, Entities_Textures[type], type);
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


