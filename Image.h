#ifndef IMAGE_H
#define IMAGE_H

#include <SDL/SDL.h>

class Image {
public:
	//Surfaces
	SDL_Surface *Backbuffer = NULL;
	SDL_Surface *BackgroundImage = NULL;
	SDL_Surface *BallImage = NULL;
	SDL_Surface *PlayerPaddleImage = NULL;
	SDL_Surface *BlockImage = NULL;
	SDL_Surface *SplashImage = NULL;
	SDL_Surface *GameoverImage = NULL;
	SDL_Surface *GamepausedImage = NULL;

public:
	void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
	void DrawImageFrame(SDL_Surface* image, SDL_Surface* destSurface, int x, int y, int width, int height, int frame);
	SDL_Surface* LoadImage(const char* fileName);
	bool LoadFiles();
	void FreeFiles();
};

#endif
