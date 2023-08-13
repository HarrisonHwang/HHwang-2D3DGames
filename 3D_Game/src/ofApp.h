#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "Octree.h"
#include <glm/gtx/intersect.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Particle.h"
#include "ParticleEmitter.h"
using namespace std;



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
		void dragEvent2(ofDragInfo dragInfo);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void toggleSelectTerrain();
		void setCameraTarget();
		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
		bool raySelectWithOctree(ofVec3f &pointRet);
		bool raySelectWithOctreeForAltitude(ofVec3f& pointRet);
		glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 p , glm::vec3 n);

		//cameras
		ofEasyCam cam;
		ofCamera firstPerson;
		ofCamera thirdPerson;	//get rid of this shit if it's too hard to figure out
		ofCamera tracking;

		ofCamera *theCam;

		bool bTracking;
		bool bFirstPerson;
		bool bThirdPerson;

		//background
		ofImage backgroundImage;
		bool bBackgroundImageLoaded;

		

		ofxAssimpModelLoader mars, lander;
		ofLight light;
		Box boundingBox, landerBounds;
		Box testBox;
		vector<Box> colBoxList;
		bool bLanderSelected = false;
		Octree octree;
		TreeNode selectedNode;
		glm::vec3 mouseDownPos, mouseLastPos;
		bool bInDrag = false;

		bool bTimingInfo = false;
		bool bTimingWorkaround;
		ofxIntSlider numLevels;
		ofxFloatSlider thrust;
		ofxToggle timeToggle;
		ofxPanel gui;

		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		bool bHide;
		bool pointSelected = false;
		bool bDisplayLeafNodes = false;
		bool bDisplayOctree = false;
		bool bDisplayBBoxes = false;
		
		bool bLanderLoaded;
		bool bTerrainSelected;
		bool bLanderHeading;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;

		vector<Box> bboxList;

		const float selectionRange = 4.0;

		//delete if unecessary
		ofFile timeFile;


		// a bunch of variables for the lander
		// since I don't want to touch the ofxAssimpModelLoader files
		glm::vec3 landerHeading = glm::vec3(0, 0, 0);

		
		void setHeading(glm::mat4 matrix) {
			//glm::mat3 rotMatrix(matrix);

			glm::vec3 foward = matrix * glm::vec4(0, 0, -1, 0);
			glm::vec3 fowardVec = glm::vec3(foward.x, foward.y, foward.z);
			fowardVec = glm::normalize(fowardVec);

			landerHeading = fowardVec;
		}

		/*
		void setHeading(float angle) {
			landerHeading = glm::rotate(glm::radians(angle + 180), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1);
		}*/

		float damping = .99;
		float mass = 1.0;

		glm::vec3 landerVelocity = glm::vec3(0, 0, 0);
		glm::vec3 landerAcceleration = glm::vec3(0, 0, 0);
		glm::vec3 landerForce = glm::vec3(0, 0, 0);

		float landerRotVelocity = 0;
		float landerRotAcceleration = 0;
		float landerRotForce = 0;

		// rotation amout is scaled based off thrust
		// this determines how much to rotate
		float rotScaler = 10;


		void integrate();


		//gamestate bools
		bool bStart = false;
		bool bWin = false;
		bool bOutOfTime = false;
		bool bExploded = false;

		float starttime;
		float battery = 120;


		bool bHasControl = true;

		//wasd
		bool bFoward = false;
		bool bBackward = false;
		bool bLeft = false;
		bool bRight = false;

		//arrows
		bool bUp = false;
		bool bDown = false;
		bool bTurnLeft = false;
		bool bTurnRight = false;

		float restitution = 0.85;

		// for sound and emitters
		bool bThrust = false; 

		// sounds
		ofSoundPlayer engineSound;
		ofSoundPlayer explosionSound;
		ofSoundPlayer ringSound;

		//decoration pad to show off lighting
		ofxAssimpModelLoader pad;

		//lighting
		ofLight keyLight, fillLight, rimLight;


		// for checkpoint rings
		int numRings = 5;
		//list to hold all the rings
		vector<ofxAssimpModelLoader> ringList;

		//bool for altitude display
		bool bAltitude;
		float altitude;


		void loadVbo();


		// textures
		//
		ofTexture  particleTex;

		ofVbo vbo;
		ofShader shader;
		//thruster 
		ParticleEmitter emitter;
		ParticleEmitter explosionEmitter;
		//ParticleEmitter emitter1;
		//ParticleEmitter emitter2;
		//ParticleEmitter emitter3;
		//ParticleEmitter emitter4;

		TurbulenceForce* turbForce;
		GravityForce* gravityForce;
		ImpulseRadialForce* radialForce;


		vector<TreeNode> colNodeList;



};
