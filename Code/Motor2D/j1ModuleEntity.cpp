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

#include "Brofiler/Brofiler.h"

j1Entities::j1Entities()
{
	name.create("entities");
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

	fx_death = App->audio->LoadFx("audio/fx/smw_stomp_bones.wav");
	fx_jump = App->audio->LoadFx("audio/fx/jump.wav");
	fx_batdeath = App->audio->LoadFx("audio/fx/bat_death.wav");
	fx_coin = App->audio->LoadFx("audio/fx/coin2.wav");

	return true;
}
bool j1Entities::Start()
{
	bool ret = true;
	const char*	path = entitiesNodeDoc.child("player1").child("image").attribute("source").as_string();

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
	}

	return ret;
}
bool j1Entities::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_ModuleEntity.cpp", Profiler::Color::Salmon)
	this->dt = dt;
	uint vec_size = list_Entities.size();
	for (uint i = 0; i < vec_size; ++i)
	{
		std::vector<j1Entity*>::iterator iter = list_Entities.begin();
		(*iter)->toDelete == true;
	}
	p2List_item<j1Entity*>* actualEntity=nullptr;
	for (actualEntity = list_Entities.start; actualEntity; actualEntity = actualEntity->next)
	{
		if (actualEntity->data->toDelete == true)
		{
			vector<>DestroyEntity(actualEntity);
		}
		else
		{
			actualEntity->data->PreUpdate(dt);
			
		}
	}
	return true;
}

bool j1Entities::Update(float dt)
{
	BROFILER_CATEGORY("Update_ModuleEntity.cpp", Profiler::Color::Coral)
	p2List_item<j1Entity*>* actualEntity = nullptr;

	for (actualEntity = list_Entities.start; actualEntity; actualEntity = actualEntity->next)
	{
		
		actualEntity->data->Move(dt);
		
	}
	
	return true;
}

bool j1Entities::PostUpdate(float dt)
{
	BROFILER_CATEGORY("PostUpdate_ModuleEntity.cpp", Profiler::Color::MediumSlateBlue)
	p2List_item<j1Entity*>* actualEntity = nullptr;
	for (actualEntity = list_Entities.start; actualEntity; actualEntity = actualEntity->next)
	{
		actualEntity->data->Draw();
	}
	return true;
}

bool j1Entities::CleanUp()
{
	BROFILER_CATEGORY("CleanUpEntities", Profiler::Color::Blue)
	LOG("Freeing all enemies");
	
	if (playerTexture != nullptr)
	{
		App->tex->UnLoad(playerTexture);
	}
	if (entitiesTexture!=nullptr)
	{
		App->tex->UnLoad(entitiesTexture);
	}
	if (list_Entities.Count() > 0)
		DestroyAllEntities();

	return true;
}


j1Entity* j1Entities::AddEntity(const EntitiesInfo& entity)
{
	BROFILER_CATEGORY("AddEntity", Profiler::Color::Green)
	j1Entity* newEntity = nullptr;
	static_assert(UNKNOW >= 3, "code need update");
	switch (entity.type)
	{
	default: 
		assert(entity.type)
		break;
	}
		list_Entities.add(newEntity);

	return nullptr;
}

bool j1Entities::DestroyEntity(p2List_item<j1Entity*>* entity)
{
	bool ret = true;
	entity->data->toDelete = false;
	RELEASE(entity->data);
	list_Entities.del(entity);
	return ret;
}

void j1Entities::DestroyAllEntities()
{
	if (list_Entities.Count() > 0)
	{
		p2List_item<j1Entity*>* itemEntity = nullptr;
		for (itemEntity = list_Entities.end; itemEntity != nullptr; itemEntity = itemEntity->prev)
		{
			delete itemEntity->data;
		}
		list_Entities.clear();
	}

	
}

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




