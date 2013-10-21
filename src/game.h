#ifndef _GAME_H
#define _GAME_H

#define TITLE "Mine Cweeper"
#define WIDTH 384
#define HEIGHT 384
#define QUAD 64
#define XSIZE WIDTH / QUAD
#define YSIZE HEIGHT / QUAD
#define CELLS_NUM XSIZE*YSIZE
#define BOMB_MAX 2
#define BOMB 9
#define WALL 10
#define MARK 11


typedef struct{
	int img;
	bool bomb;
	int adj;
	bool check;
} Cell;

void setupGame(SDL_Renderer *ren, SDL_Texture *text, Cell cells[]);
int coin(int min, int max);
void render(SDL_Renderer* ren, SDL_Texture* text, int offset, int x, int y);
void renderAll(SDL_Renderer* ren, SDL_Texture* text, Cell cells[]);
int checkCell(Cell cells[],int x, int y);
bool click(Cell cells[], int x, int y );
bool mark(Cell *cell, int *marked, int *disc);
void addAdj(Cell cells[], int x, int y);
void setBombs(Cell cells[],int bombs);

#endif