#pragma once

#include "Shape.h"



// Base class for a Sprite. Can be instantiated on it's own (deafult)
// or subclassed to create a custom sprite.  Includes Shape transformations.
// If the sprite has no image set on it, then a simple triangle will be drawn.
//

class Sprite : public Shape {
public:
	// some functions for highlighting when selected
	//
	void draw();
	

	float age() {
		return (ofGetElapsedTimeMillis() - birthtime);
	}

	void setImage(ofImage img) {
		spriteImage = img;
		//bShowImage = true;
		width = img.getWidth();
		height = img.getHeight();
	}

	
	virtual bool inside(const glm::vec3 p);
	virtual void update() {}

	bool insideTriangle(const glm::vec3 p);
	
	void setSelected(bool state) { bSelected = state; }
	void setHighlight(bool state) { bHighlight = state; }
	bool isSelected() { return bSelected; }
	bool isHighlight() { return bHighlight; }


	bool bHighlight = false;
	bool bSelected = false;
	bool bShowImage = false;

	float rotationSpeed = 0.0;
	float birthtime = 0; // elapsed time in ms
	float lifespan = -1;  //  time in ms
	string name =  "UnammedSprite";
	float width = 40;
	float height = 40;
	
	ofImage spriteImage;




	// physics stuff goes here

	void integrate();

	// physics movement
	glm::vec3 velocity = glm::vec3(0, 0, 0);
	glm::vec3 acceleration = glm::vec3(0, 0, 0);
	glm::vec3 force = glm::vec3(0, 0, 0);

	// physics rotation
	float rotVelocity = 0;
	float rotAcceleration = 0;
	float rotForce = 0;


	float mass = 1.0;
	float damping = .99;


	// get heading vector (towards player)
	glm::vec3 heading = glm::vec3(0, 0, 0);

	void setHeading(float angle) {
		heading = glm::rotate(glm::radians(angle + 180), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1);
	}

};


class Rocket : public Sprite {
public:
	Rocket() {
		scale = glm::vec3(.1, .1, .1);
		//cout << "in Rocket constructor" << endl;
	}
};

class Bullet : public Sprite {
public:
	Bullet() {
		scale = glm::vec3(.1, .1, .1);
		//cout << "in Bullet constructor" << endl;
	}

	void draw() {

		if (bShowImage) {
			ofSetColor(ofColor::white);
			ofPushMatrix();
			ofMultMatrix(getMatrix());

			spriteImage.draw((-spriteImage.getWidth() / 2.0), (-spriteImage.getHeight() / 2.0), 0);

			ofPopMatrix();
		}
		else
		{
			if (bHighlight) ofSetColor(ofColor::white);
			else ofSetColor(ofColor::red);
			ofPushMatrix();
			ofMultMatrix(getMatrix());


			ofDrawTriangle(glm::vec3(0, 0, 0), glm::vec3(100, 150, 0), glm::vec3(-100, 150, 0));
			ofPopMatrix();
		}
	}

	float radius = 5;
};
