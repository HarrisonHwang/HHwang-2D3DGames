#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);

	// create an image for sprites being spawned by emitter
	//
	if (defaultImage.load("images/asteroid.png")) {
		imageLoaded = true;
	}
	else {
		cout << "Can't open image file" << endl;
		ofExit();
	}
	// create an image for player
	if (playerImage.load("images/ship.png")) {
		playerImageLoaded = true;
	}
	else {
		cout << "No player image found" << endl;
		ofExit();
	}

	// create background image
	if (backgroundImage.load("images/background.png")) {
		backgroundImageLoaded = true;
	}
	else {
		cout << "No background image found" << endl;
		ofExit();
	}

	ofSetBackgroundColor(ofColor::black);


	// slider initialize variables
	float scale_var = .1;



	gui.setup();
	gui.add(level1.setup("Level1", true));
	gui.add(level2.setup("Level2", false)); 
	gui.add(level3.setup("Level3", false));
	gui.add(rate.setup("rate", 1, .5, 5));
	gui.add(life.setup("life", 1000, 1000, 1000));
	gui.add(energy.setup("energy", 20, 1, 20));
	gui.add(pspeed.setup("player speed", 8, 1, 20));
	gui.add(sspeed.setup("sprite speed", 500, 1, 1000));
	gui.add(pscale.setup("player scale", 1, 0.1, 5));
	gui.add(nAgents.setup("projectiles per spawn", 1, 1, 10));
	gui.add(scale.setup("sprite scale", .3, .05, .3));

	gui.add(bulletsize.setup("bullet size", 10, 5, 20));
	gui.add(bulletspeed.setup("bullet speed", 1500, 500, 2000));
	gui.add(bulletrate.setup("bullet rate", 10, 1, 10));


	// setup sounds
	engineSound.load("sounds/thrusters-loop.wav");
	engineSound.setLoop(true);

	explosionSound.load("sounds/explosion.wav");

	impactSound.load("sounds/impact.wav");

	


	// load font for text display
	verdana30.load("verdana.ttf", 30);
	verdana10.load("verdana.ttf", 10);
	
	typeStr = "ENERGY: " + to_string(energy) + "/" + to_string(energy);
	
	//emitter = new PinwheelEmitter();
	//emitter = new Emitter();
	emitter = new RocketEmitter();


	emitter->pos = glm::vec3(500, 500, 0);
	emitter->drawable = false; 
	emitter->setChildImage(defaultImage);


	// target/player
	target = new Target();

	target->pos = glm::vec3(200, 300, 0);
	target->drawable = true;

	target->setImage(playerImage);


	gun = new ParticleEmitter();
	gun->pos = target->pos;
	gun->setEmitterType(DirectionalEmitter);
	//gun->drawable = false;

	//explosion = new ExplosionEmitter();
	explosion = new ParticleEmitter();
	explosion->pos = target->pos;

	turbForce = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));
	radialForce = new ImpulseRadialForce(1000.0);

	//explosion->sys->addForce(turbForce);
	explosion->sys->addForce(radialForce);
	
	explosion->setVelocity(ofVec3f(0, 0, 0));
	explosion->setOneShot(true);
	explosion->setEmitterType(RadialEmitter);
	explosion->setGroupSize(10);


	bHide = false;

}

