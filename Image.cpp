#include "Image.h"

void Image::DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    SDL_BlitSurface( image, NULL, destSurface, &destRect);
}

void Image::DrawImageFrame(SDL_Surface* image, SDL_Surface* destSurface, int x, int y, int width, int height, int frame)
{
    //Draws a specific frame from an image
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    int columns = image->w/width;

    SDL_Rect sourceRect;
    sourceRect.y = (frame/columns)*height;
    sourceRect.x = (frame%columns)*width;
    sourceRect.w = width;
    sourceRect.h = height;

    SDL_BlitSurface( image, &sourceRect, destSurface, &destRect);
}

SDL_Surface* Image::LoadImage(const char* fileName)
{
    SDL_Surface* imageLoaded = NULL;
    SDL_Surface* processedImage = NULL;

    imageLoaded = SDL_LoadBMP(fileName);

    if(imageLoaded != NULL)
    {
        processedImage = SDL_DisplayFormat(imageLoaded);
        SDL_FreeSurface(imageLoaded);

        if( processedImage != NULL )
        {
            Uint32 colorKey = SDL_MapRGB( processedImage->format, 0xFF, 0, 0xFF );
            SDL_SetColorKey( processedImage, SDL_SRCCOLORKEY, colorKey );
        }

    }
    return processedImage;
}

bool Image::LoadFiles()
{
	//Load images
	BackgroundImage = LoadImage("graphics/background.bmp");
	SplashImage = LoadImage("graphics/splash.bmp");
	GameoverImage = LoadImage("graphics/gameover.bmp");
	GamepausedImage = LoadImage("graphics/gamepaused.bmp");
	BallImage = LoadImage("graphics/ball.bmp");
	PlayerPaddleImage = LoadImage("graphics/player.bmp");
	BlockImage = LoadImage("graphics/blocks.bmp");

	//Error checking images
	if(BackgroundImage == NULL)
		return false;
	if(SplashImage == NULL)
		return false;
	if(GameoverImage == NULL)
		return false;
	if(GamepausedImage == NULL)
		return false;
	if(BallImage == NULL)
		return false;
	if(PlayerPaddleImage == NULL)
		return false;
	if(BlockImage == NULL)
		return false;
	return true;
}

void Image::FreeFiles()
{
	//Free images
	SDL_FreeSurface(BackgroundImage);
	SDL_FreeSurface(SplashImage);
	SDL_FreeSurface(GameoverImage);
	SDL_FreeSurface(GamepausedImage);
	SDL_FreeSurface(BallImage);
	SDL_FreeSurface(PlayerPaddleImage);
	SDL_FreeSurface(BallImage);
}
