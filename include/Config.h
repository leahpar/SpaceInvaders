#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define WINDOW_TITLE "Space Invaders"

#define PIXEL_SIZE      3
#define MONSTER_SIZE_H  10
#define MONSTER_SIZE_W  12
#define BASE_SIZE       16

#define MONSTER_MARGIN  ((BASE_SIZE-MONSTER_SIZE_W)/2)

#define ARENA_X         (30)
#define ARENA_Y         (100)
#define ARENA_PADDING   (BASE_SIZE)
#define ARENA_SIZE_H    (BASE_SIZE*15)
#define ARENA_SIZE_W    (BASE_SIZE*15)

#define WINDOW_SIZE_H   ((PIXEL_SIZE * ARENA_SIZE_H)+2*ARENA_Y)
#define WINDOW_SIZE_W   ((PIXEL_SIZE * ARENA_SIZE_W)+2*ARENA_X)

#define LEFT -1
#define RIGHT 1

/**** MONSTERS ****************************/
#define NB_MONSTERS_H      5
#define NB_MONSTERS_W      12
#define MONSTER_MOVE_STEP  3
#define MONSTER_MOVE_LINE  8
#define BASE_SPEED         1

/**** BULLET ******************************/
#define BULLET_SIZE_H      4
#define BULLET_SIZE_W      2
#define BULLET_MOVE_STEP   3

/**** PLAYER ******************************/
#define PLAYER_MOVE_STEP   4

/**** RESSOURCES **************************/
#define FONT_SIZE 18
#define FONT_FILE "ressources/font.dat"
#define FONT_COLOR (SDL_Color){128,128,128}

#define AUDIO_FILE "ressources/music.dat"
#define AUDIO_RATE 22050
#define AUDIO_FORMAT AUDIO_S16SYS
/* mono */
#define AUDIO_CHANNELS 1
#define AUDIO_BUFFERS 4096

#define SCORE_FILENAME "scores.dat"
#define NB_HIGHSCORE 1

/**** USER ACTIONS ************************/
#define ACTION_NONE        -1
#define ACTION_QUIT        99
#define ACTION_PAUSE       98
#define ACTION_OVER        97
#define ACTION_MOVE_UP      0
#define ACTION_MOVE_DOWN    1
#define ACTION_MOVE_LEFT    2
#define ACTION_MOVE_RIGHT   3
#define ACTION_SHOT         4

#endif // CONFIG_H_INCLUDED
