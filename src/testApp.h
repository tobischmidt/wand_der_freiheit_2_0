#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ball.h"
#include "chef.h"
#include "verfolger.h"
#include "ofxGLWarper.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

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
        #endif

	    ofxCvGrayscaleImage grayImage; // grayscale depth image
	    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image

    	ofxCvContourFinder contourFinder;

	    bool bThreshWithOpenCV;

	    int nearThreshold;
	    int farThreshold;

	    int angle;

	    ofPoint leftEnd;
	    ofPoint rightEnd;


    private:

        float timeCur;
        float timeOld;
        int nVerfolger;
        int nChef;

};

#endif
