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
	pt.x = mid_x + x * 300;
	pt.y = mid_y + y * 300;
	return pt;
}

void DrawEdge(double* a, double* b){
	double ax = a[0] / (a[2] + 1);
	double ay = a[1] / (a[2] + 1);

	double bx = b[0] / (b[2] + 1);
	double by = b[1] / (b[2] + 1);

	double dist = sqrt((ax - bx)*(ax - bx) + (ay - by)*(ay - by));

	if(dist < RADIUS){
		SDL_Point a_p = flt_to_int_coordinates(ax - camera.c_position[0], ay - camera.c_position[1]);
		SDL_Point b_p = flt_to_int_coordinates(bx - camera.c_position[0], by - camera.c_position[1]);

		SDL_RenderDrawLine(app.renderer, a_p.x, a_p.y, b_p.x, b_p.y);
	} else {
		double mid[3];
		double mk = minkowski_InnerProduct(a, b);

		mid[0] = (a[0] + b[0]) / mk;
		mid[1] = (a[1] + b[1]) / mk;
		mid[2] = (a[2] + b[2]) / mk;

		DrawEdge(a, mid);
		DrawEdge(mid, b);
	}
}

void add_to_render_queue(cell* g0, cell* c){
	// sleep(1);
	if(c == NULL){
		return;
	}
	// printf("\nCurrently in state q = %d\n", c->state);

	double center[3];
	center[0] = 0; 
	center[1] = 0; 
	center[2] = 1;

	tile_relative_coordinates_transform(c, g0, center);
	// printf("Now, current tile relative center coordinates are : (%.2f, %.2f, %.2f)\n", center[0], center[1], center[2]);
	SDL_Point ct = flt_to_int_coordinates(center[0]/(center[2] + 1) - camera.c_position[0], center[1]/(center[2] + 1) - camera.c_position[1]);
	
	SDL_SetRenderDrawColor(app.renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawPoint(app.renderer, ct.x, ct.y);
	
	for(int i = 0; i < c->type; i++){
		int x = transition_rules(*c, i);
		
		double a = center_to_edge_midpoint_distance(*c);
		double v = center_to_vertices_distance(*c);

		double mid_edge[3];
		mid_edge[0] = cos(i * 2 * M_PI / c->type) * a;
		mid_edge[1] = sin(i * 2 * M_PI / c->type) * a;
		mid_edge[2] = sqrt(mid_edge[0]*mid_edge[0] + mid_edge[1]*mid_edge[1] + 1);

		double vert1[3];
		double vert2[3];
		vert1[0] = cos((2*i+1) * M_PI / c->type) * v;
		vert1[1] = sin((2*i+1) * M_PI / c->type) * v;
		vert1[2] = sqrt(v*v + 1);

		vert2[0] = cos((2*i-1) * M_PI / c->type) * v;
		vert2[1] = sin((2*i-1) * M_PI / c->type) * v;
		vert2[2] = sqrt(v*v + 1);

		// printf("Cooordinates for  %d-th edge midpoint : (%.2f, %.2f, %.2f)\n", i + 1, mid_edge[0], mid_edge[1], mid_edge[2]);
		tile_relative_coordinates_transform(c, g0, mid_edge);
		tile_relative_coordinates_transform(c, g0, vert1);
		tile_relative_coordinates_transform(c, g0, vert2);
		// printf("Now, current tile relative edge midpoint coordinates are : (%.2f, %.2f, %.2f)\n", mid_edge[0], mid_edge[1], mid_edge[2]);

		SDL_Point edge = flt_to_int_coordinates(mid_edge[0]/(mid_edge[2] + 1) - camera.c_position[0], mid_edge[1]/(mid_edge[2] + 1) - camera.c_position[1]);
		//SDL_Point v1 = flt_to_int_coordinates(vert1[0]/(vert1[2] + 1) - camera.c_position[0], vert1[1]/(vert1[2] + 1) - camera.c_position[1]);
		//SDL_Point v2 = flt_to_int_coordinates(vert2[0]/(vert2[2] + 1) - camera.c_position[0], vert2[1]/(vert2[2] + 1) - camera.c_position[1]);
		
		SDL_SetRenderDrawColor(app.renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderDrawPoint(app.renderer, edge.x, edge.y);

		SDL_SetRenderDrawColor(app.renderer, 0x00, 0x00, 0xFF, 0xFF);
		//SDL_RenderDrawLine(app.renderer, ct.x, ct.y, edge.x, edge.y);

		SDL_SetRenderDrawColor(app.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		//SDL_RenderDrawLine(app.renderer, v1.x, v1.y, v2.x, v2.y);
		DrawEdge(vert1, vert2);

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
	//SDL_SetRenderDrawColor(app.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
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