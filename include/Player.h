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
      void init();

      int play(int action);
      bool isAlive();
      int getLives();
      void kill();

      SDL_Rect& getRect();
      SDL_Rect* getpRect();

      SDL_Rect* getBulletpRect();
      void bulletHitTarget(int score);
      void destroyBullet();

      bool hasBullet();
      int getScore();

   protected:

   private:
      SDL_Rect rect;
      int lives;
      int score;

      Bullet * bullet;





};
#endif // PLAYER_H
