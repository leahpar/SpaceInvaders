#include <unistd.h>
#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include "Config.h"
#include "Device.h"
#include "Data.h"
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

   this->player   = new Player();
   this->level = 1;

   // UI
   for (int i=0; i<SDL_MAX_KEY; i++)
      this->key[i] = 0;
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
   int i;

   // clear screen
   SDL_SetRenderDrawColor(this->renderer,
                          0, 0, 0,
                          SDL_ALPHA_OPAQUE);
   SDL_RenderClear(this->renderer);

   // draw arena
   /*
   SDL_SetRenderDrawColor(this->renderer,
                          123, 123, 123,
                          SDL_ALPHA_OPAQUE);
   rect.x = ARENA_X;
   rect.y = ARENA_Y;
   rect.h = PIXEL_SIZE * ARENA_SIZE_H;
   rect.w = PIXEL_SIZE * ARENA_SIZE_W;
   SDL_RenderDrawRect(this->renderer, &rect);
   */

   // Draw bottom line
   SDL_SetRenderDrawColor(this->renderer,
                          0, 200, 0,
                          SDL_ALPHA_OPAQUE);
   rect.x = ARENA_X;
   rect.y = ARENA_BOTTOM;
   rect.h = PIXEL_SIZE;
   rect.w = PIXEL_SIZE * ARENA_SIZE_W;
   SDL_RenderFillRect(this->renderer, &rect);

   // draw monsters
   int tileType = this->monsters->getTileType();
   for (auto &m : this->monsters->getMonsters())
   {
      this->drawMonster(m, tileType);
   }

   // draw player
   this->drawPlayer(this->player->getRect());

   // draw lives
   for (i=0; i<this->player->getLives(); i++)
   {
      rect = this->player->getRect();
      rect.x = BASE_SIZE * i + 42;
      rect.y = ARENA_SIZE_H + 2;
      this->drawPlayer(rect);
   }

   // draw bullet
   this->drawBullet();

   // display scores
   this->displayScore();

   SDL_RenderPresent(this->renderer);
}

