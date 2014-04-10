#include "testApp.h"
#include "chef.h"
#include "verfolger.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

//--------------------------------------------------------------

void testApp::setup()
{
//---------------------------VÖGEL------------------------------------------

    vogelTextur.loadImage("Vögel_weiß_Var3.png");

    nVerfolger = 3;

    nChef = 3;

    ofSetWindowTitle("Wand der Freiheit");

    theChef = new Chef*[nChef];
    theVerfolger = new Verfolger*[nVerfolger];

    //nChef Chefs werden vordefiniert
    for (int i = 0; i < nChef; i++)
    {
        ofPoint pos = ofPoint(ofRandom(1), ofRandom(1));
        float speed = 0.00015;/*NEW*/
        float texturWidth =  35; /*NEW*/
        float texturHeight =  20; /*NEW*/
        float grauwert = 255; /*new*/

        //erstellt ein Objekt mit den Koordinaten und dim
        theChef[i] = new Chef(pos, speed/*NEW*/, texturWidth/*NEW*/, texturHeight/*NEW*/, rangeWidth/*new*/, grauwert/*new*/);
    }


    //nVerfolger Verfolger wird vordefiniert
    for (int i = 0; i < nVerfolger; i++)
    {
        ofPoint pos = ofPoint(ofRandom(1), ofRandom(1));
        float speed = 0.00015;/*NEW*/
        float createVerfolger = 0; /*NEW*/
        float texturWidth =  35; /*NEW*/
        float texturHeight =  20; /*NEW*/
        float startX = 0.99;    /*NEW*/
        float startY = 0.5; /*NEW*/

        // erstellt ein Objekt mit den Koordinaten
        theVerfolger[i] = new Verfolger(pos, speed/*NEW*/, texturWidth/*NEW*/, texturHeight/*NEW*/, rangeWidth/*new*/, grauwert/*new*/);
    }

    timeOld = ofGetElapsedTimeMillis();
    timeCur = timeOld;


//-------------------------TRACKING-----------------------------------------------


    //Tracking & Zeichnen der Endpunkte zunächst ausgeschalten
    tracking = false;
    enddraw = false;

    ofSetLogLevel(OF_LOG_VERBOSE);

    // enable depth->video image calibration
    kinect.setRegistration(false);

    //kinect.init();
    kinect.init(false, false); // disable video image (faster fps)

    kinect.open();		// opens first available kinect
    //kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))

    // print the intrinsic IR sensor values
    /*if(kinect.isConnected())
    {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }*/


    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);

    nearThreshold = 255;
    farThreshold = 0;
    bThreshWithOpenCV = true;

    ofSetFrameRate(60);

    // set the tilt to 0 on startup
    angle = 0;
    kinect.setCameraTiltAngle(angle);

    osc.setup(); /*NEW*/

#ifdef USE_TWO_KINECTS

    kinect2.init(false, false);
    kinect2.open();

    grayImage2.allocate(kinect2.width, kinect2.height);

    kinect2.setCameraTiltAngle(angle);
