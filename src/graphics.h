#ifndef GRAPHICS_H_
#define GRAPHICS_H_

struct SDL_Surface;
struct SDL_Rect;

class Graphics {
public:
	Graphics();
	virtual ~Graphics();

	void blitSurface(	SDL_Surface* source,
						SDL_Rect* source_rectangle,
						SDL_Rect* destination_rectangle);
	void clear();
	void flip();

private:
	SDL_Surface* screen_;
};

#endif /* GRAPHICS_H_ */
