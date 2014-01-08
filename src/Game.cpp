#include <unistd.h>
#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include "Config.h"
#include "Device.h"
#include "Game.h"
#include "Exceptions.h"

#include "Player.h"
#include "MonsterManager.h"
#include "Bullet.h"

using namespace std;

Game::Game()
{
   // may throw exceptions
   this->initSDL_Video();
   this->initSDL_ttf();
#ifndef __APPLE__
   //this->initSDL_Mixer();
#endif
   this->pauseStr = "";

   this->player   = new Player();
   this->monsters = new MonsterManager();
}

Game::~Game()
{
   TTF_CloseFont(this->font);
   TTF_Quit();

   //Mix_HaltMusic();
   //Mix_FreeMusic(this->sound);

   //SDL_DestroyTexture(this->tileset);
   SDL_DestroyRenderer(this->renderer);
   SDL_DestroyWindow(this->screen);
   SDL_Quit();

   delete this->player;
   delete this->monsters;
}

void Game::initSDL_Video()
{
   // Init SDL
   //SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
   SDL_Init(SDL_INIT_VIDEO);
   this->screen = SDL_CreateWindow(WINDOW_TITLE,
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   WINDOW_SIZE_W,
                                   WINDOW_SIZE_H,
                                   0);
   this->renderer = SDL_CreateRenderer(this->screen, -1, 0);
   //this->loadTextures();
}

void Game::initSDL_ttf()
{
   // Init SDL ttf
   TTF_Init();
   this->font  = TTF_OpenFont(FONT_FILE, FONT_SIZE);
}
/*
void Game::initSDL_Mixer()
{
   // Init audio
   if (Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS))
   {
      Alert(NULL, Mix_GetError(), NULL, 0);
      throw ERR_INIT_AUDIO;
   }
   this->sound = Mix_LoadMUS(AUDIO_FILE);
   if(this->sound == NULL) {
      Alert(NULL, Mix_GetError(), NULL, 0);
      throw ERR_INIT_AUDIO_FILE;
   }
   // auto pause
   this->pause = 1;
   //Mix_PlayMusic(this->sound, -1);
}
*/

void Game::display()
{
   SDL_Rect rect;

   SDL_SetRenderDrawColor(this->renderer,
                          0, 0, 0,
                          SDL_ALPHA_OPAQUE);
   SDL_RenderClear(this->renderer);

   // ...
   // debug : draw arena
   SDL_SetRenderDrawColor(this->renderer,
                          123, 123, 123,
                          SDL_ALPHA_OPAQUE);
   rect.x = ARENA_X;
   rect.y = ARENA_Y;
   rect.h = PIXEL_SIZE * ARENA_SIZE_H;
   rect.w = PIXEL_SIZE * ARENA_SIZE_W;
   SDL_RenderDrawRect(this->renderer, &rect);

   // display monsters
   for (auto &m : this->monsters->getMonsters())
   {
      this->drawMonster(m);
   }

   // dislpay player
   this->drawPlayer();

   // display bullet
   this->drawBullet();

   // display scores
   this->displayScore();

   SDL_RenderPresent(this->renderer);
}

void Game::displayScore()
{
   ostringstream text;

   text.str("");
   text << "score : " << "11";
   this->displayText(text.str(), 1, 1);

   if (this->pauseStr.size() > 1)
   {
      text.str("");
      text << this->pauseStr;
      this->displayText(text.str(), 1, 1);
   }
}

void Game::displayText(string str, int x, int y)
{
   SDL_Surface * surface;
   SDL_Texture * texture;
   SDL_Rect      rect;

   surface = TTF_RenderText_Solid(this->font, str.c_str(), FONT_COLOR);
   //surface = TTF_RenderText_Blended_Wrapped(this->font, text.str().c_str(), FONT_COLOR, MATRIX_SPACE*TILE_S);
   texture = SDL_CreateTextureFromSurface(this->renderer, surface);

   // display texture
   rect.x = x;
   rect.y = y;
   rect.w = surface->w;
   rect.h = surface->h;
   SDL_RenderCopy(this->renderer, texture, NULL, &rect);
   SDL_FreeSurface(surface);
   SDL_DestroyTexture(texture);
}

void Game::drawMonster(Monster * monster)
{
   SDL_Rect destRect;

   destRect.x = ARENA_X + PIXEL_SIZE * (monster->getRect().x);
   destRect.y = ARENA_Y + PIXEL_SIZE * (monster->getRect().y);
   destRect.w = PIXEL_SIZE * (monster->getRect().w);
   destRect.h = PIXEL_SIZE * (monster->getRect().h);

   SDL_SetRenderDrawColor(this->renderer,
                          123, 123, 123,
                          SDL_ALPHA_OPAQUE);
   SDL_RenderFillRect(this->renderer, &destRect);
}

