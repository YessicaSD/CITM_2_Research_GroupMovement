#include "j1App.h"
#include "j1ModuleEntity.h"
#include "p2Defs.h"
#include "p2Log.h"

#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

#include <assert.h>



j1Entities::j1Entities()
{
	name = "entities";
}

j1Entities::~j1Entities()
{

}


bool j1Entities::Awake(pugi::xml_node &node)
{
	bool ret = true;
	const char* path = node.child_value();
	LOG("%s", node.child_value());
	pugi::xml_parse_result result = enemiesFile.load_file(path);
	if (result == NULL)
	{
		LOG("ERROR ENTITIES LOADING FILE %s", result.description());
		return ret = false;
	}

	entitiesNodeDoc = enemiesFile.child("entities");

	if (!entitiesNodeDoc)
	{
		LOG("ERROR ENTITIES LOADING FILE ");
		return ret = false;
	}

	return true;
}
bool j1Entities::Start()
{
	bool ret = true;
	/*const char*	path = entitiesNodeDoc.child("player1").child("image").attribute("source").as_string();

	if ((playerTexture = App->tex->Load(path))==nullptr)
	{
		LOG("ERROR LOADING TEXTURE PLAYER");
		return ret = false;
	}
	path = entitiesNodeDoc.child("EnemiesImage").attribute("source").as_string();
	if ((entitiesTexture = App->tex->Load(path)) == nullptr)
	{
		LOG("ERROR LOADING TEXTURE ENEMIES");
		return ret = false;
	}*/

	return ret;
}
bool j1Entities::PreUpdate(float dt)
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

bool j1Entities::Update(float dt)
{
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
	{
		(*iter)->Move(dt);
	}
	
	return true;
}

bool j1Entities::PostUpdate(float dt)
{
	
	for (std::vector<j1Entity*>::iterator iter = list_Entities.begin(); iter != list_Entities.end(); ++iter)
		{
			(*iter)->Draw();
		}
	return true;
}

bool j1Entities::CleanUp()
{
	LOG("Freeing all enemies");
	
	if (list_Entities.size() > 0)
		DestroyAllEntities();

	return true;
}


j1Entity* j1Entities::AddEntity(entities_types type, fPoint pos)
{
	j1Entity* newEntity = nullptr;
	static_assert(UNKNOW >= 0, "code need update");
	switch (type)
	{
	default: 
		assert(!"The default case of AddEntity switch was reached.");
		break;
	}

	if(newEntity!=nullptr)
		list_Entities.push_back(newEntity);

	return nullptr;
}

bool j1Entities::DestroyEntity(std::vector<j1Entity*>::iterator entity)
{
	bool ret = true;
	(*entity)->toDelete = false;
	RELEASE(*entity);
	list_Entities.erase(entity);
	return ret;
}

void j1Entities::DestroyAllEntities()
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


