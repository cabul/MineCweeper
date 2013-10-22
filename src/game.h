#ifndef _GAME_H
#define _GAME_H

#define TITLE "Mine Cweeper"
#define QUAD 32
#define QORG 64
#define XSIZE 20
#define YSIZE 20
#define WIDTH XSIZE*QUAD
#define HEIGHT YSIZE*QUAD
#define CELLS_NUM XSIZE*YSIZE
#define BOMB_MAX CELLS_NUM/10
#define BOMB 9
#define WALL 10
#define MARK 11


typedef struct{
	int img;
	bool bomb;
	int adj;
	bool check;
} Cell;

void setupGame(SDL_Renderer *ren, SDL_Texture *text, Cell cells[], int x, int y);
int coin(int min, int max);
void render(SDL_Renderer* ren, SDL_Texture* text, int offset, int x, int y);
void renderAll(SDL_Renderer* ren, SDL_Texture* text, Cell cells[]);
int checkCell(Cell cells[],int x, int y);
bool click(Cell cells[], int x, int y );
bool mark(Cell *cell, int *marked, int *disc);
void addAdj(Cell cells[], int x, int y);
void setBombs(Cell cells[],int bombs,int x, int y);

#endif