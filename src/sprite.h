#ifndef SPRITE_H_
#define SPRITE_H_

#include <string>
#include <SDL/SDL.h>

class Graphics;

class Sprite {
public:
	Sprite(const std::string& file_path,
			int source_x, int source_y,
			int width, int height);
	// since spite class is now inheritied, the destructor has to be virtual
	virtual ~Sprite();

	// we want to update and draw sprites, and not care whether it needs to be updated
	virtual void update(int /*elapsed_time*/) {}
	void draw(Graphics& graphics, int x, int y);

protected:
	// We'll be modifying this constantly to move sprites.
	// But it should only be modified by related classes (e.g. Sprite)
	SDL_Rect source_rect_;


private:
	SDL_Surface* sprite_sheet;
};

#endif /* SPRITE_H_ */
