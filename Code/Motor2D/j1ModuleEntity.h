#ifndef __j1ModuleEnemies_H__
#define __j1ModuleEnemies_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "Animation.h"
#include "j1Entity.h"
enum entities_types
{
	UNKNOWN,
	MAX_ENTITES_TYPE
};
struct EntitiesInfo
{
	entities_types type = entities_types::UNKNOWN;
	fPoint pos = { 0.0F,0.0F };
	EntitiesInfo(entities_types type, fPoint pos) :type(type), pos(pos) {}
	EntitiesInfo() {}
};


class j1Entities : public j1Module
{
public:
	std::vector<j1Entity*> list_Entities;
	pugi::xml_document	enemiesFile;
	pugi::xml_node entitiesNodeDoc;
	SDL_Texture* playerTexture=nullptr;
	SDL_Texture* entitiesTexture=nullptr;
	
public:

	j1Entities();
	~j1Entities();

	bool Awake(pugi::xml_node&)override;
	bool Start()override;
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate(float dt) override;
	bool CleanUp() override;

	j1Entity* AddEntity(const EntitiesInfo& entity);
	bool DestroyEntity(p2List_item<j1Entity*>* entity);
	void DestroyAllEntities();
	uint fx_death;
	uint fx_jump;
	uint fx_batdeath;
	uint fx_coin;
private:
	float dt=0.0F;

};
#endif // __ModuleEnemies_H__

