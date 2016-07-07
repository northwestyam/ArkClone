#include "ArkClone.h"
#include "Image.h"
#include "Sound.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;
const int PADDLE_Y = 550;
const int BALL_WIDTH = 20;
const int BALL_HEIGHT = 20;
const int BALL_SPEED = 10;

const int PLAYER_SPEED = 10;

const int FPS = 30;

const int GAMEAREA_X1 = 20;
const int GAMEAREA_Y1 = 20;
const int GAMEAREA_X2 = 598;
const int GAMEAREA_Y2 = 600;

const int BLOCK_COLUMNS = 11;
const int BLOCK_ROWS = 5;
const int BLOCK_WIDTH = 50;
const int BLOCK_HEIGHT = 25;

TTF_Font *GameFont = NULL;

//Game Variables
Block blocks[BLOCK_COLUMNS * BLOCK_ROWS];
Player player;
Ball ball;
int gameState;

Image image;
Sound sound;

ArkClone::ArkClone(void)
{
	if(!InitGame())
	{
		FreeGame();   //If InitGame failed, kill the program
		//return 0;
	}
}

bool ArkClone::InitGame()
{
    if(!InitSDL())
    {
    	return false;
    }
    if(!LoadFiles())
    {
    	return false;
    }
    if(!image.LoadFiles())
    {
    	return false;
    }
    if(!sound.LoadFiles())
	{
    	return false;
	}
    SDL_WM_SetCaption("Super Arkanoid Clone",NULL);
    Mix_PlayMusic(sound.GameMusic, -1);
    gameState = GS_SPLASH;

    return true;
}

bool ArkClone::InitSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
    	return false;
    }
    if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1)
    {
        return false;
    }
    if(TTF_Init() == -1)
    {
        return false;
    }
    image.Backbuffer = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE );
    if(image.Backbuffer == NULL)
    {
    	return false;
    }
    return true;
}

int ArkClone::RunGameLoop(void)
{
	while(ProgramIsRunning())
	{
		prevTime = SDL_GetTicks();
		SDL_FillRect(image.Backbuffer, NULL, 0);
		RunGame();
		DrawScreen();
		frameTime = SDL_GetTicks() - prevTime;
		if(frameTime < (1000 / FPS)) // 1000ms / frames per second
		{
			SDL_Delay((1000 / FPS) - frameTime);
		}
		SDL_Flip(image.Backbuffer);
	}
	FreeGame();
	return 0;
}

bool ArkClone::ProgramIsRunning()
{
    SDL_Event event;
    bool running = true;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
        	running = false;
        }

        if( event.type == SDL_KEYDOWN )
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
            	if(gameState == GS_RUNNING)
            	{
            		gameState = GS_PAUSED;
            	}
                else if(gameState == GS_PAUSED)
                {
                	gameState = GS_RUNNING;
                }
            }
        }
    }
    return running;
}

bool ArkClone::RectsOverlap(SDL_Rect rect1, SDL_Rect rect2)
{
    if(rect1.x >= rect2.x+rect2.w)
        return false;

    if(rect1.y >= rect2.y+rect2.h)
        return false;

    if(rect2.x >= rect1.x+rect1.w)
        return false;

    if(rect2.y >= rect1.y+rect1.h)
        return false;

    return true;
}

void ArkClone::SetBlocks()
{
    int i = 0;

    for(int x = 0; x < BLOCK_COLUMNS; x++)
    {
        for(int y = 0; y < BLOCK_ROWS; y++)
        {
            blocks[i].rect.x = x * BLOCK_WIDTH + GAMEAREA_X1 + x*3;
            blocks[i].rect.y = (y*2) * BLOCK_HEIGHT + GAMEAREA_Y1 + y*3;
            blocks[i].rect.w = BLOCK_WIDTH;
            blocks[i].rect.h = BLOCK_HEIGHT;
            blocks[i].alive = true;
            blocks[i].frame = i%4;
            i++;
        }
    }
}

void ArkClone::DrawBlocks()
{
    for(int i = 0; i < BLOCK_COLUMNS*BLOCK_ROWS; i++)
    {
        if(blocks[i].alive)
        {
            image.DrawImageFrame(image.BlockImage, image.Backbuffer, blocks[i].rect.x, blocks[i].rect.y, blocks[i].rect.w, blocks[i].rect.h, blocks[i].frame);
        }
    }
}

