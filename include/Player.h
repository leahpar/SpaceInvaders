#ifndef PLAYER_H
#define PLAYER_H

#include "Config.h"
#include "Game.h"
#include "Bullet.h"

class Game;
class Player
{
   public:

      Player();
      ~Player();

      int play(int action);
      bool isAlive();
      void kill();

      SDL_Rect& getRect();
      SDL_Rect* getpRect();

      SDL_Rect* getBulletpRect();
      void bulletHitTarget();
      void destroyBullet();

      bool hasBullet();


   protected:

   private:
      SDL_Rect rect;
      bool alive;

      Bullet * bullet;



};
#endif // PLAYER_H
