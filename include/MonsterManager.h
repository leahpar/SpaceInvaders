#ifndef MONSTERMANAGER_H
#define MONSTERMANAGER_H

#include <vector>
using std::vector;

#include "Config.h"
#include "Monster.h"

class MonsterManager
{
   public:
      MonsterManager();
      virtual ~MonsterManager();

      void resetMonsters();

      int play();
      vector<Monster *> getMonsters();
      int getMonstersCount();
      bool playerImpact(SDL_Rect * rect);
      bool bulletImpact(SDL_Rect * rect);

      int getTileType();

   protected:


   private:

      int dirMove;
      bool changeLine;
      vector<Monster *> monsters;

      int tileType;

};

#endif // MONSTERMANAGER_H