//--------------------------------------------------------------
void ofApp::update() {
	
	// level presets
	if (level1) {
		levelSelect = 1;

		gui.setup();
		gui.add(level1.setup("Level1", true));
		gui.add(level2.setup("Level2", false));
		gui.add(level3.setup("Level3", false));
		gui.add(rate.setup("rate", 1, .5, 5));
		gui.add(life.setup("life", 1000, 1000, 1000));
		gui.add(energy.setup("energy", 20, 1, 20));
		gui.add(pspeed.setup("player speed", 8, 1, 20));
		gui.add(sspeed.setup("sprite speed", 500, 1, 1000));
		gui.add(pscale.setup("player scale", 1, 0.1, 5));
		gui.add(nAgents.setup("projectiles per spawn", 1, 1, 10));
		gui.add(scale.setup("sprite scale", .3, .05, .3));

		gui.add(bulletsize.setup("bullet size", 5, 5, 20));
		gui.add(bulletspeed.setup("bullet speed", 1500, 500, 2000));
		gui.add(bulletrate.setup("bullet rate", 10, 1, 10));

		nEnergy = energy;
	}

	if (level2) {
		levelSelect = 2;

		gui.setup();
		gui.add(level1.setup("Level1", false));
		gui.add(level2.setup("Level2", true));
		gui.add(level3.setup("Level3", false));
		gui.add(rate.setup("rate", 2, .5, 5));
		gui.add(life.setup("life", 1000, 1000, 1000));
		gui.add(energy.setup("energy", 10, 1, 20));
		gui.add(pspeed.setup("player speed", 4, 1, 20));
		gui.add(sspeed.setup("sprite speed", 700, 1, 1000));
		gui.add(pscale.setup("player scale", 1, 0.1, 5));
		gui.add(nAgents.setup("projectiles per spawn", 1, 1, 10));
		gui.add(scale.setup("sprite scale", .2, .05, .3));

		gui.add(bulletsize.setup("bullet size", 5, 5, 20));
		gui.add(bulletspeed.setup("bullet speed", 500, 500, 2000));
		gui.add(bulletrate.setup("bullet rate", 5, 1, 10));

		nEnergy = energy;
	}

	if (level3) {
		levelSelect = 3;

		gui.setup();
		gui.add(level1.setup("Level1", false));
		gui.add(level2.setup("Level2", false));
		gui.add(level3.setup("Level3", true));
		gui.add(rate.setup("rate", 2, .5, 5));
		gui.add(life.setup("life", 1000, 1000, 1000));
		gui.add(energy.setup("energy", 5, 1, 20));
		gui.add(pspeed.setup("player speed", 6, 1, 20));
		gui.add(sspeed.setup("sprite speed", 400, 1, 1000));
		gui.add(pscale.setup("player scale", 1, 0.1, 5));
		gui.add(nAgents.setup("projectiles per spawn", 1, 1, 10));
		gui.add(scale.setup("sprite scale", .15, .05, .3));

		gui.add(bulletsize.setup("bullet size", 5, 5, 20));
		gui.add(bulletspeed.setup("bullet speed", 500, 500, 2000));
		gui.add(bulletrate.setup("bullet rate", 2, 1, 10));

		nEnergy = energy;

	}


	
	if (startToggle) {


		emitter->setRate(rate);
		emitter->setLifespan(life * 1000);    // convert to milliseconds 
		emitter->setSpriteSpeed(float(sspeed));
		emitter->setnAgents(nAgents);
		
		gun->setVelocity(ofVec3f(bulletspeed * target->heading.x, bulletspeed * target->heading.y, 0));
		gun->setParticleRadius(float(bulletsize));
		gun->setRate(bulletrate);
		gun->setLifespan(2);    // convert to milliseconds 
		//gun->setVelocity(float(sspeed));
		//gun->setTargetHeading(target->rot);

		explosion->setLifespan(.5);
		explosion->setRate(1);
		explosion->setParticleRadius(5);




		emitter->update();
		//target->update();
		gun->update();
		gun->pos = target->pos;
		//gun->heading = target->heading;
		//gun->setTarget
		
		explosion->update();


		// setting player heading, speed, scale
		// formula o + t*d
		glm::vec3 dir = glm::normalize(glm::vec3(glm::sin(glm::radians(target->rot)), -glm::cos(glm::radians(target->rot)), 0));
		
		//checking for movement out of bounds
		glm::vec3 newpos = target->pos + float(pspeed) * dir;


		// control player movement
		// within screen boudaries
		if (foward) {
			if (newpos.y > 0 && newpos.x > 0 && newpos.y < ofGetHeight() && newpos.x < ofGetWidth()) {
				target->pos += float(pspeed) * dir;
			}
			else {
				// hitting upper limit
				if (newpos.y < 0) {
					target->pos = glm::vec3(target->pos.x, 1, 0);
				}
				// hitting leftmost limit
				if (newpos.x < 0) {
					target->pos = glm::vec3(1, target->pos.y, 0);
				}
				// hitting lower limit
				if (newpos.y > ofGetHeight()) {
					target->pos = glm::vec3(target->pos.x, ofGetHeight() - 1, 0);
				}
				// hitting rightmost limit
				if (newpos.x > ofGetWidth()) {
					target->pos = glm::vec3(ofGetWidth() - 1, target->pos.y, 0);
				}
			}
		}
		
		if (backward) {
			if (newpos.y > 0 && newpos.x > 0 && newpos.y < ofGetHeight() && newpos.x < ofGetWidth()) {
				target->pos -= float(pspeed) * dir;
			}
			else {
				// hitting upper limit
				if (newpos.y < 0) {
					target->pos = glm::vec3(target->pos.x, 1, 0);
				}
				// hitting leftmost limit
				if (newpos.x < 0) {
					target->pos = glm::vec3(1, target->pos.y, 0);
				}
				// hitting lower limit
				if (newpos.y > ofGetHeight()) {
					target->pos = glm::vec3(target->pos.x, ofGetHeight() - 1, 0);
				}
				// hitting rightmost limit
				if (newpos.x > ofGetWidth()) {
					target->pos = glm::vec3(ofGetWidth() - 1, target->pos.y, 0);
				}
			}
		}

		if (turnleft) {
			target->rot -= 5;
		}
		
		if (turnright) {
			target->rot += 5;
		}
		
		
		
		// for scaling the player sprite
		target->scale = glm::vec3(float(pscale), float(pscale), float(pscale));

	
		target->setHeading(target->rot);


		for (int i = 0; i < emitter->sys->sprites.size(); i++) {
			// Get values from sliders and update sprites dynamically
			//
			float sc = scale;
			float rs = rotationSpeed;
			emitter->sys->sprites[i].scale = glm::vec3(sc, sc, sc);
			emitter->sys->sprites[i].rotationSpeed = rs;


			
			// check for collision with bullet
			for (int b = 0; b < gun->sys->particles.size(); b++) {
				if (emitter->sys->sprites[i].inside(glm::vec3(gun->sys->particles[b].position))) {
					//cout << "hit" << endl;

					

					explosion = new ParticleEmitter();
					explosion->pos = emitter->sys->sprites[i].pos;
					explosion->sys->addForce(turbForce);
					explosion->sys->addForce(radialForce);

					explosion->setVelocity(ofVec3f(0, 0, 0));
					explosion->setOneShot(true);
					explosion->setEmitterType(RadialEmitter);
					explosion->setGroupSize(10);

					explosion->setLifespan(.5);
					explosion->setRate(1);
					explosion->setParticleRadius(5);

					explosion->update();

					explosion->start();
					explosionSound.play();
					//emitter->sys->remove(i);
					emitter->sys->sprites[i].lifespan = 0;
					if (nEnergy < energy) {
						nEnergy += 1;
					}

				}

			}

			// detect collision between player and agents
			if (target->bShowImage) {
				if (glm::distance(target->pos, emitter->sys->sprites[i].pos) < ((15 * target->scale.x) + (150 * emitter->sys->sprites[i].scale.x))) {
					//cout << "collision" << endl;
					impactSound.play();
					emitter->sys->remove(i);
					nEnergy -= 1;
					// check for game over (energy = 0)
					if (nEnergy == 0) {
						gameover = true;
						startToggle = false;
						emitter->stop();
						gun->stop();
						setup();
					}
				}
			} else {
				if (target->inside(emitter->sys->sprites[i].pos)) {
					//scout << "collision" << endl;
					impactSound.play();
					emitter->sys->remove(i);
					nEnergy -= 1;
					// check for game over (energy = 0)
					if (nEnergy == 0) {
						gameover = true;
						startToggle = false;
						emitter->stop();
						gun->stop();
						setup();
					}
				}
			}

		}


		// get the angle between emitter and target here
		// set reference angle
		ofVec2f ref(1, 0);
		float angle = ref.angle({ (target->pos.x - emitter->pos.x), ((target->pos.y + 25) - emitter->pos.y) });
		emitter->setRotation(angle + 90);

		//set target positon so that projectile will always follow target
		emitter->setTargetPos(target->pos);

		// Energy text display
		typeStr = "ENERGY: " + to_string(nEnergy) + "/" + to_string(energy);

	}
}






