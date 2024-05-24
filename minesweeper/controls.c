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
				moveCamera(event.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_LEFT){
					DigTile(mouse.c);
				}
				if(event.button.button == SDL_BUTTON_RIGHT){
					PlaceFlag();
				}
			default :
				break;
		}
	}
}

void PlaceFlag(){
	//printf("mx %d my %d\n", mouse.x, mouse.y);
	mouse.c->flagged = !mouse.c->flagged;
}

void DigTile(cell* c){
	//printf("%d\n", mouse.c->content);
	c->dug = true;
	if(c->content == 0){
		//printf("true\n");
		for(int i = 0; i < mouse.c->type; i++){
			cell* t = c->move[i];
			if(t != NULL && !t->dug && !t->flagged){
				//printf("digging %d\n", t->state);
				DigTile(t);
			}
		}
	}
	return;
}

void moveCamera(SDL_Keycode i){
	float a = 0.05;
	if(i == SDLK_UP || i == SDLK_z){
		camera.c_position[1] += a;
	} else if(i == SDLK_DOWN || i == SDLK_s){
		camera.c_position[1] -= a;
	} else if(i == SDLK_LEFT || i == SDLK_q){
		camera.c_position[0] += a;
	} else if(i == SDLK_RIGHT || i == SDLK_d){
		camera.c_position[0] -= a;
	}
	//printf("(%.2f, %.2f)\n", camera.c_position[0], camera.c_position[1]);
}