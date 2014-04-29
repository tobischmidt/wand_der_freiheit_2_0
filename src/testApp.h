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

        //Ball** theBall;
        vector<Chef*> theChef;

        vector<Verfolger*> theVerfolger;
        vector<Verfolger*>::iterator verfolgerIt;

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

        ofPath blubbs;
        ofPolyline line;

	    float adjustmentX;
	    float adjustmentY;
	    float adjustment2X;
	    float adjustment2Y;

	    float lineAdjustmentX;
	    float lineAdjustmentY;

	    float contourScaleWidth;
	    float contourScaleHeight;

	    float grauwertKontur;
	    float konturDicke;
	    float spurLaenge;

        int current_msg_string; /*NEW*/
		string msg_strings[NUM_MSG_STRINGS];    /*NEW*/
		float timers[NUM_MSG_STRINGS];  /*NEW*/

		ofImage vogelTextur;
		ofImage drahtTextur;

		//vector<ofPolyline> contours;
		vector<ofPath> contours;


		//vector<vector<ofVec2f> > curveDefine;
		ofPoint curveDefine1[420];
		ofPoint curveDefine2[420];
		ofPoint curveDefine3[420];
		ofPoint curveDefine4[420];
		vector<ofPolyline> curve;
		vector<ofVec2f> vec;

		int lineCounter;
		int endCounter;
		int zitatCounter;

		ofFbo trace;

		bool setzen;
		bool linien;
		bool transformation;
		bool blend;

		bool doOscUpdate;

		int runCounter;

		int blendCounter;

    private:

        float timeCur;
        float timeOld;
        float timeDiff;

        int nVerfolger;
        int nChef;
        bool createVerfolger;
        float texturWidth;
        float texturHeight;
        float speed;
        float par1;
        float rangeWidth;
        float grauwert;

        oscHelper osc;
        ofPoint position;

        //Fenstergröße
        int windowWidth;
        int windowHeight;

        int platzhalter;

};

#endif
