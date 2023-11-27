// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET

#include "render.h"

void initTesselation(cell* g0, int range){
	Generate_RTS(g0, range - 1);

	//print_RTS(g0, 0);
	printf("\n\n");
}

/**
 * \fn SDL_Point flt_to_int_coordinates(double x, double y)
 * \brief Turns float coordinates to SDL_Point coodinates
 * \warning This function is still untested and might be incorrect
*/
SDL_Point flt_to_int_coordinates(double x, double y){
	int mid_x = SCREEN_WIDTH / 2;
	int mid_y = SCREEN_HEIGHT / 2;
	SDL_Point pt;
	pt.x = mid_x + x * 1000;
	pt.y = mid_y + y * 1000;
	return pt;
}

void add_to_render_queue(cell* g0, cell* c){
	//sleep(1);
	if(c == NULL){
		return;
	}
	//printf("\nCurrently in state q = %d\n", c->state);

	double* center = malloc(sizeof(double) * 3);
	assert(center != NULL);
	center[0] = 0; 
	center[1] = 0; 
	center[2] = 1;
	//printf("\nCooordinates for current tile center: (%.2f, %.2f, %.2f)\n", center[0], center[1], center[2]);
	tile_relative_coordinates_transform(c, g0, center);
	//printf("Now, current tile relative center coordinates are : (%.2f, %.2f, %.2f)\n", center[0], center[1], center[2]);
	SDL_Point pt = flt_to_int_coordinates(center[0]/(center[2] + 1) - camera.c_position[0], center[1]/(center[2] + 1) - camera.c_position[1]);
	
	SDL_SetRenderDrawColor(app.renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawPoint(app.renderer, pt.x, pt.y);
	
	free(center);
	for(int i = 0; i < c->type; i++){
		int x = transition_rules(*c, i);
		
		double a = center_to_edge_midpoint_distance(*c);
		double* mid_edge = malloc(sizeof(double) * 3);
		mid_edge[0] = cos(i * 2 * M_PI / c->type) * a;
		mid_edge[1] = sin(i * 2 * M_PI / c->type) * a;
		mid_edge[2] = sqrt(a*a + 1);

		//printf("Cooordinates for current edge midpoint : (%.2f, %.2f, %.2f)\n", mid_edge[0], mid_edge[1], mid_edge[2]);
		tile_relative_coordinates_transform(c, g0, center);
		//printf("Now, current tile relative edge midpoint coordinates are : (%.2f, %.2f, %.2f)\n", mid_edge[0], mid_edge[1], mid_edge[2]);
		
		SDL_Point pt = flt_to_int_coordinates(mid_edge[0]/(mid_edge[2] + 1) - camera.c_position[0], mid_edge[1]/(mid_edge[2] + 1) - camera.c_position[1]);
		
		SDL_SetRenderDrawColor(app.renderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawPoint(app.renderer, pt.x, pt.y);
		
		free(mid_edge);
		if(x > 0){
			add_to_render_queue(g0, c->move[i]);
		}
	}
}

void prepareScene(cell* g0){
	assert(g0);
	SDL_RenderClear(app.renderer);
	//SDL_RenderDrawLine(app.renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
	//SDL_RenderDrawLine(app.renderer, SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);
	add_to_render_queue(g0, g0);
	SDL_SetRenderDrawColor(app.renderer, 0x00, 0x00, 0x00, 0xFF);
}

void presentScene(){
	SDL_RenderPresent(app.renderer);
}

void initSDL(){
	int rendererFlags, windowFlags;
	
	rendererFlags = SDL_RENDERER_ACCELERATED;
	
	windowFlags = 0;
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("Couldn't initialize SDL : %s \n", SDL_GetError());
		exit(1);
	}

	app.window = SDL_CreateWindow("Test 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	if(!app.window){
		printf("Failed to open window : %s \n", SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

	if(!app.renderer){
		printf("Failed to create renderer : %s \n", SDL_GetError());
		exit(1);
	}
}

void cleanup(){
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	SDL_Quit();
}

// ....

// initialize -> create the RTS 
// update -> apply tranformations to scene (transformations to each individual point) with C0 set
// then translate all coordinates to SDL_point structures