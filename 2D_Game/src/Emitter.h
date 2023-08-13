#pragma once

#include "ofMain.h"
#include "Shape.h"
#include "Sprite.h"

//
//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteList {
public:
	void add(Sprite);
	void remove(int);
	void update();
	void draw();
	vector<Sprite> sprites;

	bool bShowImage = false;
};




//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter : public Shape {
public:
	Emitter();
	void init();
	void draw();
	void start();
	void stop();
	void setLifespan(float);
	void setVelocity(const glm::vec3 v);
	void setChildImage(ofImage);
	void setImage(ofImage);
	void setRate(float);
	void update();

	void setRotation(float);
	void setTargetPos(const glm::vec3 tp);
	void setSpriteSpeed(float s);
	void setnAgents(int n);
	

	// virtuals - can overloaded
	virtual void moveSprite(Sprite *);
	virtual void spawnSprite();
	virtual bool inside(glm::vec3 p) {
		glm::vec3 s = glm::inverse(getMatrix()) * glm::vec4(p, 1);
		return (s.x > -width / 2 && s.x < width / 2 && s.y > -height / 2 && s.y < height / 2);
	}

	SpriteList *sys;
	float rate;
	glm::vec3 velocity;
	float lifespan = 1000;
	bool started;
	float lastSpawned;
	ofImage childImage;
	ofImage image;
	bool drawable;
	bool haveChildImage;
	bool haveImage;
	float width, height;
	

	float angle;
	glm::vec3 targetpos;

	bool bShowImage = false;
	void toggleImage();

	float sspeed;
	int nAgents = 1;
};

class Target : public Shape {
public:
	Target();
	void init();


	// class methods
	void draw();
	void setImage(ofImage);

	bool inside(const glm::vec3 p);

	bool insidePlayerTriangle(const glm::vec3 p);
	void toggleImage();

	void setHeading(glm::vec3 h);




	glm::vec3 heading;

	void setHeading(float angle) {
		heading = glm::rotate(glm::radians(angle), glm::vec3(0, 0, 1))* glm::vec4(0, -1, 0, 1);
	}

	glm::vec3 velocity;
	bool haveImage;
	bool drawable;
	float radius;
	ofImage playerImage;
	bool playerHaveImage;

	bool bShowImage = false;

	bool started;
};

//  "Custom" Rocket Emitter for sprites
//   Emits Rocket sprites which will have our own custom behavior
//       will point towards target
//
class RocketEmitter : public Emitter {
public:

	//  Overload of moveSprite so that our sprite will have it's own
	//  animated behavior.
	//
	void moveSprite(Sprite* sprite) {
		//sprite->pos += sprite->velocity / ofGetFrameRate();
		Emitter::moveSprite(sprite);


	}

	//  Oveload of "spawnsprite" so that the emitter will create our own
	//  custom object instead of the default one.
	//
	void spawnSprite() {

		//	Emitter::spawnSprite();

		Rocket rocket;
		if (haveChildImage) rocket.setImage(childImage);
		//rocket.velocity = velocity;
		rocket.lifespan = lifespan;
		//rocket.pos = pos;

		int rangex = rand() % ofGetWidth();
		int rangey = rand() % ofGetHeight();

		rocket.pos = glm::vec3(rangex, rangey, 0);



		rocket.birthtime = ofGetElapsedTimeMillis();
		//rocket.rot = angle;

		int randangle = rand() % 360;
		rocket.rot = randangle;


		sys->add(rocket);
	}

	
};