#endif

    adjustmentX = 0; /*NEW*/
    adjustmentY = 0;        /*NEW*/
    adjustment2X = ofGetScreenWidth() /2; /*NEW*/
    adjustment2Y = 0;   /*NEW*/

    contourScaleWidth = ofGetScreenWidth();    /*NEW*/
    contourScaleHeight = ofGetScreenHeight();  /*NEW*/

    trace.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGBA32F_ARB);

    trace.begin();
	ofClear(255,255,255, 0);
    trace.end();

    //--------------------------ABSCHLUSS-----------------------------------------------------

    //fencePoints.push_back(ofPoint(ofGetScreenWidth(), 350));
    //fencePoints.push_back(ofPoint(ofGetScreenWidth()/2, 700));

    curve.addVertex(ofGetScreenWidth(), 500);

    //curve.addVertex(fencePoints[0]);
    //curve.addVertex(fencePoints[1]);

    /*curve.curveTo(ofGetScreenWidth()+100, 370);
    curve.curveTo(ofGetScreenWidth(), 550);
    curve.curveTo(ofGetScreenWidth() - 300, 600);
    curve.curveTo(ofGetScreenWidth()/2, 500);
    curve.curveTo(ofGetScreenWidth()/4, 550);
    curve.curveTo(400, 200);*/

    /*fence.push_back(curve);
    curve.clear();

    curve.curveTo(ofGetScreenWidth()+100, 350);
    curve.curveTo(ofGetScreenWidth(), 500);
    curve.curveTo(ofGetScreenWidth() - 300, 650);
    curve.curveTo(ofGetScreenWidth()/2, 450);
    curve.curveTo(ofGetScreenWidth()/4, 300);
    curve.curveTo(400, 200);

    fence.push_back(curve);
    curve.clear();

    curve.curveTo(ofGetScreenWidth()+100, 250);
    curve.curveTo(ofGetScreenWidth(), 350);
    curve.curveTo(ofGetScreenWidth() - 300, 500);
    curve.curveTo(ofGetScreenWidth()/2, 350);
    curve.curveTo(ofGetScreenWidth()/4, 200);
    curve.curveTo(400, 200);

    fence.push_back(curve);
    curve.clear();*/

   counter = 0;

   for(int i=0; i<200; i++)
   {
       zaun.push_back(ofVec2f(ofGetScreenWidth() - (i*10), 500));
   }

   for(float i=0; i<200; i++)
   {
       if(i <= 54)
       {
           zaun2.push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/15) * 50 + 600));
       }
       else if(i > 54 && i <=  122)
       {
           zaun2.push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/40 + M_PI*538/1000) * 140 + 564));
       }
       else
       {
           zaun2.push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/40 + M_PI*538/1000) * 70 + 494));
       }
   }

   for(float i=0; i<200; i++)
   {
       if(i <= 54)
       {
           zaun3.push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/15 + M_PI) * 120 + 400));
       }
       else if(i > 54 && i <= 95)
       {
           zaun3.push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/20 + M_PI) * 170 + 526));
       }
       else
       {
           zaun3.push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/40 + M_PI*1738/1000) * 30 + 666));
       }
   }

   for(float i=0; i<200; i++)
   {
       if(i <= 100)
       {
           zaun4.push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/30 + M_PI*3/4) * 100 + 430));
       }
       else if(i > 100 && i <= 150)
       {
           zaun4.push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/20 + M_PI*5/100) * 125 + 487));
       }
       else
       {
           zaun4.push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/40 + M_PI*1738/1000 + M_PI*3/2) * 100 + 512));
       }
   }

   setzen = false;
   linien = false;
}

//--------------------------------------------------------------

