#ifndef GAME_H
#define GAME_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include "Player.h"
#include "MonsterManager.h"
#include "Bullet.h"

#define SDL_MAX_KEY 1000

// http://wiki.libsdl.org/SDLKeycodeLookup
// 0x40000000 + 0x00000200 = 0x3FFFFE39
#define SDL_KEY_CONVERT(k) (k > 0x40000000 ? k - 0x3FFFFF39 : k)

#define KEY_QUIT 999

#define KEY_LSHIFT   SDL_KEY_CONVERT(SDLK_LSHIFT)
#define KEY_RSHIFT   SDL_KEY_CONVERT(SDLK_RSHIFT)
#define KEY_LCTRL    SDL_KEY_CONVERT(SDLK_LCTRL)
#define KEY_RCTRL    SDL_KEY_CONVERT(SDLK_RCTRL)
#define KEY_RSHIFT   SDL_KEY_CONVERT(SDLK_RSHIFT)
#define KEY_ESCAPE   SDL_KEY_CONVERT(SDLK_ESCAPE)
#define KEY_ENTER    SDL_KEY_CONVERT(SDLK_RETURN)
#define KEY_TAB      SDL_KEY_CONVERT(SDLK_TAB)
#define KEY_PAUSE    SDL_KEY_CONVERT(SDLK_p)
#define KEY_UP       SDL_KEY_CONVERT(SDLK_UP)
#define KEY_DOWN     SDL_KEY_CONVERT(SDLK_DOWN)
#define KEY_LEFT     SDL_KEY_CONVERT(SDLK_LEFT)
#define KEY_RIGHT    SDL_KEY_CONVERT(SDLK_RIGHT)
#define KEY_SPACE    SDL_KEY_CONVERT(SDLK_SPACE)

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

      // player
      Player * player;
      MonsterManager * monsters;

      void display();      // main display function
      void displayScore(); // display scores
      void displayText(string str, int x, int y); // display a text
      void drawMonster(Monster * monster, int tileType);
      void drawPlayer(SDL_Rect playerRect);
      void drawBullet();

      // Pause game
      int actionPause(string str);
      string pauseStr;

      // UI
      int  pauseGame;
      char key[SDL_MAX_KEY];
      void UIgetAction();
      int  UIHandleAction();

      // game
      int level;


};

#endif // GAME_H
