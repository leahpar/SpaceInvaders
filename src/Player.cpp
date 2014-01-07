#include <cstddef>

#include <SDL2/SDL.h>

#include "Config.h"
#include "Player.h"
#include "Game.h"

Player::Player()
{
   this->rect.w = MONSTER_SIZE_W;
   this->rect.h = MONSTER_SIZE_H;
   this->rect.x = ARENA_SIZE_W/2-BASE_SIZE/2;
   this->rect.y = ARENA_SIZE_H-BASE_SIZE;

   this->alive = true;
   this->bullet = nullptr;
}

Player::~Player()
{
   delete bullet;
}


/*****************************************************************************/
/* do stuff... */
/*****************************************************************************/
int Player::play(int action)
{
   // move bullet
   if (this->bullet != nullptr)
   {
      this->bullet->moveStep();
      if (this->bullet->rect.y <= 0)
      {
         this->destroyBullet();
      }
   }

   switch (action)
   {
      case ACTION_MOVE_UP:
         if (this->rect.y > PLAYER_MOVE_STEP)
            this->rect.y -= PLAYER_MOVE_STEP;
         break;
      case ACTION_MOVE_DOWN:
         if (this->rect.y < ARENA_SIZE_H - PLAYER_MOVE_STEP - MONSTER_SIZE_H)
         this->rect.y += PLAYER_MOVE_STEP;
         break;
      case ACTION_MOVE_LEFT:
         if (this->rect.x > PLAYER_MOVE_STEP)
            this->rect.x -= PLAYER_MOVE_STEP;
         break;
      case ACTION_MOVE_RIGHT:
         if (this->rect.x < ARENA_SIZE_W - PLAYER_MOVE_STEP - MONSTER_SIZE_W)
            this->rect.x += PLAYER_MOVE_STEP;
         break;
      case ACTION_SHOT:
         if (this->bullet == nullptr)
         {
            this->bullet = new Bullet(this->rect.x + MONSTER_SIZE_W/2,
                                      this->rect.y - BULLET_SIZE_H/2);
         }
         break;
   }
   return 0;
}

bool Player::isAlive()
{
   return this->alive;
}

void Player::kill()
{
   this->alive = false;
}

SDL_Rect& Player::getRect()
{
   return this->rect;
}
SDL_Rect* Player::getpRect()
{
   return &(this->rect);
}

SDL_Rect* Player::getBulletpRect()
{
   return this->bullet->getpRect();
}

bool Player::hasBullet()
{
   return (this->bullet != nullptr);
}
void Player::bulletHitTarget()
{
   this->destroyBullet();
}

void Player::destroyBullet()
{
   delete this->bullet;
   this->bullet = nullptr;
}