void testApp::update()
{
    if(linien)
    {
        counter++;

        if((counter%2) == 0 && counter/2 < zaun.size())
        {
            curve.lineTo(zaun[counter/2].x, zaun[counter/2].y);
            //cout << "new point \n";
        }

        if((counter%2) == 0 && counter/2 < zaun2.size())
        {
            curve2.curveTo(zaun2[counter/2].x, zaun2[counter/2].y);
            //cout << "new point" << zaun2[counter/2].y << "\n";
        }

        if((counter%2) == 0 && counter/2 < zaun2.size())
        {
            curve3.curveTo(zaun3[counter/2].x, zaun3[counter/2].y);
            //cout << "new point" << zaun2[counter/2].y << "\n";
        }

        if((counter%2) == 0 && counter/2 < zaun2.size())
        {
            curve4.curveTo(zaun4[counter/2].x, zaun4[counter/2].y);
            //cout << "new point" << zaun2[counter/2].y << "\n";
        }
    }


    //Hintergrundfarbe schwarz
    ofBackground(50,50,50);

    osc.listen();/*NEW*/

    if(osc.settings[8] == 1){
        tracking = true;
    }
    /*else{
        tracking = false;
    }*/
    //cout << ofToString(tracking) << "\n" ;


    if(osc.settings[9] != 0)
    {
        nearThreshold = osc.settings[9];
    }
    else
    {
        nearThreshold = 255;
    }

    farThreshold = osc.settings[10];

    // verschiebung 1 x
    if(osc.settings[7] != 0)
    {
        adjustmentX = osc.settings[7] * ofGetWidth(); /*NEW*/
    }
    else
    {
        adjustmentX = 0;
    }

    // verschiebung 1 y
    if(osc.settings[20] != 0)
    {
        adjustmentY = osc.settings[20] * ofGetHeight(); /*NEW*/
    }
    else
    {
        adjustmentY = 0;
    }

    if(osc.settings[18] != 0)
    {
        adjustment2X = osc.settings[18] * ofGetWidth(); /*NEW*/
    }
    else
    {
        adjustment2X = 0;
    }

    if(osc.settings[21] != 0)
    {
        adjustment2Y = osc.settings[21] * ofGetHeight(); /*NEW*/
    }
    else
    {
        adjustment2Y = 0;
    }

    if(osc.settings[6] != 0)
    {
        contourScaleWidth = osc.settings[6] * ofGetWidth();    /*NEW*/
    }
    else
    {
        contourScaleWidth = ofGetWidth();
    }

    if(osc.settings[17] != 0)
    {
        contourScaleHeight = osc.settings[17] * ofGetHeight();  /*NEW*/
    }
    else
    {
        contourScaleHeight = ofGetHeight();
    }

    if(osc.settings[12] != 0)
    {
        blubb = ofGetWidth() - (osc.settings[12] * ofGetWidth());  /*NEW*/
    }

//----------------------------------TRACKING------------------------------------------------------------

    kinect.update();

    // there is a new frame and we are connected
    if(kinect.isFrameNew())
    {
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        // spiegelt das Bild
        grayImage.mirror(false,true);
        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        if(bThreshWithOpenCV)
        {
            grayThreshNear = grayImage;
            grayThreshFar = grayImage;
            grayThreshNear.threshold(nearThreshold, true);
            grayThreshFar.threshold(farThreshold);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        }
        else
        {
            // or we do it ourselves - show people how they can work with the pixels
            unsigned char * pix = grayImage.getPixels();

            int numPixels = grayImage.getWidth() * grayImage.getHeight();
            for(int i = 0; i < numPixels; i++)
            {
                if(pix[i] < nearThreshold && pix[i] > farThreshold){
                    pix[i] = 255;
                }
                else{
                    pix[i] = 0;
                }
            }
        }

        // update the cv images
        grayImage.flagImageChanged();

        // Wenn Tracking aktiviert wird
        // find contours which are between the size of 10 pixels and 1/2 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        if (tracking) {
           contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 2, false);
        }
    }


    leftEnd[0].set(2000, 500); //linkes/rechtes Ende der von der ersten Kinect erkannten Person
    rightEnd[0].set(0, 500);
    leftEnd[1].set(2000, 500); //linkes/rechtes Ende der von der ersten Kinect erkannten Person
    rightEnd[1].set(0, 500);

    if(contourFinder.blobs.size() > 0)//Wenn mindestens ein Objekt erkannt wird
    {
        for (int j=0; j< 2; j++)
        {
            for( int i=0; i<contourFinder.blobs[j].nPts; i+=10 )//Iteriert durch die Punkte der Kontur, nimmt nur jeden dritten wegen Laufzeit
            {
                if(contourFinder.blobs[j].pts[i].x/2 < leftEnd[j].x)
                {
                    //Wenn aktueller Punkt weiter links als gespeicherter Punkt wird dieser neu gespeichert
                    //x-Wert durch 2 geteilt, da nur auf linker Bidlschirmhälfte
                    leftEnd[j].set(contourFinder.blobs[j].pts[i].x/2, contourFinder.blobs[j].pts[i].y);
                }

                if(contourFinder.blobs[j].pts[i].x/2 > rightEnd[j].x)
                {
                    //Wenn aktueller Punkt weiter rechts als gespeicherter Punkt wird dieser neu gespeichert
                    rightEnd[j].set(contourFinder.blobs[j].pts[i].x/2, contourFinder.blobs[j].pts[i].y);
                }
            }
        }
    }

