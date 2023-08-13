
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
// 
//
//  Student Name:   < Your Name goes Here >
//  Date: <date of last version>


#include "ofApp.h"
#include "Util.h"



//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;
	bLanderHeading = true;
	
	// bool track lander or not
	bTracking = false;

	// bool for altitude display
	bAltitude = false;

	
	//Harrison start
	//background
	if (backgroundImage.load("images/background.png")) {
		bBackgroundImageLoaded = true;
	}
	else {
		cout << "No background image found" << endl;
		cout << "must in in bin/data/images" << endl;
		ofExit();
	}

	
	// Setup 3 - Light System
	// 
	keyLight.setup();
	keyLight.enable();
	keyLight.setAreaLight(1, 1);
	keyLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	keyLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	keyLight.setSpecularColor(ofFloatColor(1, 1, 1));

	keyLight.rotate(45, ofVec3f(0, 1, 0));
	keyLight.rotate(-45, ofVec3f(1, 0, 0));
	keyLight.setPosition(5, 5, 5);


	fillLight.setup();
	fillLight.enable();
	fillLight.setSpotlight();
	fillLight.setScale(.05);
	fillLight.setSpotlightCutOff(15);
	fillLight.setAttenuation(2, .001, .001);
	fillLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	fillLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	fillLight.setSpecularColor(ofFloatColor(1, 1, 1));
	fillLight.rotate(-10, ofVec3f(1, 0, 0));
	fillLight.rotate(-45, ofVec3f(0, 1, 0));
	fillLight.setPosition(-5, 5, 5);

	rimLight.setup();
	rimLight.enable();
	rimLight.setSpotlight();
	rimLight.setScale(.05);
	rimLight.setSpotlightCutOff(30);
	rimLight.setAttenuation(.2, .001, .001);
	rimLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	rimLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	rimLight.setSpecularColor(ofFloatColor(1, 1, 1));
	rimLight.rotate(180, ofVec3f(0, 1, 0));
	rimLight.setPosition(0, 5, -7);

//	ofSetWindowShape(1024, 768);
	//cam.setPosition(10, 50, 0);
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	//cam.setPosition(0, 10, 0);
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();
	
	firstPerson.setPosition(0, 0, 0);
	firstPerson.setNearClip(.1);
	firstPerson.setFov(65.5);

	theCam = &cam;



	// set up landing pad for lighting
	pad.loadModel("geo/pad.obj");
	pad.setScale(.01, .01, .01);
	pad.setPosition(0, 0.2, 5);


	// set up all the checkpoint rings
	for (int i = 0; i < numRings; i++) {
		ofxAssimpModelLoader ring;
		ring.loadModel("geo/ring.obj");

		//xyz -->  front/back   up/down   left/right
		ring.setPosition(ofRandom(-150, 150), ofRandom(0, 100), ofRandom(-150, 150));
		//ring.setPosition(3, 3, 3);
		//cout << ring.getMeshCount() << endl;

		ring.setScale(.03, .03, .03);
		cout << ring.getScale() << endl;
		ringList.push_back(ring);
	}
	

	// setup rudimentary lighting 
	//
	initLightingAndMaterials();

	//mars.loadModel("geo/mars-low-5x-v2.obj");
	//mars.loadModel("geo/moon-houdini.obj");
	//mars.loadModel("geo/terrain3.obj");
	mars.loadModel("geo/lowresterrain.obj");
	//mars.setScale(10, 10, 10);
	mars.setScaleNormalization(false);


	// setup sounds
	engineSound.load("sounds/thrusters-loop.wav");
	engineSound.setLoop(true);

	explosionSound.load("sounds/explosion.wav");

	ringSound.load("sounds/ringSound.wav");

	// create sliders for testing
	//
	gui.setup();
	gui.add(numLevels.setup("Number of Octree Levels", 1, 1, 8));
	gui.add(thrust.setup("Thrust", 15, 10, 20));
	gui.add(timeToggle.setup("toggle time info", false));
	bHide = false;

	//  Create Octree for testing.
	//
	float t1 = ofGetElapsedTimeMillis();
	octree.create(mars.getMesh(0), 20);
	float t2 = ofGetElapsedTimeMillis();
	cout << "Time to Create Octree: " << t2 - t1 << " millisec" << endl;
	
	cout << "Number of Verts: " << mars.getMesh(0).getNumVertices() << endl;

	testBox = Box(Vector3(3, 3, 0), Vector3(5, 5, 2));


	// load textures
	//
	if (!ofLoadImage(particleTex, "images/dot.png")) {
		cout << "Particle Texture File: images/dot.png not found" << endl;
		ofExit();
	}

	/*
	// load the shader
	//
#ifdef TARGET_OPENGLES
	shader.load("shaders_gles/shader");
#else
	shader.load("shaders/shader");
#endif
*/

	//Harrison end





	//Patricia start
	turbForce = new TurbulenceForce(ofVec3f(0, -10, 0), ofVec3f(0, -10, 0));
	gravityForce = new GravityForce(ofVec3f(0, 10, 0));
	radialForce = new ImpulseRadialForce(1);

	
	emitter.sys->addForce(turbForce);
	emitter.sys->addForce(gravityForce);
	emitter.sys->addForce(radialForce);

	
	emitter.setVelocity(ofVec3f(0, -20, 0));
	emitter.setOneShot(true);
	emitter.setEmitterType(RadialEmitter);
	emitter.setGroupSize(5);
	emitter.setRandomLife(true);
	emitter.setLifespanRange(ofVec2f(1, 1));


	explosionEmitter.setVelocity(ofVec3f(0, -20, 0));
	explosionEmitter.setOneShot(true);
	explosionEmitter.setEmitterType(SphereEmitter);
	explosionEmitter.setGroupSize(100);
	explosionEmitter.setRandomLife(true);
	explosionEmitter.setLifespanRange(ofVec2f(1, 1));

	//Patricia end


	/*
	
	
	//open file in trunc mode (overwrite) to log ray intersect times
	timeFile.open(ofToDataPath("times.txt"), ofFile::WriteOnly, false);
	if (!timeFile) {
		cout << "File not created" << endl;
	}
	else {
		cout << "File created" << endl;
	}
	*/

}

