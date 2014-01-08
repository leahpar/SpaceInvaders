#ifndef GAME_H
#define GAME_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include "Player.h"
#include "MonsterManager.h"
#include "Bullet.h"


using namespace std;

class Player;
class Game
{
   public:
      Game();
      ~Game();

      // Game content
      int play();

   protected:
   private:

      // screen display
      SDL_Window   *screen;
      SDL_Renderer *renderer;

      // font
      TTF_Font     *font;

      // audio
      /*
      Mix_Music    *sound;
      int           pause;
      */

      // Init functions - called by constructor
      void initSDL_Video();
      void initSDL_ttf();
      //void initSDL_Mixer();
      //void loadTextures();

      // get player's keyboard action
      int getAction();

      // player
      Player * player;
      Bullet * bullet;
      MonsterManager * monsters;

      void display();      // main display function
      void displayScore(); // display scores
      void displayText(string str, int x, int y); // display a text
      void drawMonster(Monster * monster);
      void drawPlayer();
      void drawBullet();

      // Pause game
      int actionPause(string str);
      string pauseStr;

};

#endif // GAME_H
