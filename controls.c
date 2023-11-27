// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET

#include "controls.h"

void doInput(void){
	SDL_Event event;

	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT){
					moveCamera(event.key.keysym.sym);
				}
				printf("%s Pressed!\n", SDL_GetKeyName(event.key.keysym.sym));
				break;
			default :
				break;
		}
	}
}

void moveCamera(SDL_Keycode i){
	if(i == SDLK_UP){
		camera.c_position[1] += 0.05;
	} else if(i == SDLK_DOWN){
		camera.c_position[1] -= 0.05;
	} else if(i == SDLK_LEFT){
		camera.c_position[0] -= 0.05;
	} else if(i == SDLK_RIGHT){
		camera.c_position[0] += 0.05;
	}
	printf("Camera Position : (%.2f, %.2f)\n", camera.c_position[0], camera.c_position[1]);
}