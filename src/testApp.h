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
        void draw();
	    void exit();

	    void drawContours();

        void keyPressed(int);
        void mouseReleased(int x, int y, int button);

        ofImage background;
        ofImage zitat;


//-------------------------------V�GEL------------------------------------

        Ball** theBall;
        Chef** theChef;
        Verfolger** theVerfolger;

//------------------------------TRACKING----------------------------------

	    bool tracking;
	    bool enddraw;

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

	    bool bThreshWithOpenCV;

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

//		ofPolyline curve2;
//		ofPolyline curve3;
//		ofPolyline curve4;
//
//		vector<ofVec2f> zaun2;
//		vector<ofVec2f> zaun3;
//		vector<ofVec2f> zaun4;

		vector<vector<ofVec2f> > curveDefine;
		vector<ofPolyline> curve;

		int counter;

		ofFbo trace;

		bool setzen;
		bool linien;
		bool transformation;

		int runCounter;
		float distanceCounter;

    private:

        float timeCur;
        float timeOld;
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

};

#endif
