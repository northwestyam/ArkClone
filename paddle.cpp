#include "paddle.h"
#include "graphics.h"
#include <cmath>

namespace {
	const float kSlowdownFactor = 0.8f;
	const float kWalkingAcceleration = 0.0012f;
	const float kMaxSpeedX = 0.325f;
}

Paddle::Paddle(
		const std::string& file_path,
		int source_x, int source_y,
		int width, int height)
{
	sprite_sheet = SDL_LoadBMP(file_path.c_str());
	source_rect_.x = source_x;
	source_rect_.y = source_y;
	source_rect_.w = width;
	source_rect_.h = height;
	velocity_x_ = 0.0f;
	acceleration_x_ = 0.0f;
	x_ = 0;
	y_ = 0;
}

Paddle::~Paddle() {
	SDL_FreeSurface(sprite_sheet);
}

void Paddle::draw(Graphics& graphics, int x, int y) {
	SDL_Rect destination_rectangle;
	destination_rectangle.x = x;
	destination_rectangle.y = y;
	graphics.blitSurface(sprite_sheet, &source_rect_, &destination_rectangle);
}

void Paddle::update(int elapsed_time_ms) {
	x_ += round(velocity_x_ * elapsed_time_ms);
	velocity_x_ += acceleration_x_ * elapsed_time_ms;
	if (acceleration_x_ < 0.0f) {
		velocity_x_ = std::max(velocity_x_, -kMaxSpeedX);
	} else if (acceleration_x_ > 0.0f) {
		velocity_x_ = std::min(velocity_x_, kMaxSpeedX);
	} else {
		velocity_x_ *= kSlowdownFactor;
	}
}

void Paddle::startMovingLeft() {
	acceleration_x_ = -kWalkingAcceleration;
}

void Paddle::startMovingRight() {
	acceleration_x_ = kWalkingAcceleration;
}

void Paddle::stopMoving() {
	acceleration_x_ = 0.0f;
}