// load vertex buffer in preparation for rendering
//
void ofApp::loadVbo() {
	if (emitter.sys->particles.size() < 1) return;

	vector<ofVec3f> sizes;
	vector<ofVec3f> points;
	for (int i = 0; i < emitter.sys->particles.size(); i++) {
		points.push_back(emitter.sys->particles[i].position);
		sizes.push_back(ofVec3f(5));
	}
	// upload the data to the vbo
	//
	int total = (int)points.size();
	vbo.clear();
	vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
}
 
//--------------------------------------------------------------
// incrementally update scene (animation)
//
// Harrison and Patricia
void ofApp::update() {
	if (timeToggle) {
		bTimingInfo = true;
	}
	else {
		bTimingInfo = false;
	}

	//cout << cam.getPosition() << endl;

	if (bLanderLoaded && bHasControl) {

		if (bStart && (bFoward || bBackward || bLeft || bRight || bUp || bDown || bTurnLeft || bTurnRight)) {
			

			//120 for 2 mins
			//cout << float(120 - (ofGetElapsedTimef() - starttime)) << endl;
			// timeleft = float(120 - (ofGetElapsedTimef() - starttime));
			battery = battery - (ofGetElapsedTimef() - starttime) / 100;
			if (battery <= 0) {
				cout << "out of battery" << endl;
				bOutOfTime = true;
				bStart = false;
			}
		}
		else if (!bThrust)
			starttime = ofGetElapsedTimef();


		// don't forget to update emitter
		//
		
		emitter.update();
		explosionEmitter.update();


		//set heading
		setHeading(lander.getModelMatrix());
		//cout << landerHeading << endl;
		//setHeading(lander.getRotationAngle(0));

		
		
		//swap between camera positions
		if (bFirstPerson) {
			firstPerson.setPosition(lander.getPosition());
			firstPerson.lookAt(lander.getPosition() + landerHeading);
		}

		//third person
		if (bThirdPerson) {
			glm::vec3 backview;

			ofPushMatrix();
			ofMultMatrix(lander.getModelMatrix());
			backview = lander.getPosition() - landerHeading * 20 + glm::vec3(0, 5, 0);
			thirdPerson.setPosition(backview);
			thirdPerson.lookAt(lander.getPosition());
			ofPopMatrix();
		}

		// cam tracking or not
		if (bTracking) {
			tracking.lookAt(lander.getPosition());
		}

		// for altitude display
		if (bAltitude) {
			ofVec3f p;
			raySelectWithOctreeForAltitude(p);
		}

		


		//handle thrust sound
		//if (bThrust) {
		if(bFoward || bBackward || bLeft || bRight || bUp || bDown || bTurnLeft || bTurnRight) {
			if (!engineSound.isPlaying())
				engineSound.play();
			emitter.start();
		}
		else engineSound.stop();

		//collision detection
		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		//Octree::drawBox(bounds);

		glm::vec3 avgNormal = glm::vec3(0, 0, 0);
		colNodeList.clear();
		colBoxList.clear();
		//octree.intersect(bounds, octree.root, colBoxList);
		octree.intersect(bounds, octree.root, colBoxList, colNodeList);
		//cout << colBoxList.size() << endl;

		glm::vec3 bounceForce = glm::vec3(0, 0, 0);
		// has intersect
		if (colBoxList.size() > 0) {
			cout << "terrain hit" << endl;

			float speedmeasure = abs(landerVelocity.x) + abs(landerVelocity.y) + abs(landerVelocity.z);
			//cout << "lander velocity: " << landerVelocity << "speedmeasure: " << speedmeasure << endl;
			

			if (landerVelocity.y < -20) {
				//cout << "explosion" << endl;
				explosionEmitter.start();
				
				bExploded = true;
				bHasControl = false;
				engineSound.stop();
				explosionSound.play();
			}
			else {						
				//cout << "touchdown" << endl;


				//swap out for restitution/bounce force
				for (int i = 0; i < colNodeList.size(); i++) {
					//octree.mesh.getNormal(colNodeList[i].points[0]);
					avgNormal += octree.mesh.getNormal(colNodeList[i].points[0]);
				}



				//need to swap glm::vec3 with normal angle/force?
				//bounceForce = (1 + float(restitution)) * (dot(-landerVelocity, glm::vec3(0, -1, 0)) * glm::vec3(0, -1, 0));
				bounceForce = (1 + float(restitution)) * (dot(-landerVelocity, avgNormal/50) * avgNormal/50);
				landerVelocity = glm::vec3(landerVelocity.x, -landerVelocity.y, landerVelocity.z) * restitution;

				glm::vec3 setpos = lander.getPosition() + glm::vec3(0, 0.1, 0);
				lander.setPosition(setpos.x, setpos.y, setpos.z);


				landerForce = bounceForce;
			}

			
		}
		
		
		// boxes for the rings
		for (int i = 0; i < ringList.size(); i++) {
			ofVec3f ringmin = ringList[i].getSceneMin() * 3 + ringList[i].getPosition();
			ofVec3f ringmax = ringList[i].getSceneMax() * 3 + ringList[i].getPosition();

			Box ringBounds = Box(Vector3(ringmin.x, ringmin.y, ringmin.z), Vector3(ringmax.x, ringmax.y, ringmax.z));
			
			Vector3 landerVec3 = Vector3(lander.getPosition().x, lander.getPosition().y, lander.getPosition().z);
			if (ringBounds.inside(landerVec3) && bStart) {
				cout << "passed ring" << endl;
				ringSound.play();
				ringList.erase(ringList.begin() + i);
			}

		}

		/*
		//box for pad
		ofVec3f padmin = pad.getSceneMin() + pad.getPosition();
		ofVec3f padmax = pad.getSceneMax() + pad.getPosition();

		Box padBounds = Box(Vector3(padmin.x, padmin.y, padmin.z), Vector3(padmax.x, padmax.y, padmax.z));
		*/
		
		if (bHasControl) {
			//handle movement
		//WASD
			if (bFoward) {
				landerForce += glm::vec3(landerHeading.x * thrust, landerHeading.y * thrust, landerHeading.z * thrust);
			}
			if (bBackward) {
				landerForce -= glm::vec3(landerHeading.x * thrust, landerHeading.y * thrust, landerHeading.z * thrust);
			}
			if (bLeft) {
				glm::vec3 dirVector = glm::cross(landerHeading, glm::vec3(0, 1, 0));
				landerForce -= glm::vec3(dirVector.x * thrust, dirVector.y * thrust, dirVector.z * thrust);
			}
			if (bRight) {
				glm::vec3 dirVector = glm::cross(landerHeading, glm::vec3(0, 1, 0));
				landerForce += glm::vec3(dirVector.x * thrust, dirVector.y * thrust, dirVector.z * thrust);
			}

			//arrows
			if (bUp) {
				landerForce += glm::vec3(landerHeading.x * 1, landerHeading.y + 1 * thrust, landerHeading.z * 1);
			}
			if (bDown) {
				landerForce -= glm::vec3(landerHeading.x * 1, landerHeading.y + 1 * thrust, landerHeading.z * 1);
			}
			if (bTurnLeft) {
				landerRotForce += thrust/2 * rotScaler;
			}
			if (bTurnRight) {
				landerRotForce -= thrust/2 * rotScaler;
			}
		}


		

		//if we need gravity should be able to add a downward force to landerForce here
		//might ask prof to see if we can get away without it tho
		//landerForce += glm::vec3(0, -2, 0);

		landerRotAcceleration = (1 / mass) * landerRotForce;
		landerAcceleration = (1 / mass) * landerForce;

		integrate();

		landerForce = glm::vec3(0, 0, 0);
		landerRotForce = 0;
	}
	

}
//--------------------------------------------------------------
// Harrison and Patricia
void ofApp::draw() {

	loadVbo();

	ofBackground(ofColor::white);
	
	ofSetColor(ofColor::white);
	
	
	ofDisableDepthTest();

	




	ofSetColor(ofColor::white);
	backgroundImage.draw(0, 0, 100);
	if (!bHide) gui.draw();
	ofEnableDepthTest();
	ofDrawBitmapString("fps: " + to_string(ofGetFrameRate()), 20, 20);
	ofDrawBitmapString("fuel: " + to_string(battery), 700, 20);

	if (!bStart)
		ofDrawBitmapString("Load drone.obj and press SPACE to start", ofGetWindowWidth()/2, ofGetWindowHeight()/2);

	if (bAltitude) {
		ofDrawBitmapString("Altitude: " + to_string(altitude), 500, 20);
	}

	if (bWin) {
		ofDrawBitmapString("YOU WIN", 300, 300);
	}

	if (bOutOfTime) {
		ofDrawBitmapString("Ran out of fuel", 300, 300);
	}

	if (bExploded) {
		ofDrawBitmapString("You Exploded", 300, 300);
	}
	

	ofEnableDepthTest();


	// draw all the lights 
	//
	keyLight.draw();
	fillLight.draw();
	rimLight.draw();


	// this makes everything look glowy :)
	//
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites();
	


	//cam.begin();
	theCam->begin();
	ofPushMatrix();


	// check if all rings are gone
	if (ringList.size() == 0) {
		
		if (!bOutOfTime) {
			cout << "you win" << endl;
			bWin = true;
		}
		
	}
	else {
		// draw checkpoint rings
		for (int i = 0; i < ringList.size(); i++) {
			//cout << ringList[i].getScale() << endl;

			ringList[i].drawFaces();
		}
	}
	



	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		mars.drawWireframe();
		if (bLanderLoaded) {

			


			//ofPushMatrix();
			//ofMultMatrix(lander.getModelMatrix());
			lander.drawWireframe();
			//ofPopMatrix();

			if (!bTerrainSelected) drawAxis(lander.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		mars.drawFaces();
		//draw landing pad
		pad.drawFaces();
		ofMesh mesh;
		if (bLanderLoaded) {
			//positions the emitter under the lander
			//emitter.setPosition(lander.getPosition());
			
			glm::mat4 landerMatrix = lander.getModelMatrix();
			
			//back right
			//glm::vec3 emitterOffset = glm::vec3(3.1, 2.4, 3.1);
			//front right
			//glm::vec3 emitterOffset = glm::vec3(3.1, 2.4, -3.1);
			//back left
			//glm::vec3 emitterOffset = glm::vec3(-3.1, 2.4, 3.1);
			//front left
			glm::vec3 emitterOffset = glm::vec3(-3.1, 2.4, -3.1);

			explosionEmitter.setPosition(lander.getPosition() + ofVec3f(0, 1, 0));
			//explosionEmitter.setPosition(lander.getPosition());


			glm::vec3 emitterPosition = landerMatrix * glm::vec4(emitterOffset, 1.0);
			emitter.setPosition(emitterPosition);


			shader.begin();

			// delete emitter.draw() for final version
			emitter.draw();
			explosionEmitter.draw();
			particleTex.bind();
			vbo.draw(GL_POINTS, 0, (int)emitter.sys->particles.size());
			particleTex.unbind();
			shader.end();

			if (bDisplayBBoxes) {
			//draw bounding boxes
			ofVec3f min = lander.getSceneMin() + lander.getPosition();
			ofVec3f max = lander.getSceneMax() + lander.getPosition();

			Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
			ofSetColor(ofColor::white);
			ofNoFill();
			Octree::drawBox(bounds);


			/*
			//box for pad
			ofVec3f padmin = pad.getSceneMin() + pad.getPosition();
			ofVec3f padmax = pad.getSceneMax() + pad.getPosition();

			Box padBounds = Box(Vector3(padmin.x, padmin.y, padmin.z), Vector3(padmax.x, padmax.y, padmax.z));
			ofSetColor(ofColor::white);
			ofNoFill();
			Octree::drawBox(padBounds);
			*/

			// draw colliding boxes
			//
			ofSetColor(ofColor::lightBlue);
			for (int i = 0; i < colBoxList.size(); i++) {
				Octree::drawBox(colBoxList[i]);
			}

			
				for (int i = 0; i < ringList.size(); i++) {
					ofVec3f ringmin = (ringList[i].getSceneMin() * 3 + ringList[i].getPosition());
					ofVec3f ringmax = (ringList[i].getSceneMax() * 3 + ringList[i].getPosition());

					Box ringBounds = Box(Vector3(ringmin.x, ringmin.y, ringmin.z), Vector3(ringmax.x, ringmax.y, ringmax.z));
					ofSetColor(ofColor::white);
					ofNoFill();
					Octree::drawBox(ringBounds);
				}
			}
			


			// old delete before submission
			if (bLanderHeading) {
				ofPushMatrix();
				ofMultMatrix(lander.getModelMatrix());
				//ofDrawLine(lander.getPosition(), lander.getPosition() + landerHeading * 50);
				ofPopMatrix();
			}
			
			lander.drawFaces();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
			if (bDisplayBBoxes) {
				ofNoFill();
				ofSetColor(ofColor::white);
				for (int i = 0; i < lander.getNumMeshes(); i++) {
					ofPushMatrix();
					ofMultMatrix(lander.getModelMatrix());
					//ofRotate(-90, 1, 0, 0);

					Octree::drawBox(bboxList[i]);
					ofPopMatrix();

					//ofPoint landerPosPt = lander.getPosition();
					//glm::vec3 landerPosVec(landerPosPt.x, landerPosPt.y, landerPosPt.z);

					ofDrawLine(lander.getPosition(), lander.getPosition() + landerHeading * 50);
				}
			}

			if (bLanderSelected) {

				ofVec3f min = lander.getSceneMin() + lander.getPosition();
				ofVec3f max = lander.getSceneMax() + lander.getPosition();

				Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
				ofSetColor(ofColor::white);
				ofNoFill();
				Octree::drawBox(bounds);

				// draw colliding boxes
				//
				ofSetColor(ofColor::lightBlue);
				for (int i = 0; i < colBoxList.size(); i++) {
					Octree::drawBox(colBoxList[i]);
				}
			}
			
		}
	}
	if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));



	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		mars.drawVertices();
	}

	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, .1);
	}


	// recursively draw octree
	//
	ofDisableLighting();
	int level = 0;
	//	ofNoFill();

	if (bDisplayLeafNodes) {
		octree.drawLeafNodes(octree.root);
		cout << "num leaf: " << octree.numLeaf << endl;
    }
	else if (bDisplayOctree) {
		ofNoFill();
		ofSetColor(ofColor::white);
		octree.draw(numLevels, 0);
	}

	// if point selected, draw a sphere
	//
	if (pointSelected) {
		ofVec3f p = octree.mesh.getVertex(selectedNode.points[0]);
		ofVec3f d = p - cam.getPosition();
		//ofSetColor(ofColor::lightGreen);
		ofSetColor(ofColor::white);
		ofDrawSphere(p, .02 * d.length());
	}

	//draw sphere right below lander
	if (bAltitude) {
		ofVec3f p = octree.mesh.getVertex(selectedNode.points[0]);
		ofVec3f d = p - cam.getPosition();
		//ofSetColor(ofColor::lightGreen);
		ofSetColor(ofColor::white);
		ofDrawSphere(p, .02 * d.length());
	}

	ofPopMatrix();
	//cam.end();
	theCam->end();


	
}


