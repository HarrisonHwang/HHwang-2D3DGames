#include "ofApp.h"
//----------------------------------------------------------------------------------
//
// This example code demonstrates the use of an "Emitter" class to emit Sprites
// and set them in motion. The concept of an "Emitter" is taken from particle
// systems (which we will cover next week).
//
// The Sprite class has also been upgraded to include lifespan, velocity and age
// members.   The emitter can control rate of emission and the current velocity
// of the particles. In this example, there is no acceleration or physics, the
// sprites just move simple frame-based animation.
//
// The code shows a way to attach images to the sprites and optional the
// emitter (which is a point source) can also have an image.  If there are
// no images attached, a placeholder rectangle is drawn.
// Emitters  can be placed anywhere in the window. In this example, you can drag
// it around with the mouse.
//
// OF has an add-in called ofxGUI which is a very simple UI that is useful for
// creating sliders, buttons and fields. It is not recommended for commercial 
// game development, but it is useful for testing.  The "h" key will hide the GUI
// 
// If you want to run this example, you need to use the ofxGUI add-in in your
// setup.
//
//
//  Kevin M. Smith - CS 134 SJSU




//  Add a Sprite to the Sprite System
//
void SpriteList::add(Sprite s) {
	sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteList::remove(int i) {
	sprites.erase(sprites.begin() + i);
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteList::update() {

	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	
	
	
	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}
	

}

//  Render all the sprites
//
void SpriteList::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}


Emitter::Emitter() {
	sys = new SpriteList();
	init();
}


void Emitter::init() {
	//lifespan = 3000;    // default milliseconds
	started = false;

	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = ofVec3f(100, 100, 0);
	drawable = false;
	width = 50;
	height = 50;
	angle = 0;
}



//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
	if (drawable) {

		if (haveImage) {
			image.draw(-image.getWidth() / 2.0 + pos.x, -image.getHeight() / 2.0 + pos.y);
		}
		else {
			ofSetColor(0, 0, 200);

			ofDrawRectangle(-width / 2 + pos.x, -height / 2 + pos.y, width, height);

		}
	}

	// draw sprite system
	//
	sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {
	if (!started) return;

	float time = ofGetElapsedTimeMillis();

	if ((time - lastSpawned) > (1000.0 / rate)) {

		// call virtual to spawn a new sprite
		//
		for (int i = 0; i < nAgents; i++) {
			spawnSprite();
		}
		lastSpawned = time;
		
	}

	// update sprite list
	//
	if (sys->sprites.size() == 0) return;
	vector<Sprite>::iterator s = sys->sprites.begin();
	vector<Sprite>::iterator tmp;
	
	
	
	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sys->sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sys->sprites.erase(s);
			s = tmp;
		}
		else s++;
	}
	

	// move each sprite
	for (int i = 0; i < sys->sprites.size(); i++) {
		moveSprite(&sys->sprites[i]);
	}
}


// virtual function to move sprite (can be overloaded)
// integrate sprite physics here
void Emitter::moveSprite(Sprite *sprite) {
	Target target;

	// same method as player movement
	// o + t*d

	ofVec2f ref(1, 0);
	float anglelive = ref.angle({ (targetpos.x - sprite->pos.x), (targetpos.y - sprite->pos.y) });

	/*
	glm::vec3 dir = glm::normalize(glm::vec3(glm::sin(glm::radians(anglelive+90)), -glm::cos(glm::radians(anglelive+90)), 0));

	sprite->pos += sspeed * dir;
	*/
	

	// for toggling sprite image
	if (bShowImage) {
		sprite->bShowImage = true;
	}
	else {
		sprite->bShowImage = false;
	}

	// this makes it so that already fired projectiles will correct its angle too
	// old non-physics based rotation
	sprite->rot = anglelive+90;

	/*
	// physics based chasing using attraction force from
	// ..\examples\math\particleExample
	//

	//glm::vec3 attractPoint(targetpos.x, targetpos.y, 0);
	sprite->force = targetpos - sprite->pos; 
	sprite->force = glm::normalize(sprite->force);

	//sprite->acceleration = (1 / sprite->mass) * sprite->force;
	//sprite->integrate();

	
	
	float drag = 0.95;
	sprite->velocity *= drag;
	sprite->velocity += sprite->force * .4;


	sprite->pos += sprite->velocity;	
	*/

	sprite->setHeading(anglelive-90);
	
	sprite->force = sprite->force + glm::vec3(sprite->heading.x * sspeed, sprite->heading.y * sspeed, 0);
	//sprite->rotForce = sprite->rotForce + 

	sprite->acceleration = (1 / sprite->mass) * sprite->force;
	sprite->integrate();



	// zero out force
	sprite->force = glm::vec3(0, 0, 0);



	//sspeed for sprite speed in place of thrust
	//sprite->force = sprite->force + glm::vec3(sprite->heading.x * sspeed, )


	
}

