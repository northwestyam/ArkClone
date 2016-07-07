#include "Sound.h"

bool Sound::LoadFiles()
{
	//Load sounds
	BallBounceSound = Mix_LoadWAV("audio/ball_bounce.wav");
	BallSpawnSound = Mix_LoadWAV("audio/ball_spawn.wav");
	ExplosionSound = Mix_LoadWAV("audio/explosion.wav");

	//Error check sounds
	if(BallBounceSound == NULL)
		return false;
	if(BallSpawnSound == NULL)
		return false;
	if(ExplosionSound == NULL)
		return false;

	//Load music
	GameMusic = Mix_LoadMUS("audio/song.mp3");

	//Error check music
	if(GameMusic == NULL)
		return false;

	return true;
}

void Sound::FreeFiles()
{
	//Free sounds
	Mix_FreeChunk(BallBounceSound);
	Mix_FreeChunk(BallSpawnSound);
	Mix_FreeChunk(ExplosionSound);
	//Free music
	Mix_FreeMusic(GameMusic);
}
