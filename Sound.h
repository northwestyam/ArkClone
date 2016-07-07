#ifndef SOUND_H_
#define SOUND_H_

#include "SDL/SDL_mixer.h"

class Sound {
public:
	//Sounds
	Mix_Chunk *BallBounceSound;
	Mix_Chunk *BallSpawnSound;
	Mix_Chunk *ExplosionSound;

	//Music
	Mix_Music *GameMusic;

	bool LoadFiles();
	void FreeFiles();
};

#endif /* SOUND_H_ */
