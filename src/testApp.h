#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ball.h"
#include "chef.h"
#include "verfolger.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

#include "oscHelper.h" /*NEW*/
#define PORT 8000   /*NEW*/
#define NUM_MSG_STRINGS 20  /*NEW*/

#define USE_TWO_KINECTS

class testApp : public ofBaseApp{

    public:

        void setup();
        void update();
        void updateOsc();
        void draw();
	    void exit();

	    void drawContours();

        void keyPressed(int);
        void mouseReleased(int x, int y, int button);

        ofImage background;
        ofImage zitat;


//-------------------------------VÖGEL------------------------------------

        Ball** theBall;
        Chef** theChef;
        Verfolger** theVerfolger;

//------------------------------TRACKING----------------------------------

	    bool tracking;
	    bool enddraw;
	    bool hasContours;

	    ofxKinect kinect;

#ifdef USE_TWO_KINECTS
        ofxKinect kinect2;

        ofxCvGrayscaleImage grayImage2; // grayscale depth image
	    //ofxCvGrayscaleImage grayThreshNear2; // the near thresholded image
	    //ofxCvGrayscaleImage grayThreshFar2; // the far thresholded image

	    ofxCvContourFinder contourFinder2;

#endif

	    ofxCvGrayscaleImage grayImage; // grayscale depth image
	    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image

    	ofxCvContourFinder contourFinder;

	    float nearThreshold;
	    float farThreshold;
	    float angle;

	    ofPoint attraktoren[8];

	    ofPoint leftEnd[4];
	    ofPoint rightEnd[4];

	    float startX;
	    float startY;

        int blubb;
        ofPolyline line;

	    float adjustmentX;
	    float adjustmentY;
	    float adjustment2X;
	    float adjustment2Y;

	    float contourScaleWidth;
	    float contourScaleHeight;

        int current_msg_string; /*NEW*/
		string msg_strings[NUM_MSG_STRINGS];    /*NEW*/
		float timers[NUM_MSG_STRINGS];  /*NEW*/

		ofImage vogelTextur;
		ofImage drahtTextur;

		vector<ofPolyline> contours;

		vector<vector<ofVec2f> > curveDefine;
		vector<ofPolyline> curve;
		vector<ofVec2f> vec;

		int counter;
		int endCounter;
		int zitatCounter;

		ofFbo trace;

		bool setzen;
		bool linien;
		bool transformation;

		bool doOscUpdate;

		int runCounter;

    private:

        float timeCur;
        float timeOld;
        float timeDiff;

        int nVerfolger;
        int nChef;
        bool createVerfolger; /*NEW*/
        float texturWidth; /*NEW*/
        float texturHeight; /*NEW*/
        float speed; /*NEW*/
        float par1; /*NEW*/
        float rangeWidth; /*new*/
        float grauwert; /*new*/

        oscHelper osc; /*NEW*/
        ofPoint position;

        //Fenstergröße
        int windowWidth;
        int windowHeight;

};

#endif
