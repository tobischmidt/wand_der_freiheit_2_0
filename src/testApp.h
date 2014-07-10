#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ball.h"
#include "chef.h"
#include "verfolger.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

#include "oscHelper.h"
#define PORT 8000
#define NUM_MSG_STRINGS 20

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

        ofImage zitat;  //Zitat als png


//-------------------------------V�GEL------------------------------------

        vector<Chef*> theChef;

        vector<Verfolger*> theVerfolger;
        vector<Verfolger*>::iterator verfolgerIt;

//------------------------------TRACKING----------------------------------

	    bool tracking;  //tracking an-/ausschalten
	    bool hasContours;  //ein Objekt wird getrackt

	    ofxKinect kinect;

	    ofxCvGrayscaleImage grayImage; // grayscale depth image
	    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image

    	ofxCvContourFinder contourFinder;

#ifdef USE_TWO_KINECTS
        ofxKinect kinect2;

        ofxCvGrayscaleImage grayImage2; // grayscale depth image

	    ofxCvContourFinder contourFinder2;

#endif

	    float nearThreshold;
	    float farThreshold;
	    float angle;  //camera winkel

	    ofPoint attraktoren[4];  //attraktorpunkte

	    ofPoint leftEnd[2];  //linkesten Punkte der getrackten Objekte
	    ofPoint rightEnd[2];  //rechtesten Punkte der getrackten Objekte

	    //startpunkt wenn neuer Vogel erstellt wird
	    float startX;
	    float startY;

        ofPath blubbs;  //Platzhalter zur Initialisierung von contours
        ofPolyline line;  //Platzhalter zur Initialisierung von curve

	    float adjustmentX;  //Verschiebung in x-Richtung erste Silhouette
	    float adjustmentY;  //Verschiebung in y-Richtung erste Silhouette
	    float adjustment2X;  //Verschiebung in x-Richtung zweite Silhouette
	    float adjustment2Y;  //Verschiebung in y-Richtung zweite Silhouette

	    float lineAdjustmentX;  //Linien von Animation verschieben in x-Richtung
	    float lineAdjustmentY;  //Linien von Animation verschieben in y-Richtung

	    float contourScaleWidth;  //Breite der Silhouetten skalieren
	    float contourScaleHeight;  //H�he der Silhouetten skalieren

	    float grauwertKontur;
	    float konturDicke;
	    float spurLaenge;  //L�nge der nachgezogenen Spur der Silhouetten

		ofImage vogelTextur;  //png mit 64 Frames der Vogeltextur als Fluganimation
		ofImage drahtTextur;  //png mit 21 Frames zur Verwandlung von Vogel in Stacheldraht

		vector<ofPath> contours;  //Konturen der Silhouetten

		ofPoint curveDefine1[420];  //oberste Linie der Animation vorgefertigt
		ofPoint curveDefine2[420];  //zweite Linie der Animation vorgefertigt
		ofPoint curveDefine3[420];  //dritte Linie der Animation vorgefertigt
		ofPoint curveDefine4[420];  //vierte Linie der Animation vorgefertigt
		vector<ofPolyline> curve;  //Linien der Animation, die erst zur Laufzeit gezeichnet werden
		vector<ofVec2f> vec;  //Platzhalter zur Initialisierung von line

		int lineCounter;  //Z�hler, startet wenn Linien einluafen sollen
		int endCounter;  //Z�hler, startet wenn setzen aktiviert ist
		int zitatCounter;  //Z�hler, l�sst Zitat langsam einblenden

		ofFbo trace;  //Frame Buffer Object f�r Spur der Silhouetten

		bool setzen;  //V�gel sollen sich auf Linien setzen
		bool linien;  //Linien sollen einlaufen
		bool transformation;  //V�gel sollen sihc zu Stacheldraht umwandeln
		bool blend;  //Bild wird schwarz �berblendet

		bool doOscUpdate;  //OSC soll upgedatet werden

		int runCounter;  //Z�hler, der jeden durchlauf der Update mitl�uft und alle 150 Durchg�nge zur�ckgesetzt wird

		int blendCounter;  //Z�hler, der mit �berblendung mitl�uft

    private:

        float timeCur;  //aktuelle Zeit
        float timeOld;  //alte Zeit
        float timeDiff;  //Zeitdifferenz

        int nVerfolger;  //Zahl der Verfolger
        int nChef;  //Zahl der Chefs
        bool createVerfolger;  //Verfolger erstellen
        float texturWidth;  //Breite der Vogeltextur
        float texturHeight;  //H�he der Vogeltextur
        float speed;  //Fluggeschwindigkeit
        float par1;  //Parameter zur Orientierung in alte Bewegungsrichtung
        float rangeWidth;  //x-Koordinate des Bereichs, in dem die V�gel fliegen sollen
        float grauwert;  //Grauwert der Konturf�llung

        oscHelper osc;
        ofPoint position;  //Neue Position, zu der die V�gel fliegen sollen

        //Fenstergr��e
        int windowWidth;
        int windowHeight;

        int platzhalter;  //Entspricht Zahl der Verfolger, aber maximal 40 -> Dient zur Einordnung von neuen Verfolgern in den ersten 40 Indizen, damit diese gleich zur Silhouette fliegen

};

#endif
