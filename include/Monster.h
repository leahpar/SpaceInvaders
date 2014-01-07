#ifndef MONSTER_H
#define MONSTER_H

#include <SDL2/SDL.h>

class Monster
{
   public:
      Monster(int x, int y);
      virtual ~Monster();

      SDL_Rect& getRect();
      SDL_Rect* getpRect();
      bool isAlive();

      bool moveStep(int dir);
      bool moveLine();

   protected:


   private:
      SDL_Rect    rect;

};

#endif // MONSTER_H