// 
// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	

	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}

// Harrison and Patricia
void ofApp::keyPressed(int key) {

	switch (key) {
	case 'B':
	case 'b':
		bDisplayBBoxes = !bDisplayBBoxes;
		break;
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		bLanderHeading = !bLanderHeading;
		break;
	case 'L':
	case 'l':
		bDisplayLeafNodes = !bDisplayLeafNodes;
		break;
	case 'O':
	case 'o':
		bDisplayOctree = !bDisplayOctree;
		break;
	case 'r':
		cam.reset();
		break;
	/*
	case 's':
		savePicture();
		break;
	*/
	
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'g':
		bHide = !bHide;
		break;
	/*
	case 'w':
		toggleWireframeMode();
		break;
	*/
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;

	//movement controls start here

	// for multiple keys, might get rid of bthrust and just use the bFoward etc
	case 'w':
		//go foward
		bFoward = true;
		bThrust = true;
		break;
	case 'a':
		//go left
		bLeft = true;
		bThrust = true;
		break;
	case 's':
		//go back
		bBackward = true;
		bThrust = true;
		break;
	case 'd':
		//go right
		bRight = true;
		bThrust = true;
		break;
	case OF_KEY_LEFT:
		//turn left
		bTurnLeft = true;
		bThrust = true;
		break;
	case OF_KEY_RIGHT:
		//turn right
		bTurnRight = true;
		bThrust = true;

		break;
	case OF_KEY_UP:
		//go up
		bUp = true;
		bThrust = true;
		break;
	case OF_KEY_DOWN:
		//go down
		bDown = true;
		bThrust = true;
		break;

	//camera controls
	case 't':
		//setCameraTarget();
		tracking.setPosition(theCam->getPosition());
		theCam = &tracking;
		//bTracking = !bTracking;
		bTracking = true;
		bFirstPerson = false;
		bThirdPerson = false;
		break;
	case '1':
		theCam = &firstPerson;
		//bFirstPerson = !bFirstPerson;
		bFirstPerson = true;
		bTracking = false;
		bThirdPerson = false;
		break;
	case '2':
		theCam = &cam;
		bFirstPerson = false;
		bTracking = false;
		bThirdPerson = false;
		break;
	case '3':
		theCam = &thirdPerson;
		//bThirdPerson = !bThirdPerson;
		bThirdPerson = true;
		bTracking = false;
		bFirstPerson = false;
		break;
	case '4':
		bAltitude = !bAltitude;
		break;
	case ' ':
		bStart = true;
		starttime = ofGetElapsedTimef();
		break;
	case 'p':
		explosionEmitter.start();
		break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

// Harrison and Patricia
void ofApp::keyReleased(int key) {

	switch (key) {
	
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;


	//movement controls start here
	case 'w':
		//go foward
		bFoward = false;
		bThrust = false;
		break;
	case 'a':
		//go left
		bLeft = false;
		bThrust = false;
		break;
	case 's':
		//go back
		bBackward = false;
		bThrust = false;
		break;
	case 'd':
		//go right
		bRight = false;
		bThrust = false;
		break;
	case OF_KEY_LEFT:
		//turn left
		bTurnLeft = false;
		bThrust = false;
		break;
	case OF_KEY_RIGHT:
		//turn right
		bTurnRight = false;
		bThrust = false;
		//lander.setRotation(0, 45, 0, 1, 0);
		break;
	case OF_KEY_UP:
		//go up
		bUp = false;
		bThrust = false;
		break;
	case OF_KEY_DOWN:
		//go down
		bDown = false;
		bThrust = false;
		break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

	
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	// if moving camera, don't allow mouse interaction
//
	if (cam.getMouseInputEnabled()) return;

	// if rover is loaded, test for selection
	//
	if (bLanderLoaded) {
		glm::vec3 origin = cam.getPosition();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		bool hit = bounds.intersect(Ray(Vector3(origin.x, origin.y, origin.z), Vector3(mouseDir.x, mouseDir.y, mouseDir.z)), 0, 10000);
		if (hit) {
			landerVelocity = glm::vec3(0, 0, 0);
			landerAcceleration = glm::vec3(0, 0, 0);
			bLanderSelected = true;
			mouseDownPos = getMousePointOnPlane(lander.getPosition(), cam.getZAxis());
			mouseLastPos = mouseDownPos;
			bInDrag = true;
		}
		else {
			bLanderSelected = false;
		}
	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

bool ofApp::raySelectWithOctree(ofVec3f &pointRet) {
	ofVec3f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
		Vector3(rayDir.x, rayDir.y, rayDir.z));

	//log time for mouse ray intersect
	float t1 = ofGetElapsedTimeMicros();
	pointSelected = octree.intersect(ray, octree.root, selectedNode);
	float t2 = ofGetElapsedTimeMicros();
	timeFile << "Time to intersect: " << t2 - t1 << " microsec" << endl;
	if (bTimingInfo) {
		cout << "Time to intersect: " << t2 - t1 << " microsec" << endl;
	}
	

	if (pointSelected) {
		pointRet = octree.mesh.getVertex(selectedNode.points[0]);
	}
	return pointSelected;
}


bool ofApp::raySelectWithOctreeForAltitude(ofVec3f& pointRet) {
	//ofVec3f mouse(mouseX, mouseY);
	if (bLanderLoaded) {

		ofVec3f rayPoint = lander.getPosition();
		ofVec3f rayDir = glm::vec3(0, -1, 0);
		rayDir.normalize();
		Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
			Vector3(rayDir.x, rayDir.y, rayDir.z));

		pointSelected = octree.intersect(ray, octree.root, selectedNode);

		if (pointSelected) {
			pointRet = octree.mesh.getVertex(selectedNode.points[0]);
			altitude = rayPoint.distance(pointRet);
			//cout << "altitude: " << altitude << endl;
		}
	}
	return pointSelected;
}




//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	if (bInDrag) {

		glm::vec3 landerPos = lander.getPosition();

		glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam.getZAxis());
		glm::vec3 delta = mousePos - mouseLastPos;
	
		landerPos += delta;
		lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
		mouseLastPos = mousePos;

		// collision detection code
		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		colBoxList.clear();
		octree.intersect(bounds, octree.root, colBoxList, colNodeList);
	

		/*if (bounds.overlap(testBox)) {
			cout << "overlap" << endl;
		}
		else {
			cout << "OK" << endl;
		}*/
		


	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bInDrag = false;
}



// Set the camera to use the selected point as it's new target
//  

//delete before submission of not needed
void ofApp::setCameraTarget() {
	//bTracking = !bTracking;

	cam.setTarget(lander.getPosition());
	if (bTracking) {
		cam.setTarget(lander.getPosition());
	}
	/*
	else {
		cam.removeAllInteractions();
	}*/

}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent2(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);
	if (lander.loadModel(dragInfo.files[0])) {
		lander.setScaleNormalization(false);
//		lander.setScale(.1, .1, .1);
	//	lander.setPosition(point.x, point.y, point.z);
		lander.setPosition(1, 1, 0);

		bLanderLoaded = true;
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		cout << "Mesh Count: " << lander.getMeshCount() << endl;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (lander.loadModel(dragInfo.files[0])) {
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		lander.setPosition(0, 0, 0);
		cout << "number of meshes: " << lander.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		//		lander.setRotation(1, 180, 1, 0, 0);

				// We want to drag and drop a 3D object in space so that the model appears 
				// under the mouse pointer where you drop it !
				//
				// Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
				// once we find the point of intersection, we can position the lander/lander
				// at that location.
				//

				// Setup our rays
				//
		glm::vec3 origin = cam.getPosition();
		glm::vec3 camAxis = cam.getZAxis();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}


}

//  intersect the mouse ray with the plane normal to the camera 
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
	// Setup our rays
	//
	glm::vec3 origin = cam.getPosition();
	glm::vec3 camAxis = cam.getZAxis();
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}

void ofApp::integrate() {

	// init current framerate 
	//
	float framerate = ofGetFrameRate();
	float dt = 1.0 / framerate;
	
	//scout << lander.getPosition() << endl;
	glm::vec3 landerpos = lander.getPosition() + landerVelocity * dt;
	lander.setPosition(landerpos.x, landerpos.y, landerpos.z);
	landerVelocity = landerVelocity + landerAcceleration * dt;
	landerVelocity = landerVelocity * damping;

	float landerrot = lander.getRotationAngle(0) + landerRotVelocity * dt;
	lander.setRotation(lander.getNumRotations(), landerrot, 0, 1, 0);
	landerRotVelocity = landerRotVelocity + landerRotAcceleration * dt;
	landerRotVelocity = landerRotVelocity * damping;
	
}