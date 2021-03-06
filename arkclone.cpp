#include "arkclone.h"
#include <SDL/SDL.h>
#include "graphics.h"
#include "paddle.h"
#include "input.h"

namespace {
	const int kFPS = 60;
}

Arkclone::Arkclone() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_ShowCursor(SDL_DISABLE);
	eventLoop();
}

Arkclone::~Arkclone() {
	SDL_Quit();
}

void Arkclone::eventLoop() {
	Graphics graphics;
	Input input;

	SDL_Event event;
	paddle_.reset(new Paddle("graphics/player.bmp",0,0,102,22));


	bool running = true;
	int last_update_time = SDL_GetTicks();
	while (running) {
		const int start_time = SDL_GetTicks();
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					input.keyDownEvent(event);
					break;
				case SDL_KEYUP:
					input.keyUpEvent(event);
					break;
				default:
					break;
			}
		}
		if (input.wasKeyPressed(SDLK_ESCAPE)) {
			running = false;
		}
		if (input.isKeyHeld(SDLK_LEFT) && input.isKeyHeld(SDLK_RIGHT)) {
			paddle_->stopMoving();
		} else if (input.isKeyHeld(SDLK_LEFT)) {
			paddle_->startMovingLeft();
		} else if (input.isKeyHeld(SDLK_RIGHT)) {
			paddle_->startMovingRight();
		} else {
			paddle_->stopMoving();
		}

		const int current_time = SDL_GetTicks();
		update(current_time - last_update_time);
		last_update_time = current_time;

		draw(graphics);
		const int elapsed_time = SDL_GetTicks() - start_time;
		SDL_Delay(1000/kFPS - elapsed_time);

	}

}

void Arkclone::update(int elapsed_time_ms) {
	paddle_->update(elapsed_time_ms);
}

void Arkclone::draw(Graphics& graphics) {
	graphics.clear();
	paddle_->draw(graphics,0,0);
	graphics.flip();
}