void Game::drawPlayer()
{
   SDL_Rect destRect;

   destRect.x = ARENA_X + PIXEL_SIZE * (this->player->getRect().x);
   destRect.y = ARENA_Y + PIXEL_SIZE * (this->player->getRect().y);
   destRect.w = PIXEL_SIZE * (this->player->getRect().w);
   destRect.h = PIXEL_SIZE * (this->player->getRect().h);

   SDL_SetRenderDrawColor(this->renderer,
                          123, 123, 123,
                          SDL_ALPHA_OPAQUE);
   SDL_RenderFillRect(this->renderer, &destRect);
}

void Game::drawBullet()
{
   if (!this->player->hasBullet()) return;

   SDL_Rect srcRect = *(this->player->getBulletpRect());
   SDL_Rect destRect;

   destRect.x = ARENA_X + PIXEL_SIZE * (srcRect.x);
   destRect.y = ARENA_Y + PIXEL_SIZE * (srcRect.y);
   destRect.w = PIXEL_SIZE * (srcRect.w);
   destRect.h = PIXEL_SIZE * (srcRect.h);

   SDL_SetRenderDrawColor(this->renderer,
                          123, 123, 123,
                          SDL_ALPHA_OPAQUE);
   SDL_RenderFillRect(this->renderer, &destRect);
}


int Game::play()
{
   int action;
   int p;

   // init pause
   p = this->actionPause(string("Press Enter to begin"));
   if (p == ACTION_QUIT) return ACTION_QUIT;

   unsigned int tic = SDL_GetTicks();
   unsigned int tac = 0;

   while (this->player->isAlive())
   {
      // Get player's action
      action = this->getAction();

      // Quit game
      if (action == ACTION_QUIT) return ACTION_QUIT;

      this->player->play(action);

      tac = SDL_GetTicks();
      if (tac - tic > (unsigned int)(BASE_SPEED * this->monsters->getMonstersCount()))
      {
         if (this->monsters->play())
         {
            this->player->kill();
         }
         tic = SDL_GetTicks();
      }

      // check impacts
      if (this->monsters->playerImpact(this->player->getpRect()))
      {
         this->player->kill();
      }

      // check impacts
      if (this->player->hasBullet()
       && this->monsters->bulletImpact(this->player->getBulletpRect()))
      {
         this->player->bulletHitTarget();
      }

      this->display();
      SDL_Delay(10);
   }
   /* GAME OVER */
   p = this->actionPause(string("GAME OVER"));
   return ACTION_QUIT;
}

int Game::getAction()
{
   int ret = ACTION_NONE;
   SDL_Event event;

   if (SDL_PollEvent(&event))
   {
      switch(event.type)
      {
         case SDL_WINDOWEVENT:
            // Set pause on focus lost
            if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
            {
               ret = this->actionPause(string("Pause..."));
            }
            break;
         case SDL_QUIT:
            ret = ACTION_QUIT;
            break;
         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
               case SDLK_ESCAPE:
                  ret = this->actionPause(string("Pause..."));
                  break;
               case SDLK_m:
                  /* music */
                  ret = ACTION_NONE;
                  break;
               case SDLK_p:
               case SDLK_LALT:
                  ret = this->actionPause(string("Pause..."));
                  break;
               case SDLK_UP:
                  ret = ACTION_MOVE_UP;
                  break;
               case SDLK_DOWN:
                  ret = ACTION_MOVE_DOWN;
                  break;
               case SDLK_RIGHT:
                  ret = ACTION_MOVE_RIGHT;
                  break;
               case SDLK_LEFT:
                  ret = ACTION_MOVE_LEFT;
                  break;
               case SDLK_SPACE:
                  ret = ACTION_SHOT;
                  break;
               default:
                  ret = ACTION_NONE;
                  break;

            }
            break;
      }
   }
   return ret;
}

int Game::actionPause(string str)
{
   int action = ACTION_NONE;
   SDL_Event event;

   this->pauseStr = str;
   this->display();

   // purge events in queue
   while(SDL_PollEvent(&event));

   // wait for end pause or quit
   while(action == ACTION_NONE)
   {
      SDL_WaitEvent(&event);
      switch(event.type)
      {
         case SDL_QUIT:
            action = ACTION_QUIT;
            break;
         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
               case SDLK_ESCAPE:
                  action = ACTION_QUIT;
                  break;
               case SDLK_p:
               case SDLK_RETURN:
                  action = ACTION_PAUSE;
                  break;
               case SDLK_m:
                  /* music */
                  action = ACTION_NONE;
                  break;
               default:
                  action = ACTION_NONE;
                  break;
            }
            break;
      }
      this->display();
      SDL_Delay(50);
   }
   this->pauseStr = "";
   return action;
}

/********************************
	accessors
********************************/



/********************************
	end accessors
********************************/
