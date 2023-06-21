#include "window_test.h"

int main(int argc, char** argv){
	assert(argc == 1);
	
	SDL_Window* window = NULL;
	int width =  640;
	int height = 360;
	
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0){
		fprintf(stderr, "Error initializing SDL : %s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	if(window == NULL){
		SDL_Quit();
		fprintf(stderr, "Error initializing window : %s\n", SDL_GetError());
		return 1;
	}
	SDL_Delay(3000);

	uint32_t render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);
	if(renderer == NULL){
		fprintf(stderr, "Error initializing renderer : %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	/* SDL_Surface* surface = IMG_Load("./test.png");
	if(surface == NULL){
		fprintf(stderr, "Error initializing surface : %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if(texture == NULL){
		fprintf(stderr, "Error initializing texture : %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_RenderClear(window);

	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(texture);
	*/
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}
