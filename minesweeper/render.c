// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET

#include "render.h"

/**
 * \fn SDL_Point flt_to_int_coordinates(double x, double y)
 * \brief Turns float coordinates to SDL_Point coodinates
*/
SDL_Point flt_to_int_coordinates(double x, double y){
	int mid_x = SCREEN_WIDTH / 2;
	int mid_y = SCREEN_HEIGHT / 2;
	SDL_Point pt;
	int M = MIN(mid_x, mid_y) * 0.95;
	pt.x = mid_x + x * M;
	pt.y = mid_y + y * M;
	return pt;
}

int Dist(SDL_Point p, int x, int y){
	return abs(p.x - x) + abs(p.y - y);
}

void DrawEdge(double* a, double* b){
	double ax = a[0] / (a[2] + 1);
	double ay = a[1] / (a[2] + 1);

	double bx = b[0] / (b[2] + 1);
	double by = b[1] / (b[2] + 1);

	double dist = sqrt((ax - bx)*(ax - bx) + (ay - by)*(ay - by));

	if(dist < RADIUS){
		SDL_Point a_p = flt_to_int_coordinates(ax, ay);
		//SDL_Point b_p = flt_to_int_coordinates(bx, by);
		//SDL_RenderDrawLine(app.renderer, a_p.x, a_p.y, b_p.x, b_p.y);
		SDL_RenderDrawPoint(app.renderer, a_p.x, a_p.y);
	} else {
		//sleep(1);
		double mid[3];

		double a_b[3]; a_b[0] = a[0] + b[0]; a_b[1] = a[1] + b[1]; a_b[2] = a[2] + b[2];
		double mk = sqrt(-minkowski_InnerProduct(a_b, a_b));

		mid[0] = (a[0] + b[0]) / mk;
		mid[1] = (a[1] + b[1]) / mk;
		mid[2] = (a[2] + b[2]) / mk;

		//printf("Midpoint of a and b is m = (%.2f, %.2f, %.2f)\n", mid[0], mid[1], mid[2]);

		DrawEdge(a, mid);
		DrawEdge(mid, b);
	}
}

void add_to_render_queue(cell* g0, cell* c){
	// sleep(1);
	if(c == NULL){
		return;
	}

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

		lboost(camera.c_position[0], 0, mid_edge);
		lboost(camera.c_position[1], 1, mid_edge);

		lboost(camera.c_position[0], 0, vert1);
		lboost(camera.c_position[1], 1, vert1);

		lboost(camera.c_position[0], 0, vert2);
		lboost(camera.c_position[1], 1, vert2);

		// printf("Now, current tile relative edge midpoint coordinates are : (%.2f, %.2f, %.2f)\n", mid_edge[0], mid_edge[1], mid_edge[2]);

		SDL_SetRenderDrawColor(app.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		if(mouse.c == c){
			SDL_SetRenderDrawColor(app.renderer, 0x00, 0xFF, 0x00, 0xFF);
		} 
		DrawEdge(vert1, vert2);


		if(x > 0){
			add_to_render_queue(g0, c->move[i]);
		}
	}
}

void blit(vector* v){
	cell* g0 = camera.c;
	for(int i = 0; i < mouse.v->n; i++){
		cell* c = v->c[i];
		double center[3];
		center[0] = 0;
		center[1] = 0;
		center[2] = 1;

		tile_relative_coordinates_transform(c, g0, center);
		lboost(camera.c_position[0], 0, center);
		lboost(camera.c_position[1], 1, center);


		double cx = center[0] / (center[2] + 1);
		double cy = center[1] / (center[2] + 1);
		SDL_Point p = flt_to_int_coordinates(cx, cy);
		c->center = p;
		
		// sets mouse center
		if(Dist(p, mouse.x, mouse.y) < Dist(mouse.c->center, mouse.x, mouse.y)){
			mouse.c = c;
		}
		//SDL_SetRenderDrawColor(app.renderer, 0xFF, 0xFF, 0x00, 0xFF);
		//SDL_RenderDrawPoint(app.renderer, p.x, p.y);

		if(c->dug || c->flagged){
			SDL_Texture* texture = textureTable(c->content);
			if(c->flagged){
				texture =  app.entities[11];
			}

			SDL_Rect dest;

			dest.x = c->center.x - 25;
			dest.y = c->center.y - 25;

			SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

			SDL_RenderCopy(app.renderer, texture, NULL, &dest);
		}
	}
}

SDL_Texture* loadTexture(char* filename){
	SDL_Texture* t;
	t = IMG_LoadTexture(app.renderer, filename);
	return t;
}

SDL_Texture* textureTable(int i){
	if(i >= 0 && i < 11){
		return app.entities[i];
	}
	else if(i == -1){
		return app.entities[12];
	}
	else {
		return app.entities[0];
	}
}

void prepareScene(int range){
	assert(camera.c);
	SDL_RenderClear(app.renderer);
	SDL_GetMouseState(&mouse.x, &mouse.y);
	blit(mouse.v); 
	//mouse.c = minDist(mouse.v, mouse.x, mouse.y);
	//mouse.c = minDist(mouse.v, mouse.x, mouse.x);
	Generate_RTS(camera.c, range);
	add_to_render_queue(camera.c, camera.c);
	SDL_RenderDrawPoint(app.renderer, mouse.c->center.x, mouse.c->center.y);
	//SDL_SetRenderDrawColor(app.renderer, 0xFF, 0x00, 0x00, 0xFF);
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

	app.window = SDL_CreateWindow("BADBADENGINE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

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

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}

void cleanup(){
	IMG_Quit();
	SDL_FreeSurface(app.surface);
	for(int i = 0; i < 10; i++){
		SDL_DestroyTexture(app.entities[i]);
	}
	SDL_DestroyTexture(app.texture);
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	SDL_Quit();
}

// ....

// initialize -> create the RTS 
// update -> apply tranformations to scene (transformations to each individual point) with C0 set
// then translate all coordinates to SDL_point structures