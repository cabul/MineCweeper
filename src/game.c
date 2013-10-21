#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "game.h"
#include <time.h>
#include <stdlib.h>

void setupGame(SDL_Renderer *ren, SDL_Texture *text, Cell cells[]) {
	SDL_RenderClear(ren);
	
	for(int i = 0; i < XSIZE; i++) {
		for(int j = 0; j < YSIZE; j++) {
			cells[i+j*XSIZE].img = WALL;
			cells[i+j*XSIZE].bomb = false;
			cells[i+j*XSIZE].adj = 0;
			cells[i+j*XSIZE].check = false;
			render(ren,text,WALL,i,j);
		}
	}

	setBombs(cells,BOMB_MAX);

	SDL_RenderPresent(ren);

}

int coin(int min, int max) {
	int offset = rand() % (max-min);
	return min+offset;
}

void render(SDL_Renderer* ren, SDL_Texture* text, int offset, int x, int y) {
	SDL_Rect srcR;
	SDL_Rect destR;

	srcR.x = offset*QUAD;
	srcR.y = 0;
	srcR.w = QUAD;
	srcR.h = QUAD;

	destR.x = x*QUAD;
	destR.y = y*QUAD;
	destR.w = QUAD;
	destR.h = QUAD;

	SDL_RenderCopy(ren,text,&srcR,&destR);
}

void renderAll(SDL_Renderer* ren, SDL_Texture* text, Cell cells[]) {
	SDL_RenderClear(ren);
	for(int i = 0; i < XSIZE; i++) {
		for(int j = 0; j < YSIZE; j++) {
			render(ren, text, cells[i+XSIZE*j].img,i,j);
		}
	}
	SDL_RenderPresent(ren);
}

bool mark(Cell *cell, int *marked, int *disc) {
	switch(cell->img) {
		case MARK: {
			cell->img = WALL;
			*marked -= 1;
			break;
		}
		case WALL: {
			if(*marked >= BOMB_MAX) break;
			cell->img = MARK;
			*marked+=1;
			if(cell->bomb) {
				*disc+=1;
			}
			printf("%d - %d\n", *disc,*marked);
			return true;
			break;
		}
	}
	printf("%d - %d\n", *disc,*marked);
	return false;
}

int checkCell(Cell cells[], int x, int y) {
	int result = 0;
	Cell *cell = &cells[x+y*XSIZE];
	if(cell->check) return 0;
	cell->img = cell->adj;
	cell->check = true;
	result++;
	if(cell->adj == 0) {
		int downX = x-1;
		int upX = x+1;
		int downY = y-1;
		int upY = y+1;
		for(int i=downX;i<=upX;i++) {
			for(int j=downY;j<=upY;j++) {
				if( (i>=0) &&
						(j>=0) &&
						(i<XSIZE) &&
						(j<YSIZE) ) {
					result+=checkCell(cells,i,j);
				}
			}
		}
	}
	return result;
}


bool click(Cell cells[], int x, int y) {
	Cell *cell = &cells[x+y*XSIZE];
	if(cell->bomb) {
		cell->img = BOMB;
		cell->check=true;
		return false;
	}
	return true;
}

void addAdj(Cell cells[], int x, int y) {
	int downX = x-1;
	if(downX<0) downX = 0;
	int downY = y-1;
	if(downY<0) downY = 0;
	int upX = x+1;
	if(upX >= XSIZE) upX = XSIZE-1;
	int upY = y+1;
	if(upY >= YSIZE) upY = YSIZE-1;	
	for(int i=downX; i<=upX; i++) {
		for(int j=downY; j<=upY; j++) {
			cells[i+j*XSIZE].adj++;
		}
	}
}

void setBombs(Cell cells[], int bombs) {
	for(int i=0; i < bombs; i++) {
		int x = coin(0,XSIZE);
		int y = coin(0,YSIZE);
		if(cells[x+y*XSIZE].bomb) {
			i--;
		} else {
			cells[x+y*XSIZE].bomb = true;
			addAdj(cells,x,y);
		} 
	}
}

int main(int argc, char** argv) 
{
	srand(time(NULL));

	if( SDL_Init(SDL_INIT_EVERYTHING) != 0 ) {
		printf("Initializing Error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *win = NULL;
	SDL_Renderer *ren = NULL;

	win = SDL_CreateWindow(TITLE,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH, HEIGHT, 0);
	if(!win) {
		printf("Window Error: %s\n", SDL_GetError());	
		return 1;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if(!ren) {
		printf("Renderer Error: %s\n", SDL_GetError());	
		return 1;
	}

	if(TTF_Init() == -1) {
		printf("TTF Error: %s\n", TTF_GetError());
		return 1;
	}

	SDL_Texture *sprites;

	sprites = IMG_LoadTexture(ren, "../images/bomb.png");

	Cell cells[CELLS_NUM];

	setupGame(ren,sprites,cells);

	SDL_Event event;
	bool running = true;

	bool lost = false;

	int undisc = CELLS_NUM;
	int marked = 0;
	int disc = 0;

	while(running) {
		if(SDL_PollEvent(&event)) {
			switch(event.type) {

				case SDL_QUIT: {
					running = false;
					break;
				}

				case SDL_KEYDOWN: {
					SDL_Keysym key = event.key.keysym;
					switch(key.sym){
						case SDLK_ESCAPE: {
							running = false;
							break;
						}
						case SDLK_n: {
							setupGame(ren,sprites,cells);
							undisc = CELLS_NUM;
						}

					}
					break;
				}	

				case SDL_MOUSEBUTTONDOWN: {
					SDL_MouseButtonEvent button = event.button;
					int x = button.x/QUAD;
					int y = button.y/QUAD;
					if(button.button == SDL_BUTTON_LEFT){
						lost = !click(cells,x,y);
						if(lost) {
							printf("%s\n", "Game Over!");
							renderAll(ren,sprites,cells);
							SDL_Delay(2000);
							setupGame(ren,sprites,cells);
							undisc = CELLS_NUM;
							marked = 0;
							disc = 0;
						} else {
							undisc -= checkCell(cells,x,y);
							if(undisc == BOMB_MAX) {
								printf("%s\n", "You won!");
								renderAll(ren,sprites,cells);
								SDL_Delay(2000);
								setupGame(ren,sprites,cells);
								undisc = CELLS_NUM;
								marked = 0;
								disc = 0;
							}
						} 
					} else if(button.button == SDL_BUTTON_RIGHT) {
						if(mark(&cells[x+y*XSIZE], &marked, &disc)) {

							if(disc == BOMB_MAX) {
								printf("%s\n", "You won!");
								renderAll(ren,sprites,cells);
								SDL_Delay(2000);
								setupGame(ren,sprites,cells);
								undisc = CELLS_NUM;
								marked = 0;
								disc = 0;	
							}
						}
					}
					renderAll(ren,sprites,cells);
				}
			}
		}
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);


	TTF_Quit();
	SDL_Quit();

	return 0;
}