//--------------------------------------------------------------
void ofApp::draw(){

	if (!startToggle && !gameover) {
		verdana30.drawString("start with 'space'", 400, 450);
		verdana30.drawString("'h' to toggle gui menu", 400, 550);
		verdana30.drawString("'t' to toggle between sprites and shapes", 400, 600);

		
		switch (levelSelect) {
		case 1:
			verdana30.drawString("Level 1", 400, 300);
			break;
		case 2:
			verdana30.drawString("Level 2", 400, 300);
			break;
		case 3:
			verdana30.drawString("Level 3", 400, 300);
			break;
		default:
			verdana30.drawString("Something Went Wrong", 400, 300);
		}

	}

	if (startToggle && !gameover) {

		//draw background first
		ofSetColor(ofColor::white);
		backgroundImage.draw(0, 0);

		/*
		//test prints to see bounded circle for collisions
		ofDrawCircle(target->pos, 15 * target->scale.x);
		for (int i = 0; i < emitter->sys->sprites.size(); i++) {
			ofDrawCircle(emitter->sys->sprites[i].pos, 150 * emitter->sys->sprites[i].scale.x);
		}
		*/
		
		// draw energy level text
		ofSetColor(225);
		verdana30.drawString(typeStr, 500, 40);


		framerate = to_string(int(ofGetFrameRate()));
		elapsedtime = to_string(ofGetElapsedTimef());
		verdana10.drawString("fps: " + framerate, ofGetWidth() - 300, 30);
		verdana10.drawString("elapsed time: " + elapsedtime + " secs", ofGetWidth() - 300, 50);

		// display level
		switch (levelSelect) {
		case 1:
			verdana30.drawString("Level 1", ofGetWidth()/2 + 50, 40);
			break;
		case 2:
			verdana30.drawString("Level 2", ofGetWidth() / 2 + 50, 40);
			break;
		case 3:
			verdana30.drawString("Level 3", ofGetWidth() / 2 + 50, 40);
			break;
		default:
			verdana30.drawString("Something Went Wrong", 400, 300);
		}

		
		emitter->draw();
		target->draw();
		gun->draw();
		explosion->draw();
		

	}

	if (gameover) {
		//cout << "gameover" << endl;
		verdana30.drawString("GAME OVER", 512, 600);
		verdana30.drawString("Time Survived: " + elapsedtime + " secs", 512, 650);
		verdana30.drawString("press 'r' to play again", 512, 700);
		
	}


	if (!bHide) {
		gui.draw();
	}
}