int ArkClone::NumBlocksLeft()
{
    int result = 0;

    for(int i = 0; i < BLOCK_COLUMNS*BLOCK_ROWS; i++)
    {
        if(blocks[i].alive)
        {
            result++;
        }
    }
    return result;
}

void ArkClone::ResetGame()
{
    //Position the player's paddle
    player.rect.x = (GAMEAREA_X2-GAMEAREA_X1)/2 - PADDLE_WIDTH/2 + GAMEAREA_X1;
    player.rect.y = PADDLE_Y;
    player.rect.w = PADDLE_WIDTH;
    player.rect.h = PADDLE_HEIGHT;

    //Position the ball
    ball.rect.x = SCREEN_WIDTH/2 - BALL_WIDTH/2;
    ball.rect.y = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
    ball.rect.w = BALL_WIDTH;
    ball.rect.h = BALL_HEIGHT;

    //Play the spawn sound
    Mix_PlayChannel(-1, sound.BallSpawnSound, 0);

    //Set blocks
    SetBlocks();
    ball.isLocked = true;
    player.lives = 3;
}

void ArkClone::UpdatePlayer()
{
    Uint8 *keys = SDL_GetKeyState(NULL);

    //Move the paddle when the left/right key is pressed
    if(keys[SDLK_LEFT])
        player.rect.x -= PLAYER_SPEED;

    if(keys[SDLK_RIGHT])
        player.rect.x += PLAYER_SPEED;

    if(keys[SDLK_SPACE] && ball.isLocked)
    {
        ball.isLocked = false;
        ball.xVel = rand()%3 - 1;
        ball.yVel = BALL_SPEED;
    }

    //Make sure the paddle doesn't leave the screen
    if(player.rect.x < GAMEAREA_X1)
        player.rect.x = GAMEAREA_X1;

    if(player.rect.x > GAMEAREA_X2-player.rect.w)
        player.rect.x = GAMEAREA_X2-player.rect.w;
}

void ArkClone::UpdateBall()
{
    if(ball.isLocked) //If the ball is locked, position it on top of the paddle and cenetred on the X axis
    {
        int PaddleCenterX = player.rect.x + player.rect.w/2;
        ball.rect.x = PaddleCenterX - ball.rect.w/2;
        ball.rect.y = player.rect.y - ball.rect.h;
    }
    else
    {
        ball.rect.x += ball.xVel;

        //If the ball hits the player while moving on the X-Axis, make it bounce accordingly
        if(RectsOverlap(ball.rect, player.rect))
        {
            ball.rect.x -= ball.xVel;
            ball.xVel *= -1;
            Mix_PlayChannel(-1, sound.BallBounceSound, 0);
        }
        else  //If not, check if it hit any blocks while moving on the X-Axis
        {
            for(int i = 0; i < BLOCK_COLUMNS*BLOCK_ROWS; i++)
            {
                if(blocks[i].alive && RectsOverlap(ball.rect, blocks[i].rect))
                {
                    ball.rect.x -= ball.xVel;
                    ball.xVel *= -1;
                    blocks[i].alive = false;

                    Mix_PlayChannel(-1, sound.ExplosionSound, 0);
                }
            }
        }

        ball.rect.y += ball.yVel;

        //If the ball hits the player while moving in the Y-Axis, make it bounce accordingly
        if(RectsOverlap(ball.rect, player.rect))
        {
            ball.rect.y -= ball.yVel;
            ball.yVel *= -1;

            //Make the X velocity the distance between the paddle's center and the ball's center devided by 5
            int ballCenterX = ball.rect.x+ball.rect.w/2;
            int paddleCenterX = player.rect.x+player.rect.w/2;

            ball.xVel = (ballCenterX - paddleCenterX)/5;
            Mix_PlayChannel(-1, sound.BallBounceSound, 0);
        }
        else  //If not, check if it hit any blocks while moving on the Y-Axis
        {
            for(int i = 0; i < BLOCK_COLUMNS*BLOCK_ROWS; i++)
            {
                if(blocks[i].alive && RectsOverlap(ball.rect, blocks[i].rect))
                {
                    ball.rect.y -= ball.yVel;
                    ball.yVel *= -1;
                    blocks[i].alive = false;

                    Mix_PlayChannel(-1, sound.ExplosionSound, 0);
                }
            }
        }

        //Make sure the ball doesn't leave the screen and make it
        //bounce randomly
        if(ball.rect.y < GAMEAREA_Y1)
        {
            ball.rect.y = GAMEAREA_Y1;
            ball.yVel *= -1;
            Mix_PlayChannel(-1, sound.BallBounceSound, 0);
        }

        if(ball.rect.x > GAMEAREA_X2 - ball.rect.w)
        {
            ball.rect.x = GAMEAREA_X2 - ball.rect.w;
            ball.xVel *= -1;
            Mix_PlayChannel(-1, sound.BallBounceSound, 0);
        }

        if(ball.rect.x < GAMEAREA_X1)
        {
            ball.rect.x = GAMEAREA_X1;
            ball.xVel *= -1;
            Mix_PlayChannel(-1, sound.BallBounceSound, 0);
        }
        //If the player looses the ball
        if(ball.rect.y > GAMEAREA_Y2)
        {
            ball.isLocked = true;

            //Reposition Ball
            int PaddleCenterX = player.rect.x + player.rect.w/2;
            ball.rect.x = PaddleCenterX - ball.rect.w/2;
            ball.rect.y = player.rect.y - ball.rect.h;

            player.lives--;
            Mix_PlayChannel(-1, sound.BallSpawnSound, 0);
        }
    }
}

