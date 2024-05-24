// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET

#include "main.h"

Camera camera;
App app;
int TESSELATION;
Mouse mouse;

// initialisation d'une partie
/*
 * d'abord on génère un terrain, chaque case n'est pas creusée, et n'a pas de contenu
 * on clique une première fois sur une case
 * cette case a un contenu fixé à 0
 * on creuse cette case comme si c'était normal
 * puis le jeu continue normalement
 * 
 * ATTENTION BUG: LES CASES AU BORD NE FONCTIONNENT PAS
*/

int main(int argc, char** argv){
    assert(argc == 3);

	srand(time(NULL));

	int type = atoi(argv[1]);
	int range = atoi(argv[2]);

	printf("\nType : %d \nRange : %d \n\n", type, range);

	assert(type == 5 || type == 4 || type == 7);
    assert(range > 0);
	
	// ------ //
	
	cell* g0 = NewTile(type, 0);
	Generate_RTS(g0, range);

	int TILE_NB = TileCount(g0);
	vector* v = initVector(TILE_NB, g0);

	memset(&camera, 0, sizeof(Camera));
	memset(&app, 0, sizeof(App));
	memset(&mouse, 0, sizeof(Mouse));
	
	double* camera_pos = malloc(sizeof(double) * 2);
	camera.c = g0;
	camera.c_position = camera_pos;
	mouse.c = g0;
	mouse.v = v;

	// terrain generation with constraint

	cell* CONSTRAINT;
	CONSTRAINT = g0;

	setTileContent(v, CONSTRAINT);
	//print_RTS(g0, 0);
	initSDL();

	app.entities[0] = loadTexture("assets/empty.png");
	app.entities[1] = loadTexture("assets/1.png");
	app.entities[2] = loadTexture("assets/2.png");
	app.entities[3] = loadTexture("assets/3.png");
	app.entities[4] = loadTexture("assets/4.png");
	app.entities[5] = loadTexture("assets/5.png");
	app.entities[6] = loadTexture("assets/6.png");
	app.entities[7] = loadTexture("assets/7.png");
	app.entities[8] = loadTexture("assets/8.png");
	app.entities[9] = loadTexture("assets/9.png");
	app.entities[10] = loadTexture("assets/10.png");
	app.entities[11] = loadTexture("assets/flag.png");
	app.entities[12] = loadTexture("assets/mine.png");

	while(1){
		prepareScene(range);
		doInput();
		presentScene();
		SDL_Delay(16);
	}
	
	atexit(cleanup);

	freeVect(v);
	free(camera_pos);
	free_tile(g0);
	assert(v);
	assert(g0);
    return 0;
}