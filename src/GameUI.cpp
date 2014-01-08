#include <SDL2/SDL.h>

#include "Game.h"
#include "Config.h"
#include "Device.h"

using namespace std;


/**
* Handle the current player's action
*/
int Game::UIHandleAction()
{
   int action = ACTION_NONE;
   this->UIgetAction();

   // quit game
   if (this->key[KEY_QUIT])
   {
      action = ACTION_QUIT;
   }
   else if (this->key[KEY_ESCAPE] && this->pauseGame > 0)
   {
      action = ACTION_QUIT;
   }

   // pause game
   else if (this->key[KEY_PAUSE] && this->pauseGame == 0)
   {
      this->pauseStr = string("Pause");
      this->pauseGame = 1;
      action = ACTION_PAUSE;
      this->key[KEY_PAUSE] = 0;
   }
   else if (this->key[KEY_ESCAPE] && this->pauseGame == 0)
   {
      this->pauseStr = string("Pause");
      this->pauseGame = 1;
      action = ACTION_PAUSE;
      this->key[KEY_ESCAPE] = 0;
   }
   // unpause game
   else if (this->key[KEY_PAUSE] && this->pauseGame == 1)
   {
      this->pauseStr = string("");
      this->pauseGame = 0;
      this->key[KEY_PAUSE] = 0;
   }
   // start game
   else if (this->key[KEY_SPACE] && this->pauseGame == 2)
   {
      this->pauseStr = string("");
      this->pauseGame = 0;
      this->key[KEY_SPACE] = 0;
   }

   return action;

}

/**
* Get the current player's action
*/
void Game::UIgetAction()
{
   SDL_Event event;
   while (SDL_PollEvent(&event))
   {
      // ignore repeted event
      if (event.key.repeat == 1) continue;

      switch(event.type)
      {
         case SDL_QUIT:
            this->key[KEY_QUIT] = 1;
            break;
         case SDL_KEYDOWN:
            this->key[SDL_KEY_CONVERT(event.key.keysym.sym)] = 1;
            break;
         case SDL_KEYUP:
            this->key[SDL_KEY_CONVERT(event.key.keysym.sym)] = 0;
            break;
     }
   }
}