// for toggling player and projectile sprite images
void ofApp::toggleImage() {
	target->toggleImage();
	emitter->toggleImage();
}


//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//	cout << "mouse( " << x << "," << y << ")" << endl;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

	if (bDragging) {
		glm::vec3 mouseCurrent = glm::vec3(x, y, 0);
		glm::vec3 delta = mouseCurrent - mouseLast;
		
		emitter->pos += delta;
		//cout << "dragging emitter " << target->pos;
		mouseLast = mouseCurrent;
	}

	if (targetDragging) {
		glm::vec3 mouseCurrent = glm::vec3(x, y, 0);
		glm::vec3 delta = mouseCurrent - mouseLast;

		target->pos += delta;
		//cout << "dragging " << target->pos;
		mouseLast = mouseCurrent;

	}

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (emitter->inside(glm::vec3(x, y, 0))) {
		bDragging = true;
		cout << "in emitter";
		mouseLast = glm::vec3(x, y, 0);
	}

	if (target->inside(glm::vec3(x, y, 0))) {
		cout << "in target";
		targetDragging = true;
		mouseLast = glm::vec3(x, y, 0);
	}

	//cout << x << ", " << y << ", " << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bDragging = false;
	targetDragging = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		bHide = !bHide;
		break;
	case 't':
		//for toggle
		toggleImage();
		break;
	case 'w':
		//target->rot = 0;
		foward = true;
		engineSound.play();

		//cout << "target pos: " << target->pos.x << ", " << target->pos.y << endl;
		break;
	case 'a':
		//target->rot = 270;
		//target->rot -= 15;

		turnleft = true;
		//cout << "target pos: " << target->pos.x << ", " << target->pos.y << endl;
		break;
	case 's':
		
		/*
		// keeping this around cuz it somehow has wrap around behavior
		// dist var has been deleted so just replace with number
		if (target->pos.y > 0 && target->pos.y < ofGetHeight()) {
			if ((target->pos.y + dist) >= ofGetHeight()) {
				target->pos.y += (dist - (target->pos.y + dist)) + 1;
			}
			else {
				target->pos.y += dist;
			}

		}
		*/

		//target->rot = 180;
		backward = true;
		engineSound.play();
		
		//cout << "target pos: " << target->pos.x << ", " << target->pos.y << endl;
		break;
	case 'd':
		//target->rot = 90;
		//target->rot += 15;

		turnright = true;

		
		//cout << "target pos: " << target->pos.x << ", " << target->pos.y << endl;
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case 'r':
		gameover = false;
		break;
	case ' ':
		startToggle = true;
		ofResetElapsedTimeCounter();
		nEnergy = energy;
		emitter->start();
		gun->start();
		break;
	case 'g':
		explosion->sys->reset();
		explosion->start();
		break;
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
	case OF_KEY_LEFT:
	case OF_KEY_RIGHT:
	case OF_KEY_UP:
	case OF_KEY_DOWN:
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	case 'w':
		foward = false;
		engineSound.stop();
		break;
	case 'a':
		turnleft = false;

		break;
	case 's':
		backward = false;
		engineSound.stop();
		break;
	case 'd':
		turnright = false;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