//selbe Funktionen wie für die erste Kinect, nur auf rechte Bildschirmhälfte bezogen
#ifdef USE_TWO_KINECTS
	kinect2.update();

	// there is a new frame and we are connected
    if(kinect2.isFrameNew())
    {
        // load grayscale depth image from the kinect source
        grayImage2.setFromPixels(kinect2.getDepthPixels(), kinect2.width, kinect2.height);
        grayImage2.mirror(false,true);
        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        if(bThreshWithOpenCV)
        {
            grayThreshNear = grayImage2;
            grayThreshFar = grayImage2;
            grayThreshNear.threshold(nearThreshold, true);
            grayThreshFar.threshold(farThreshold);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage2.getCvImage(), NULL);
        }
        else
        {
            // or we do it ourselves - show people how they can work with the pixels
            unsigned char * pix = grayImage2.getPixels();

            int numPixels = grayImage2.getWidth() * grayImage2.getHeight();
            for(int i = 0; i < numPixels; i++)
            {
                if(pix[i] < nearThreshold && pix[i] > farThreshold)
                {
                    pix[i] = 255;
                }
                else
                {
                    pix[i] = 0;
                }
            }
        }

        // update the cv images
        grayImage2.flagImageChanged();

        // find contours which are between the size of 10 pixels and 1/2 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        if (tracking) {
           contourFinder2.findContours(grayImage2, 10, (kinect2.width*kinect2.height)/2, 2, false);
        }
    }

    leftEnd[2].set(5000, 500);
    rightEnd[2].set(0, 500);
    leftEnd[3].set(5000, 500);
    rightEnd[3].set(0, 500);

    if(contourFinder2.blobs.size() > 0)
    {
        for (int j=0; j< 2; j++)
        {
            for( int i=0; i<contourFinder2.blobs[j].nPts; i+=10 )//Iteriert durch die Punkte der Kontur, nimmt nur jeden dritten wegen Laufzeit
            {
                if(contourFinder2.blobs[j].pts[i].x/2 < leftEnd[j+2].x)
                {
                    //Wenn aktueller Punkt weiter links als gespeicherter Punkt wird dieser neu gespeichert
                    //x-Wert durch 2 geteilt, da nur auf linker Bidlschirmhälfte
                    leftEnd[j+2].set(contourFinder2.blobs[j].pts[i].x/2, contourFinder2.blobs[j].pts[i].y);
                }

                if(contourFinder2.blobs[j].pts[i].x/2 > rightEnd[j+2].x)
                {
                    //Wenn aktueller Punkt weiter rechts als gespeicherter Punkt wird dieser neu gespeichert
                    rightEnd[j+2].set(contourFinder2.blobs[j].pts[i].x/2, contourFinder2.blobs[j].pts[i].y);
                }
            }
        }
    }

#endif


