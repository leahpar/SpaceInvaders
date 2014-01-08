#include <SDL2/SDL.h>

#include "Config.h"
#include "Monster.h"
#include "MonsterManager.h"


MonsterManager::MonsterManager()
{
   int x, y;

   this->dirMove = RIGHT;
   this->changeLine = false;
   this->tileType = 0;

   // create monsters
   for (y=0; y<NB_MONSTERS_H; y++)
   {
      for (x=0; x<NB_MONSTERS_W; x++)
      {
         this->monsters.push_back(new Monster(x, y, (2*y)%3));
      }
   }
}

MonsterManager::~MonsterManager()
{
   for (auto &m : this->monsters)
   {
      delete m;
   }
}

void MonsterManager::resetMonsters()
{
   // create monsters
   for (auto &m : this->monsters)
   {
      m->resetLine();
   }
}

int MonsterManager::play()
{
   bool limit = false;
   this->tileType = !this->tileType;

   // change line
   if (this->changeLine)
   {
      this->dirMove *= -1;
      this->changeLine = false;
      limit = false;
      for (auto &m : this->monsters)
      {
         limit |= m->moveLine();
      }
      if (limit)
      {
         return 1;
      }
   }
   else
   {
      // move monsters
      for (auto &m : this->monsters)
      {
         this->changeLine |= m->moveStep(this->dirMove);
      }
   }
   return 0;
}

vector<Monster *> MonsterManager::getMonsters()
{
   return this->monsters;
}

int MonsterManager::getMonstersCount()
{
   return this->monsters.size();
}

bool MonsterManager::playerImpact(SDL_Rect * rect)
{
   for (auto &m : this->monsters)
   {
      if (SDL_HasIntersection(m->getpRect(), rect))
      {
         return true;
      }
   }
   return false;
}

bool MonsterManager::bulletImpact(SDL_Rect * rect)
{
   unsigned int pos = 0;

   for (auto &m : this->monsters)
   {
      if (SDL_HasIntersection(m->getpRect(), rect))
      {
         this->monsters.erase(this->monsters.begin()+pos);
         //delete m; <== m is deleted by vector.erase()
         return true;
      }
      pos++;
   }

   return false;
}

int MonsterManager::getTileType()
{
   return this->tileType;
}

