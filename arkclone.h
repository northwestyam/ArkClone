#ifndef ARKCLONE_H_
#define ARKCLONE_H_

#include <boost/scoped_ptr.hpp>

class Graphics;
class Paddle;

class Arkclone {
public:
	Arkclone();
	~Arkclone();

private:
	void eventLoop();
	void update(int elapsed_time_ms);
	void draw(Graphics& graphics);

	boost::scoped_ptr<Paddle> paddle_;
};

#endif /* ARKCLONE_H_ */