//-----------------------------------VÖGEL-----------------------------------------------------------

    timeCur = ofGetElapsedTimeMillis();
    ofPoint position = ofPoint(0,0);

    for (int i=0; i<nChef; i++)
    {
        // Wenn ein Körper von der Kinect getrackt wird
        if(contourFinder.blobs.size() > 0)
        {
            attraktoren[0] = rightEnd[0];
            attraktoren[1] = leftEnd[0];
            attraktoren[2] = rightEnd[1];
            attraktoren[3] = leftEnd[1];
            attraktoren[4] = rightEnd[2];
            attraktoren[5] = leftEnd[2];
            attraktoren[6] = rightEnd[3];
            attraktoren[7] = leftEnd[3];

            for(int j=0; j<4; j++)
            {
                attraktoren[j].x = (attraktoren[j].x/kinect.width + adjustmentX/ofGetWidth()) * contourScaleWidth/ofGetWidth();
                attraktoren[j].y = (attraktoren[j].y/kinect.height + adjustmentY/ofGetHeight()) * contourScaleHeight/ofGetHeight();
            }

            for(int j=4; j<8; j++)
            {
                attraktoren[j].x = (attraktoren[j].x/kinect.width + adjustment2X/ofGetWidth()) * contourScaleWidth/ofGetWidth() + 0.5;
                attraktoren[j].y = (attraktoren[j].y/kinect.height + adjustment2Y/ofGetHeight()) * contourScaleHeight/ofGetHeight();
            }

            //cout << "contourscalewidth: " << contourScaleWidth << "\n";

            theChef[i]->update(timeCur-timeOld, attraktoren[i], osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12], osc.getSettings()[5]);

        }
        else
        {
            if( !( ofGetSeconds()%3 ) )  // Alle 3 Sekunden
            {
                // Zufälliger Position folgen.
                position = ofPoint( ofRandom(1), ofRandom(1) ); // Bei schnellen Prozessoren pendeln die Kugeln sich in der Mitte aus. Hier müsste ein Timer eingebaut werden, damit die Chefs erstmal eine Zeit lang in eine Richtung fliegen.
            }
            else  // Ansonnsten
            {
                // Dem letzten Punkt folgen.
                position = ofPoint(-1, -1);
            }
            theChef[i]->update(timeCur-timeOld, position, osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12],osc.getSettings()[5]);
        }
    }

    for (int i=0; i<nVerfolger; i++)
    {trace.draw(0, 0);

        // Die Verfolger werden nacheinander den n Chefs zugeordnet.
        theVerfolger[i]->update(timeCur-timeOld, theChef[i%nChef]->getPos(), osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12], osc.getSettings()[5]);
    }

    timeOld = timeCur;

    createVerfolger = osc.settings[4];

    //cout << createVerfolger << "\n" ;

    if(createVerfolger == 1)
    {
        theVerfolger[nVerfolger] = new Verfolger(ofPoint(osc.getSettings()[3]/*startX,startY*/), 0.0001/*speed*/, 70/*texturWidth*/, 40/*texturHeight*/, ofGetWidth()/*rangeWidth*/, osc.getSettings()[5]);
        nVerfolger++;

        createVerfolger = 0;
    }

    ofEnableAlphaBlending();

    trace.begin();
        drawContours();
    trace.end();

    //test = fencePoints[0] + ((fencePoints[1] - fencePoints[0]) * (ofGetElapsedTimeMillis()/5000));

    //fence[0].clear();
    //fence[0].curveTo(ofGetScreenWidth() + 100, 350);
    //fence[0].curveTo(ofGetScreenWidth(), 350);
    //fence[0].curveTo(test);
    //fence[0].curveTo(0, 0);

    if(setzen)
    {
        for(int i=0; i<nChef; i++)
        {
            theChef[i]->update(timeCur-timeOld, ofPoint(zaun2[i*20].x/ofGetWidth(), zaun2[i*20].y/ofGetHeight()), osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12], osc.getSettings()[5]);
        }
    }

}

//--------------------------------------------------------------


void testApp::drawContours()
{
    ofFill();
	ofSetColor(0,0,0,30);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());

    ofPushStyle();

	// ---------------------------- draw the blobs
	//ofSetColor(0x00FFFF);
	ofFill();
	ofSetColor(255, 255, 255);

	for( int i=0; i<(int)contourFinder.blobs.size(); i++ )
    {

		contours.push_back(contourFinder.blobs[i].pts);
		contours[i].clear();

		for(int j=0; j<contourFinder.blobs[i].nPts; j++)
        {
            contours[i].addVertex((contourFinder.blobs[i].pts[j].x*ofGetWidth()/640/2 + adjustmentX) * contourScaleWidth/ofGetWidth(), (contourFinder.blobs[i].pts[j].y*ofGetHeight()/480 + adjustmentY) * contourScaleHeight/ofGetHeight());
        }

		contours[i].draw();
	}
	contours.clear();

	for( int i=0; i<(int)contourFinder2.blobs.size(); i++ )
    {

		contours.push_back(contourFinder2.blobs[i].pts);
		contours[i].clear();

		for(int j=0; j<contourFinder2.blobs[i].nPts; j++)
        {
            contours[i].addVertex((contourFinder2.blobs[i].pts[j].x*ofGetWidth()/640/2 + adjustment2X) * contourScaleWidth/ofGetWidth() + ofGetWidth()/2, (contourFinder2.blobs[i].pts[j].y*ofGetHeight()/480 + adjustment2Y) * contourScaleHeight/ofGetHeight());
        }

		//contours[i].getSmoothed(10, 1);
		contours[i].draw();

	}
	contours.clear();

	ofPopStyle();
}