// Base Emitter spawnSprite that came with base code
// actual used spawnsprite method in ofApp.h under rocket RocketEmitter
// virtual function to spawn sprite (can be overloaded)
void Emitter::spawnSprite() {
	Sprite sprite;
	if (haveChildImage) sprite.setImage(childImage);
	sprite.velocity = velocity;
	sprite.lifespan = lifespan;
	sprite.pos = pos;
	

	sprite.birthtime = ofGetElapsedTimeMillis();
	sprite.rot = angle;

	sys->add(sprite);
}

// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
}


void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(const glm::vec3 v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}

void Emitter::setImage(ofImage img) {
	image = img;
	haveImage = true;
}

void Emitter::setRate(float r) {
	rate = r;
}

void Emitter::setRotation(float a) {
	angle = a;
}

void Emitter::setTargetPos(const glm::vec3 tp) {
	targetpos = tp;

}

void Emitter::toggleImage() {
	bShowImage = !bShowImage;
}

void Emitter::setSpriteSpeed(float s) {
	sspeed = s;
}

void Emitter::setnAgents(int n) {
	nAgents = n;
}




Target::Target() {
	init();
}

void Target::init() {
	

	radius = 25;
	drawable = true;
	playerHaveImage = false;
}

void Target::draw() {
	glm::vec3 localOrigin = glm::vec3(0, 0, 0);
	//glm::vec3 origin = glm::vec3(200, 200, 0);

	if (bShowImage) {
		
		ofSetColor(ofColor::white);

		ofPushMatrix();
		ofMultMatrix(getMatrix());
		playerImage.draw((-playerImage.getWidth() / 2.0), (-playerImage.getHeight() / 2.0), 0);
		//playerImage.draw(pos.x,  pos.y, 0);
		ofPopMatrix();
	}
	else {
		ofFill();
		ofSetCircleResolution(50);
		ofSetColor(ofColor::red);

		
		ofPushMatrix();
		ofMultMatrix(getMatrix());
		
		//ofDrawTriangle(glm::vec3(pos.x, pos.y-20, 0), glm::vec3(pos.x - 30, pos.y + 30, 0), glm::vec3(pos.x + 30, pos.y + 30, 0));
		//ofDrawTriangle(glm::vec3(-30, 30, 0), glm::vec3(30, 30, 0), glm::vec3(0, -30, 0));
		ofDrawTriangle(glm::vec3(-15, 15, 0), glm::vec3(15, 15, 0), glm::vec3(0, -15, 0));


		//ofDrawCircle(pos.x, pos.y, 0, radius);
		ofPopMatrix();
	}
}


void Target::setImage(ofImage img) {
	playerImage = img;
	playerHaveImage = true;
}

void Target::setHeading(glm::vec3 h) {
	heading = h;
}


bool Target::inside(const glm::vec3 p){
	// if there is no sprite image attached, then just use triangle case.
	//
	if (!bShowImage) {
		return insidePlayerTriangle(p);
	}

	// if sprite image attached, then first check if point is inside bounds of image
	// in object space.  If point is inside bounds, then make sure the point is in
	// opaque part of image.
	//
	

	glm::vec3 s = glm::inverse(getMatrix()) * glm::vec4(p, 1);
	int w = playerImage.getWidth();
	int h = playerImage.getHeight();
	if (s.x > -w / 2 && s.x < w / 2 && s.y > -h / 2 && s.y < h / 2) {
		int x = s.x + w / 2;
		int y = s.y + h / 2;
		ofColor color = playerImage.getColor(x, y);
		return (color.a != 0);   // check if color is opaque (not the transparent background)
	}
	else return false;

}

bool Target::insidePlayerTriangle(const glm::vec3 p) {

	/*
	// transform the screen space point p to the triangle's local 
	// oordinate system  (object space);  this will take into account any
	// rotation, translation or scale on the object.
	//
	glm::vec4 p2 = glm::inverse(getMatrix()) * glm::vec4(p, 1);

	glm::vec3 v1 = glm::normalize(verts[0] - p2);
	glm::vec3 v2 = glm::normalize(verts[1] - p2);
	glm::vec3 v3 = glm::normalize(verts[2] - p2);
	float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
	float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
	float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
	if (a1 < 0 && a2 < 0 && a3 < 0) return true;
	else return false;
	*/
	
	
	// for the circle
	glm::vec3 localOrigin = glm::vec3(0, 0, 0);
	glm::vec3 p2 = glm::inverse(getMatrix()) * glm::vec4(p, 1);
	//return (glm::length(p - pos) < radius);
	return (glm::distance(p2, localOrigin) < radius-10);
}


void Target::toggleImage() {
	bShowImage = !bShowImage;
}

