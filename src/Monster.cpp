#include <SDL2/SDL.h>

#include "Config.h"
#include "Monster.h"

Monster::Monster(int x, int y)
{
   this->rect.w = MONSTER_SIZE_W;
   this->rect.h = MONSTER_SIZE_H;
   this->rect.x = x*BASE_SIZE + MONSTER_MARGIN + ARENA_SIZE_W/2 - NB_MONSTERS_W*BASE_SIZE/2;
   this->rect.y = y*BASE_SIZE + MONSTER_MARGIN + ARENA_PADDING;
}

Monster::~Monster()
{
   //dtor
}

SDL_Rect& Monster::getRect()
{
   return this->rect;
}
SDL_Rect* Monster::getpRect()
{
   return &(this->rect);
}

bool Monster::moveStep(int dir)
{
   this->rect.x += dir * MONSTER_MOVE_STEP;

   if (this->rect.x >= ARENA_SIZE_W - BASE_SIZE - 2
    || this->rect.x <= MONSTER_MARGIN + 2)
   {
      return true;
   }
   return false;
}

bool Monster::moveLine()
{
   this->rect.y += MONSTER_MOVE_LINE;
   if (this->rect.y >= ARENA_SIZE_H - BASE_SIZE - 2)
   {
      return true;
   }
   return false;
}