void testApp::draw()
{
        //ofNoFill();

    ofSetColor(120);

    /*ofBeginShape();

    ofNoFill();

    ofSetLineWidth(3);

    for(int i = 0; i<fence.size(); i++)
    {
        fence[i].draw();
    }

    ofEndShape();*/

    //curve.draw();
    if(linien)
    {
        ofSetLineWidth(3);

        curve2.draw();
        curve3.draw();
        curve4.draw();
    }

    if(setzen)
    {
        for(int i=0; i<nChef; i++)
            {
                ofSetHexColor(0x00FF00);
                ofCircle(zaun2[i*20].x, zaun2[i*20].y, 5);
            }
    }

//----------------------------------TRACKING----------------------------------------------


    ofSetColor(255, 255, 255);

    //grayImage.draw(0, 0, ofGetWidth()/2, ofGetHeight());

    //Wenn Tracking aktiviert ist wird die Kontur gezeichnet
    trace.draw(0, 0);


    if(tracking) {
       //contourFinder.draw();
       //drawContours();
       trace.draw(0, 0);

       //Wenn enddraw = true werden die Attraktoren als rote Punkte dargestellt
       if(enddraw){
          if(contourFinder.blobs.size() > 0)    //wenn mindestens ein Körper erkannt wird
          {
               //zeichnet 4 Punkte an äußersten Punkten der beiden erkannten Körper
              ofSetHexColor(0xFF0000);
              ofCircle(attraktoren[0].x*ofGetWidth(), attraktoren[0].y*ofGetHeight(), 7);
              ofCircle(attraktoren[1].x*ofGetWidth(), attraktoren[1].y*ofGetHeight(), 7);
              ofCircle(attraktoren[2].x*ofGetWidth(), attraktoren[2].y*ofGetHeight(), 7);
              ofCircle(attraktoren[3].x*ofGetWidth(), attraktoren[3].y*ofGetHeight(), 7);

          }
        }
    }

#ifdef USE_TWO_KINECTS

    ofSetHexColor(0xFFFFFF);

    //grayImage2.draw(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());

    if (tracking) {
       //contourFinder2.setAnchorPoint(-adjustment2X, -adjustment2Y);
       //contourFinder2.draw(0, 0, contourScaleWidth, contourScaleHeight);
       //cout << adjustment2X << " - ";

       if(enddraw){
          if(contourFinder2.blobs.size() > 0)    //wenn ein Körper erkannt wird
          {
              //zeichnet 4 Punkte an äußersten Punkten der beiden erkannten Körper
              ofSetHexColor(0xFF0000);
              ofCircle(attraktoren[4].x*ofGetWidth(), attraktoren[4].y*ofGetHeight(), 7);
              ofCircle(attraktoren[5].x*ofGetWidth(), attraktoren[5].y*ofGetHeight(), 7);
              ofCircle(attraktoren[6].x*ofGetWidth(), attraktoren[6].y*ofGetHeight(), 7);
              ofCircle(attraktoren[7].x*ofGetWidth(), attraktoren[7].y*ofGetHeight(), 7);
          }
        }
    }

#endif


//----------------------------VÖGEL--------------------------------------------------------

    //Bindet die Textur auf die Festplatte
    vogelTextur.getTextureReference().bind();

    //Zeichnet alle Chefs
    for (int i=0; i<nChef; i++)
    {
        theChef[i]->draw();
    }

    //Zeichnet alle Verfolger
    for (int i=0; i<nVerfolger; i++)
    {
        theVerfolger[i]->draw();
    }

    vogelTextur.getTextureReference().unbind();

    // Gibt Framerate in linker oberer Ecke aus
    ofDrawBitmapString(ofToString(ofGetFrameRate()),10,10);

    //Linie, um Fluggrenze rechts zu zeigen
    //ofSetColor(255);
    //ofRect(blubb, 0, 5, ofGetHeight());

    // draw instructions
    /*ofSetColor(255);
    stringstream reportStream;

    if(kinect.hasAccelControl())
    {
        reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
                     << ofToString(kinect.getMksAccel().y, 2) << " / "
                     << ofToString(kinect.getMksAccel().z, 2) << endl;
    }
    else
    {
        reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
                     << "motor / led / accel controls are not currently supported" << endl << endl;
    }

    reportStream << "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
                 << "set near threshold " << nearThreshold << " (press: + -)" << endl
                 << "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
                 << ", fps: " << ofGetFrameRate() << endl
                 << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;


    if(kinect.hasCamTiltControl())
    {
        reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
                     << "press 1-5 & 0 to change the led mode" << endl;
    }

    ofDrawBitmapString(reportStream.str(), 20, 652);*/
}


