#include <SDL2/SDL.h>

#include "Config.h"
#include "Bullet.h"

Bullet::Bullet(int x, int y)
{
   this->rect.x = x;
   this->rect.y = y;
   this->rect.w = BULLET_SIZE_W;
   this->rect.h = BULLET_SIZE_H;
}

Bullet::~Bullet()
{
   //dtor
}

void Bullet::moveStep()
{
   this->rect.y -= BULLET_MOVE_STEP;
}

SDL_Rect& Bullet::getRect()
{
   return this->rect;
}
SDL_Rect* Bullet::getpRect()
{
   return &(this->rect);
}
