#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Map.h"
#include <assert.h>

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name = "pathfinding";
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

bool j1PathFinding::PostUpdate(float dt)
{
	
	// https://www.youtube.com/watch?v=f7mtWD9GdJ4 
	//Explanation to Local Statics

	static iPoint origin;
	static bool origin_selected = false;
	static bool createdDebugPath = false;

	iPoint mousePos;
	App->input->GetMousePosition(mousePos.x, mousePos.y);
	iPoint p = App->render->ScreenToWorld(mousePos.x, mousePos.y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonState(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{

			origin_selected = false;

			if (CreatePath(origin, p) != -1)
			{
				createdDebugPath = true;
			}

		}
		else
		{
			origin = p;
			origin_selected = true;
			createdDebugPath = false;
			debugPath.clear();

		}
	}

	if (createdDebugPath)
	{
		uint debugPathSize = debugPath.size();
		if (debugPathSize == 0)
		{
		
			const std::vector<iPoint>* path = App->pathfinding->GetLastPath();
			uint sizeArray = path->size();
			for (uint i = 0; i < sizeArray; ++i)
			{
				debugPath.push_back(path->at(i));
			}
		}
		else
		{
			for (uint i = 0; i < debugPathSize; ++i)
			{
				iPoint pos = App->map->MapToWorld(debugPath.at(i).x, debugPath.at(i).y);
				App->render->Blit(debug_tex, pos.x, pos.y);
			}
		}

	}
	return true;
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const std::vector<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node position number or -1
// ---------------------------------------------------------------------------------
int PathList::Find(const iPoint& point)
{
	int list_pos=0;
	std::vector<PathNode>::iterator item = list.begin();
	
	while (list_pos < list.size())
	{
		if ((*item).pos == point)
			return list_pos;
		++list_pos;
		
	}
	return list_pos=-1;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
std::vector<PathNode>::iterator PathList::GetNodeLowestScore()
{
	std::vector<PathNode>::iterator ret;
	int min = 65535;

	std::vector<PathNode>::iterator item = list.end();
	while(item!=list.end())
	{
		if((*item).Score() < min)
		{
			min = (*item).Score();
			ret = item;
		}
		++item;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : numSteps(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : numSteps(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : numSteps(node.numSteps), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.size();
		

	// north
	cell.create(pos.x, pos.y + 1);
	if(App->pathfinding->IsWalkable(cell))
		
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	return list_to_fill.list.size();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return numSteps + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	numSteps = parent->numSteps + 1;
	h = pos.DistanceTo(destination);

	return numSteps + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	if (!IsWalkable(origin) || !IsWalkable(destination))
		return -1;

	PathList openList, closeList;

	openList.list.push_back({ 0, origin.DistanceTo(destination), origin, nullptr });

	bool findDestination = false;

	while (openList.list.size()>0 && !findDestination)
	{
		std::vector<PathNode>::iterator lowerNode = (std::vector<PathNode>::iterator)openList.GetNodeLowestScore();
		closeList.list.push_back(*lowerNode);
		std::vector<PathNode>::iterator currNode = closeList.list.end();
		openList.list.erase(lowerNode);

		if ((*currNode).pos == destination)
		{
			findDestination = true;
			break;
		}

		PathList neighbours;
		(*currNode).FindWalkableAdjacents(neighbours);


		for (std::vector<PathNode>::iterator nodeNeigh = neighbours.list.begin(); nodeNeigh!= neighbours.list.end(); ++nodeNeigh)
		{

			int pos_close_list = closeList.Find((*nodeNeigh).pos);
			if (pos_close_list == -1)
				continue;
			std::vector<PathNode>::iterator closeItem = closeList.list.begin() + pos_close_list;
			//assert(closeItem != closeList.list.end());
			std::vector<PathNode>::iterator openItem;

			(*nodeNeigh).CalculateF(destination);
			if (int list_pos = openList.Find((*nodeNeigh).pos) == -1)
			{
				openList.list.push_back(*nodeNeigh);
			}
			
			else if ((*nodeNeigh).numSteps < (*openItem).numSteps)
			{
				(*openItem).parent = &(*currNode);

			}


		}


	}
	if (findDestination)
	{
		last_path.clear();
		 std::vector<PathNode>::const_iterator pathNode = closeList.list.end();
		while (pathNode != closeList.list.begin())
		{
			last_path.push_back((*pathNode).pos);
			if ((*pathNode).parent != nullptr)
			{
				int aux = closeList.Find((*pathNode).parent->pos);
				pathNode = closeList.list.begin() + aux;
			}
				
		}
		//Here we flip last_path order -------------------------------------
		std::vector<iPoint> aux;
		for (uint i = last_path.size()-1; i > 0; --i)
		{
			aux.push_back(last_path.at(i));
		}
		last_path.swap(aux);
		return 1;
	}


	return -1;
}