//--------------------------------------------------------------
void testApp::exit()
{
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();

#ifdef USE_TWO_KINECTS
    kinect2.setCameraTiltAngle(0);
    kinect2.close();
#endif
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    //Bei Linksklick wird Tracking de/aktiviert
    if(!button){
        tracking = !tracking;
        cout << "tracking" << ofToString(tracking);
    }

    //Bei Rechtsklick wird zeichnen der Attraktorpunkte de/aktiviert
    if(button){
        enddraw = !enddraw;
        cout << "balldraw" << ofToString(enddraw);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

    switch (key)
    {

    case 'l' :
        linien = !linien;
        break;

    case 's' :
        setzen = !setzen;
        break;


    case 'f' :
        //Fullcreen
        ofToggleFullscreen();
        break;

//----------------------------------VÖGEL------------------------------------------------

    case 'v':

        //neuer verfolger wird erstellt
        theVerfolger[nVerfolger] = new Verfolger(ofPoint(startX, startY), 0.00015/*NEW*/, texturWidth/*NEW*/, texturHeight/*NEW*/, ofGetWidth()/*rangeWidth*/, grauwert/*new*/);
        nVerfolger++;
        break;

    case 'b':

        //neuer chef wird erstellt
        theChef[nChef] = new Chef(ofPoint(startX, startY), 0.00015/*NEW*/, texturWidth/*NEW*/, texturHeight/*NEW*/, ofGetWidth()/*rangeWidth*/, grauwert/*new*/);
        nChef++;
        break;


//-----------------------------TRACKING-------------------------------------------

    case ' ':
        bThreshWithOpenCV = !bThreshWithOpenCV;
        break;

    case '>':
    case '.':
        //Schwellwert für ferne Bereiche erhöhen
        farThreshold ++;
        if (farThreshold > 255) farThreshold = 255;
        break;

    case '<':
    case ',':
        //Schwellwert für ferne Bereiche verringern
        farThreshold --;
        if (farThreshold < 0) farThreshold = 0;
        break;

    case '+':
    case '=':
        //Schwellwert für nahe Bereiche erhöhen
        nearThreshold ++;
        if (nearThreshold > 255) nearThreshold = 255;
        break;

    case '-':
        //Schwellwert für nahe Bereiche verringern
        nearThreshold --;
        if (nearThreshold < 0) nearThreshold = 0;
        break;

    case 'o':
        //Kinect wieder einschalten
        kinect.setCameraTiltAngle(angle); // go back to prev tilt
        kinect.open();
        break;

    case 'c':
        //Kinect ausschalten
        kinect.setCameraTiltAngle(0); // zero the tilt
        kinect.close();
        break;

    case OF_KEY_UP:
        //Kinect nach oben ausrichten, bis max. 30 Grad
        angle++;
        if(angle>30) angle=30;
        kinect.setCameraTiltAngle(angle);
        kinect2.setCameraTiltAngle(angle);
        break;

    case OF_KEY_DOWN:
        //Kinect nach unten ausrichten, bis max. 30 Grad
        angle--;
        if(angle<-30) angle=-30;
        kinect.setCameraTiltAngle(angle);
        kinect2.setCameraTiltAngle(angle);
        break;


    default:
        break;
    }
}
