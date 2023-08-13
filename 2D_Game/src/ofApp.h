#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "Shape.h"
#include "TransformObject.h"




class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		void toggleImage();
		
		Emitter  *emitter = NULL;
		Target   *target = NULL;
		ParticleEmitter  *gun = NULL;
		ParticleEmitter *explosion = NULL;


		// forces for explosion
		TurbulenceForce* turbForce;
		ImpulseRadialForce* radialForce;

		ofImage defaultImage;
		ofVec3f mouse_last;
		bool imageLoaded;

		// Some basic UI
		//
		bool bHide;

		ofxButton level1;
		ofxButton level2;
		ofxButton level3;

		ofxIntSlider level;
		ofxFloatSlider rate;
		ofxFloatSlider life;
		ofxVec3Slider velocity;
		ofxLabel screenSize;
		ofxFloatSlider scale;
		ofxFloatSlider rotationSpeed = 3.0;
		ofxIntSlider energy;

		ofxFloatSlider pspeed;
		ofxFloatSlider sspeed;
		ofxFloatSlider pscale;
		ofxIntSlider nAgents;

		ofxFloatSlider bulletsize;
		ofxFloatSlider bulletspeed;
		ofxFloatSlider bulletrate;


		ofxPanel gui;

		// for sprite dragging
		glm::vec3 mouseLast;
		bool bDragging = false;

		//for target dragging
		glm::vec3 mouseLastT;
		bool targetDragging = false;

		bool gameover = false;

		// for screen text display
		int nEnergy;
		string typeStr;
		string framerate;
		string elapsedtime;
		float lasttime;
		ofTrueTypeFont verdana30;
		ofTrueTypeFont verdana10;


		bool startToggle = false;
		bool playerImageLoaded;
		ofImage playerImage;

		bool backgroundImageLoaded;
		ofImage backgroundImage;


		// booleans for player movement
		bool foward;
		bool backward;
		bool turnleft;
		bool turnright;	


		// sounds
		ofSoundPlayer engineSound;
		ofSoundPlayer explosionSound;
		ofSoundPlayer impactSound;


		int levelSelect = 1;

};
