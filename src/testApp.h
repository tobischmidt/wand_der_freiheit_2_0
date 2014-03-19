#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ball.h"
#include "chef.h"
#include "verfolger.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

#define USE_TWO_KINECTS

class testApp : public ofBaseApp{

    public:

        void setup();
        void update();
        void draw();
	    void exit();

        void keyPressed(int);
        void mouseReleased(int x, int y, int button);


//-------------------------------VÖGEL------------------------------------

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

	    int nearThreshold;
	    int farThreshold;

	    int angle;

	    ofPoint attraktoren[8];

	    ofPoint leftEnd[4];
	    ofPoint rightEnd[4];

	    float startX;
	    float startY;

	    float adjustmentX;
	    float adjustmentY;
	    float adjustment2X;
	    float adjustment2Y;

	    float contourScaleWidth;
	    float contourScaleHeight;

    private:

        float timeCur;
        float timeOld;
        int nVerfolger;
        int nChef;

};

#endif