void Game::displayScore()
{
   ostringstream text;

   text.str("");
   text << "Score " << this->player->getScore();
   this->displayText(text.str(), ARENA_X, 10);

   text.str("");
   text << "Lives ";
   this->displayText(text.str(), ARENA_X, ARENA_BOTTOM+10);

   text.str("");
   text << "Level " << this->level;
   this->displayText(text.str(), ARENA_RIGHT-150, ARENA_BOTTOM+10);


   if (this->pauseStr.size() > 1)
   {
      text.str("");
      text << this->pauseStr;
      this->displayText(text.str(), ARENA_X+PIXEL_SIZE*ARENA_SIZE_W/3, ARENA_Y+PIXEL_SIZE*ARENA_SIZE_H*2/3);
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

void Game::drawMonster(Monster * monster, int tileType)
{
   SDL_Rect destRect;
   SDL_Rect mRect = monster->getRect();

   // draw hitbox
   /*
   destRect.x = ARENA_X + PIXEL_SIZE * (mRect.x);
   destRect.y = ARENA_Y + PIXEL_SIZE * (mRect.y);
   destRect.w = PIXEL_SIZE * (mRect.w);
   destRect.h = PIXEL_SIZE * (mRect.h);

   SDL_SetRenderDrawColor(this->renderer,
                          42, 42, 42,
                          SDL_ALPHA_OPAQUE);
   SDL_RenderFillRect(this->renderer, &destRect);
   */

   // draw mob

   char * c;
   int i = 0;
   for (c=monster->getTile(tileType); *c!='\0'; c++)
   {
      SDL_SetRenderDrawColor(this->renderer,
                             (*c=='r'?255:0),
                             (*c=='g'?255:0),
                             (*c=='b'?255:0),
                             SDL_ALPHA_OPAQUE);
      destRect.x = ARENA_X + PIXEL_SIZE * (mRect.x + (int)i%MONSTER_SIZE_W);
      destRect.y = ARENA_Y + PIXEL_SIZE * (mRect.y + (int)i/MONSTER_SIZE_W);
      destRect.w = PIXEL_SIZE;
      destRect.h = PIXEL_SIZE;
      SDL_RenderFillRect(this->renderer, &destRect);
      i++;
   }
}

void Game::drawPlayer(SDL_Rect playerRect)
{
   SDL_Rect destRect;

   // draw hitbox
   /*
   destRect.x = ARENA_X + PIXEL_SIZE * (playerRect.x);
   destRect.y = ARENA_Y + PIXEL_SIZE * (playerRect.y);
   destRect.w = PIXEL_SIZE * (playerRect.w);
   destRect.h = PIXEL_SIZE * (playerRect.h);

   SDL_SetRenderDrawColor(this->renderer,
                          123, 123, 123,
                          SDL_ALPHA_OPAQUE);
   SDL_RenderFillRect(this->renderer, &destRect);
   */

   char * c;
   char tile[] = PLAYER_TILE;
   int i = 0;
   for (c=tile; *c!='\0'; c++)
   {
      SDL_SetRenderDrawColor(this->renderer,
                             (*c=='r'?255:0),
                             (*c=='g'?255:0),
                             (*c=='b'?255:0),
                             SDL_ALPHA_OPAQUE);
      destRect.x = ARENA_X + PIXEL_SIZE * (playerRect.x + (int)i%MONSTER_SIZE_W);
      destRect.y = ARENA_Y + PIXEL_SIZE * (playerRect.y + (int)i/MONSTER_SIZE_W);
      destRect.w = PIXEL_SIZE;
      destRect.h = PIXEL_SIZE;
      SDL_RenderFillRect(this->renderer, &destRect);
      i++;
   }
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
   bool killed = false;
   bool start  = false;

   unsigned int tic = 0;
   unsigned int tac = 0;
   int speed;

   // init pause
   this->pauseStr = string("Press space to begin");
   this->pauseGame = 2;


   // init game
   /*
   while (this->pauseGame > 0)
   {
      this->UIHandleAction();
   }
   */

   while (this->player->isAlive())
   {
      if (!killed)
      {
         // new level
         this->monsters = new MonsterManager();
      }
      else
      {
         this->monsters->resetMonsters();
      }

      // init player
      this->player->init();
      killed = false;
      start  = false;

      tic = SDL_GetTicks();
      while (!killed && this->monsters->getMonstersCount() > 0)
      {
         // Get player's action
         action = this->UIHandleAction();

         // Quit game
         if (action == ACTION_QUIT)
            return ACTION_QUIT;

         // Pause game
         if (this->pauseGame == 0)
         {

            if (this->key[KEY_UP])
               this->player->play(ACTION_MOVE_UP);
            else if (this->key[KEY_DOWN])
               this->player->play(ACTION_MOVE_DOWN);
            if (this->key[KEY_LEFT])
               this->player->play(ACTION_MOVE_LEFT);
            else if (this->key[KEY_RIGHT])
               this->player->play(ACTION_MOVE_RIGHT);

            if (this->key[KEY_SPACE])
            {
               this->player->play(ACTION_SHOT);
               start = true;
            }

            this->player->play(ACTION_BULLET);

            tac = SDL_GetTicks();

            speed = (start) ? (BASE_SPEED * this->monsters->getMonstersCount())
                            : (BASE_SPEED * NB_MONSTERS_W * NB_MONSTERS_H);
            if (tac - tic > (unsigned int)speed)
            {
               // move monsters
               if (this->monsters->play())
               {
                  // a monster reached the floor
                  this->player->kill();
                  killed = true;
               }
               tic = SDL_GetTicks();
            }

            // Check if a monster killed the player
            if (this->monsters->playerImpact(this->player->getpRect()))
            {
               this->player->kill();
               killed = true;
            }

            // check if the bullet killed a monster
            if (this->player->hasBullet()
             && this->monsters->bulletImpact(this->player->getBulletpRect()))
            {
               this->player->bulletHitTarget(MONSTER_SCORE * level);
            }
         }
         this->display();
         SDL_Delay(10);

      } // end level
      if (!killed) this->level++;
   } // game over

   /* GAME OVER */
   //this->actionPause(string("GAME OVER"));
   this->pauseStr = string("GAME OVER");
   this->display();
   return ACTION_OVER;
}



