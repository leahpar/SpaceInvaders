#ifndef MONSTER_H
#define MONSTER_H

#include <SDL2/SDL.h>

#include "Config.h"

class Monster
{
   public:
      Monster(int x, int y, int type);
      virtual ~Monster();

      SDL_Rect& getRect();
      SDL_Rect* getpRect();
      bool isAlive();

      bool moveStep(int dir);
      bool moveLine();
      void resetLine();

      char * getTile(int tileType);

   protected:


   private:
      SDL_Rect rect;
      int yInit;

      char TileA[MONSTER_SIZE_H*MONSTER_SIZE_W +1];
      char TileB[MONSTER_SIZE_H*MONSTER_SIZE_W +1];

};

#endif // MONSTER_H
