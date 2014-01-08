#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>


class Bullet
{
   public:
      Bullet(int x, int y);
      virtual ~Bullet();

      SDL_Rect rect;
      SDL_Rect& getRect();
      SDL_Rect* getpRect();

      void moveStep();

   protected:

   private:

};

#endif // BULLET_H
