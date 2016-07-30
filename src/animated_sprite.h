/*
	Animated_sprite class
	We want to see our sprite's animated. To do this, we need...
		- a way to update before being drawn
		- all Sprite class functionality
		- Keep track of # of frames, current frame, time per frame,
		 	and time since the last frame-change
	Our images are...
		- laid out left to right
		- frames are spaced by tile_size
*/
#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_

#include "sprite.h"

// inherits from sprite
class AnimatedSprite : public Sprite
{
public:
	// Constructor must take all of sprite's parameters
	AnimatedSprite(	const std::string& file_path,
					int source_x, int source_y,
					int width, int height,
					int fps, int num_frames);
	void update(int elapsed_time_ms);

// how many frames do i want to run in 1 second
// or how many millisenconds do I want this to run?
private:
	const int frame_time_;
	const int num_frames_;
	int current_frame_;
	int elapsed_time_;		// since last frame change
};

#endif /* ANIMATEDSPRITE_H_ */
