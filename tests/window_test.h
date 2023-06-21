#ifndef _MAIN_H_
#define _MAIN_H_

#include "librairies.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
// #include <SDL2/SDL_image.h>

bool init(void);
bool initGL(void);
void close(void);

int main(int, char**);

#endif
