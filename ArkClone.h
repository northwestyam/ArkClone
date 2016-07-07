#ifndef ARKCLONE_H_
#define ARKCLONE_H_

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

enum {
	GS_SPLASH,
	GS_RUNNING,
	GS_GAMEOVER,
	GS_PAUSED
};


struct Block
{
    SDL_Rect rect;
    bool alive;
    int frame;
};

struct Ball
{
    SDL_Rect rect;
    int xVel;
    int yVel;
    bool isLocked;
};

struct Player
{
    SDL_Rect rect;
    int lives;
};


class ArkClone {
public:
	ArkClone(void);
	bool InitGame();
	int RunGameLoop(void);
	bool InitSDL();
	void DrawText(SDL_Surface *surface, char *string, int x, int y, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b);
	bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2);

	void SetBlocks();
	void DrawBlocks();
	int NumBlocksLeft();
	void ResetGame();

	void UpdatePlayer();
	void UpdateBall();
	void UpdateSplash();
	void DrawSplash();
	void UpdateGame();
	void DrawGame();
	void DrawGamePaused();
	void UpdateGameOver();
	void DrawGameOver();
	void DrawScreen();
	void FreeGame();
	bool ProgramIsRunning();
	void RunGame();
	bool LoadFiles();
	void FreeFiles();

private:
	uint prevTime;					// variable to manage game loop refresh
	int frameTime;

};
#endif /* ARKCLONE_H_ */