void ArkClone::UpdateSplash()
{
    Uint8 *keys = SDL_GetKeyState(NULL);

    if(keys[SDLK_RETURN])
    {
        //This will start a new game
        ResetGame();
        gameState = GS_RUNNING;
    }
}

void ArkClone::DrawSplash()
{
    image.DrawImage(image.SplashImage, image.Backbuffer, 0, 0);
}

void ArkClone::UpdateGame()
{
    UpdatePlayer();
    UpdateBall();

    if(player.lives <= 0)
        gameState = GS_GAMEOVER;

    if(NumBlocksLeft() <= 0)
        ResetGame();
}

void ArkClone::DrawGame()
{
    image.DrawImage(image.BackgroundImage, image.Backbuffer, 0, 0);
    image.DrawImage(image.BallImage, image.Backbuffer, ball.rect.x, ball.rect.y);
    image.DrawImage(image.PlayerPaddleImage, image.Backbuffer, player.rect.x, player.rect.y);
    DrawBlocks();

    char blocksText[64];
    char livesText[64];

    sprintf(blocksText, "Blocks: %d", NumBlocksLeft());
    sprintf(livesText, "Lives: %d", player.lives);
    DrawText(image.Backbuffer, blocksText, 645, 150, GameFont, 255, 255, 255);
    DrawText(image.Backbuffer, livesText, 645, 170, GameFont, 255, 255, 255);
}

void ArkClone::DrawText(SDL_Surface *surface, char *string, int x, int y, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Surface* renderedText = NULL;
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	renderedText = TTF_RenderText_Solid( font, string, color );
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	SDL_BlitSurface( renderedText, NULL, surface, &pos );
	SDL_FreeSurface(renderedText);
}

void ArkClone::DrawGamePaused()
{
    DrawGame();
    image.DrawImage(image.GamepausedImage, image.Backbuffer, 0, 0);
}

void ArkClone::UpdateGameOver()
{
    Uint8 *keys = SDL_GetKeyState(NULL);

    if(keys[SDLK_SPACE])
    {
        gameState = GS_SPLASH;
    }
}

void ArkClone::DrawGameOver()
{
    DrawGame();
    image.DrawImage(image.GameoverImage, image.Backbuffer, 0, 0);
}

void ArkClone::RunGame()
{
    switch(gameState)
    {
    case GS_SPLASH:
        UpdateSplash();
        break;
    case GS_RUNNING:
        UpdateGame();
        break;
    case GS_GAMEOVER:
        UpdateGameOver();
        break;
    default:
        break;
    }
}

void ArkClone::DrawScreen()
{
    switch(gameState)
    {
    case GS_SPLASH:
        DrawSplash();
        break;
    case GS_RUNNING:
        DrawGame();
        break;
    case GS_GAMEOVER:
        DrawGameOver();
        break;
    case GS_PAUSED:
        DrawGamePaused();
        break;
    default:
        break;
    }
}

bool ArkClone::LoadFiles()
{
    GameFont = TTF_OpenFont("graphics/brick.ttf", 20);
    if(GameFont == NULL)
        return false;

    return true;
}

void ArkClone::FreeFiles()
{
    TTF_CloseFont(GameFont);
}

void ArkClone::FreeGame()
{
    Mix_HaltMusic();
    image.FreeFiles();
    sound.FreeFiles();
    FreeFiles();
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}
