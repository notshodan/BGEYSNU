// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET

#include "main.h"

int main(int argc, char** argv){
    assert(argc == 3);

	int type = atoi(argv[1]);
	int range = atoi(argv[2]);

	printf("\nType : %d \nRange : %d \n\n", type, range);

	assert(type == 5 || type == 4 || type == 7);
    assert(range > 0);
	
	// ------ //
	
	cell* g0 = NewTile(type, 0);
	
	initTesselation(g0, range);

	memset(&camera, 0, sizeof(Camera));
	
	memset(&app, 0, sizeof(App));
	
	double* camera_pos = malloc(sizeof(double) * 2);
	camera.c = g0;
	camera.c_position = camera_pos;

	initSDL();

	while(1){
		prepareScene(g0);
		doInput();
		presentScene();
		SDL_Delay(16);
	}
	
	atexit(cleanup);

	free(camera_pos);
	free_tile(g0);
	assert(g0);
    return 0;
}