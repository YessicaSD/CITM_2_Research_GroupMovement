#ifndef __j1ModuleEnemies_H__
#define __j1ModuleEnemies_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "Animation.h"
#include "j1Entity.h"


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

	j1Entity* AddEntity(entities_types type,fPoint pos);
	bool DestroyEntity(std::vector<j1Entity*>::iterator entity);
	void DestroyAllEntities();
private:
	float dt=0.0F;

};
#endif // __ModuleEnemies_H__

