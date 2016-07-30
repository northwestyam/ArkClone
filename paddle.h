#ifndef PADDLE_H_
#define PADDLE_H_

#include <string>
#include <SDL/SDL.h>
#include <boost/scoped_ptr.hpp>

class Graphics;

class Paddle {
public:
	Paddle(
			const std::string& file_path,
			int source_x, int source_y,
			int width, int height);
	virtual ~Paddle();
	void update(int elapsed_time_ms);
	void draw(Graphics& graphics, int x, int y);
	void startMovingLeft();
	void startMovingRight();
	void stopMoving();

protected:
	SDL_Rect source_rect_;

private:
	SDL_Surface* sprite_sheet;
	int x_, y_;
	float velocity_x_;
	float acceleration_x_;


};

#endif /* PADDLE_H_ */
