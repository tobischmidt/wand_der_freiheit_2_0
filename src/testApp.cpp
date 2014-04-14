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
    //background.loadImage("background_neu.jpg");

    nVerfolger = 8;

    nChef = 8;

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

    contourScaleWidth = ofGetScreenWidth()/2;    /*NEW*/
    contourScaleHeight = ofGetScreenHeight();  /*NEW*/

    trace.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGBA32F_ARB);

    trace.begin();
    ofClear(255,255,255, 0);
    trace.end();

    //--------------------------ABSCHLUSS-----------------------------------------------------

    counter = 0;

    vector<ofVec2f> vec;
    vec.push_back(ofVec2f(0, 0));

    for(int i= 0; i<3; i++)
    {
        curveDefine.push_back(vec);
    }
    for(int i=0; i<curveDefine.size(); i++)
    {
        curveDefine[i].clear();
    }

    for(float i=0; i<200; i++)
    {
        if(i <= 54)
        {
            curveDefine[0].push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/15) * 50 + 600));
        }
        else if(i > 54 && i <=  122)
        {
            curveDefine[0].push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/40 + M_PI*538/1000) * 140 + 564));
        }
        else
        {
            curveDefine[0].push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/40 + M_PI*538/1000) * 70 + 494));
        }
    }

    for(float i=0; i<200; i++)
    {
        if(i <= 54)
        {
            curveDefine[1].push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/15 + M_PI) * 120 + 400));
        }
        else if(i > 54 && i <= 95)
        {
            curveDefine[1].push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/20 + M_PI) * 170 + 526));
        }
        else
        {
            curveDefine[1].push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/40 + M_PI*1738/1000) * 30 + 666));
        }
    }

    for(float i=0; i<200; i++)
    {
        if(i <= 100)
        {
            curveDefine[2].push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/30 + M_PI*3/4) * 100 + 430));
        }
        else if(i > 100 && i <= 150)
        {
            curveDefine[2].push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/20 + M_PI*5/100) * 125 + 487));
        }
        else
        {
            curveDefine[2].push_back(ofVec2f(ofGetScreenWidth() - (i*10), sin(i/40 + M_PI*1738/1000 + M_PI*3/2) * 100 + 512));
        }
    }

    setzen = false;
    linien = false;
    createVerfolger = false;

    runCounter = -1;

    line.addVertex(ofPoint(vec[0]));
    line.clear();
}

//--------------------------------------------------------------

void testApp::update()
{
    //Hintergrundfarbe schwarz
    ofBackground(0);

    runCounter++;
    if(runCounter > 150)
    {
        runCounter = 0;
    }

//-------------------------------------------------------OSC----------------------------------------------

    osc.listen();/*NEW*/

    if(osc.settings[8] == 1)
    {
        tracking = true;
    }
//    else
//    {
//        tracking = false;
//    }
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
        adjustment2X = osc.settings[18] * ofGetScreenWidth(); /*NEW*/
    }
    else
    {
        adjustment2X = 0;
    }

    if(osc.settings[21] != 0)
    {
        adjustment2Y = osc.settings[21] * ofGetScreenHeight(); /*NEW*/
    }
    else
    {
        adjustment2Y = 0;
    }

    if(osc.settings[6] != 0)
    {
        contourScaleWidth = osc.settings[6] * ofGetScreenWidth();    /*NEW*/
    }
    else
    {
        contourScaleWidth = ofGetWidth();
    }

    if(osc.settings[17] != 0)
    {
        contourScaleHeight = osc.settings[17] * ofGetScreenHeight();  /*NEW*/
    }
    else
    {
        contourScaleHeight = ofGetHeight();
    }

    if(osc.settings[12] != 0)
    {
        blubb = ofGetWidth() - (osc.settings[12] * ofGetScreenWidth());  /*NEW*/
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
        grayImage.flagImageChanged();

        // Wenn Tracking aktiviert wird
        // find contours which are between the size of 10 pixels and 1/2 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        if (tracking)
        {
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
        if (tracking)
        {
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

            theChef[i]->update(timeCur-timeOld, attraktoren[i], osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12], osc.getSettings()[5]);

        }
        else
        {
            //if( !( (int)ofGetElapsedTimeMicros()%2000000 ) )  // Alle 3 Sekunden
            if(!runCounter) // Alle 150 Durchläufe
            {
                // Zufälliger Position folgen.
                position = ofPoint(ofRandom(1), ofRandom(1)); // Bei schnellen Prozessoren pendeln die Kugeln sich in der Mitte aus. Hier müsste ein Timer eingebaut werden, damit die Chefs erstmal eine Zeit lang in eine Richtung fliegen.
                //cout << "sers - ";
            }
            else  // Ansonsten
            {
                // Dem letzten Punkt folgen.
                position = ofPoint(-1, -1);
            }
            theChef[i]->update(timeCur-timeOld, position, osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12],osc.getSettings()[5]);
        }
    }


    if(!setzen)
    {
        for (int i=0; i<nVerfolger; i++)
        {
            // Die Verfolger werden nacheinander den n Chefs zugeordnet.
            theVerfolger[i]->update(timeCur-timeOld, theChef[i%nChef]->getPos(), osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12], osc.getSettings()[5]);
        }
    }
    else
    {
        for (int i=0; i<nVerfolger; i++)
        {
            // Die Verfolger werden nacheinander den n Chefs zugeordnet.
            theVerfolger[i]->update(timeCur-timeOld, ofPoint(-1, -1), osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12], osc.getSettings()[5]);
        }
    }


    timeOld = timeCur;

    if(osc.settings[4] == 1 || osc.settings[19] == 1)
    {
        createVerfolger = true;
        osc.settings[4] = 0;
        osc.settings[19] = 0;
    }

    if(createVerfolger)
    {
        //theVerfolger[nVerfolger] = new Verfolger(ofPoint(osc.getSettings()[3], osc.getSettings()[15]/*startX,startY*/), 0.0001/*speed*/, osc.getSettings()[1]/*texturWidth*/, osc.getSettings()[16]/*texturHeight*/, ofGetWidth()/*rangeWidth*/, osc.getSettings()[5]);
        theVerfolger[nVerfolger] = new Verfolger(ofPoint(ofRandom(1), ofRandom(1)), 0.00015/*NEW*/, 35/*NEW*/, 20/*NEW*/, ofGetWidth()/*rangeWidth*/, grauwert/*new*/);

        nVerfolger++;

        createVerfolger = false;
    }

    ofEnableAlphaBlending();

    trace.begin();
    drawContours();
    trace.end();

//-------------------------------------------Abschluss--------------------------------------------------------------------

    if(linien)
    {
        counter++;

        for(int i=0; i<3; i++)
        {
            curve.push_back(line);
        }

        if((counter%2) == 0 && counter/2 < curveDefine[0].size())
        {
            //curve[0].curveTo(ofPoint(curveDefine[0].at(counter/2).x, curveDefine[0].at(counter/2).y));
            curve[0].curveTo(ofPoint(curveDefine[0].at(counter/2)));
        }

        if((counter%2) == 0 && counter/2 < curveDefine[1].size() + 20 && counter > 20)
        {
            //curve[1].curveTo(ofPoint(curveDefine[1].at((counter-20)/2).x, curveDefine[1].at((counter-20)/2).y));
            curve[1].curveTo(ofPoint(curveDefine[1].at((counter - 20)/2)));
        }

        if((counter%2) == 0 && counter/2 < curveDefine[2].size() + 40 && counter > 40)
        {
            //curve[2].curveTo(ofPoint(curveDefine[2].at((counter-40)/2).x, curveDefine[2].at((counter-40)/2).y));
            curve[2].curveTo(ofPoint(curveDefine[2].at((counter - 40)/2)));
        }
    }

    if(setzen)
    {
        for(int i=0; i<nVerfolger; i++)
        {
            //theChef[i]->update(timeCur-timeOld, ofPoint(zaun2[i*20].x/ofGetWidth(), zaun2[i*20].y/ofGetHeight()), osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12], osc.getSettings()[5]);
            theVerfolger[i]->update(timeCur-timeOld, ofPoint(curveDefine[0].at(i*20).x/ofGetWidth(), curveDefine[0].at(i*20).y/ofGetHeight()), osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12], osc.getSettings()[5]);
        }
    }
}

//--------------------------------------------------------------


void testApp::drawContours()
{
    ofFill();
    ofSetColor(0,0,0,30);
    ofRect(0, 0, ofGetScreenWidth(), ofGetScreenHeight());

    ofPushStyle();
    ofSetLineWidth(3);

    // ---------------------------- draw the contours

    ofFill();
    ofSetColor(255, 255, 255);
    //grayImage.draw(adjustmentX, adjustmentY, contourScaleWidth, contourScaleHeight);
    //grayImage2.draw(adjustment2X, adjustment2Y, contourScaleWidth, contourScaleHeight);
    //grayImage.draw(0, 0, ofGetScreenWidth()/2, ofGetScreenHeight());
    //grayImage2.draw(ofGetScreenWidth()/2, 0, ofGetScreenWidth()/2, ofGetScreenHeight());

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

        contours[i].draw();

    }
    contours.clear();

    ofPopStyle();
}

void testApp::draw()
{
    //ofSetColor(255);
    //background.draw(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
    ofSetColor(120);

    if(linien)
    {
        ofPushStyle();
        ofSetLineWidth(3);

        for(int i=0; i<curve.size(); i++)
        {
            curve[i].draw();
        }

        ofPopStyle();
    }

    if(setzen)
    {
        for(int i=0; i<nVerfolger; i++)
        {
            ofSetHexColor(0x00FF00);
            ofCircle(curveDefine[0].at(i*20).x, curveDefine[0].at(i*20).y, 5);
        }
    }

//----------------------------------TRACKING----------------------------------------------


    ofSetColor(255, 255, 255);

    //grayImage.draw(0, 0, ofGetWidth()/2, ofGetHeight());

    if(tracking)
    {
        //Wenn Tracking aktiviert ist wird die Kontur gezeichnet
        trace.draw(0, 0);

        //Wenn enddraw = true werden die Attraktoren als rote Punkte dargestellt
        if(enddraw)
        {
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

    if (tracking)
    {
        if(enddraw)
        {
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
    if(!button)
    {
        tracking = !tracking;
        cout << "tracking" << ofToString(tracking);
    }

    //Bei Rechtsklick wird zeichnen der Attraktorpunkte de/aktiviert
    if(button)
    {
        enddraw = !enddraw;
        cout << "balldraw" << ofToString(enddraw);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

    switch (key)
    {

    case 'f' :
        //Fullcreen
        ofToggleFullscreen();
        break;

//----------------------------------VÖGEL------------------------------------------------

    case 'l' :

        linien = !linien;
        break;

    case 's' :

        setzen = !setzen;
        break;

    case 'v':

        //neuer verfolger wird erstellt
        theVerfolger[nVerfolger] = new Verfolger(ofPoint(ofRandom(1), ofRandom(1)), 0.00015/*NEW*/, 35/*NEW*/, 20/*NEW*/, ofGetWidth()/*rangeWidth*/, grauwert/*new*/);
        nVerfolger++;
        cout << "Verfolger \n";
        break;

    case 'b':

        //neuer chef wird erstellt
        theChef[nChef] = new Chef(ofPoint(ofRandom(1), ofRandom(1)), 0.00015/*NEW*/, 35/*NEW*/, 20/*NEW*/, ofGetWidth()/*rangeWidth*/, grauwert/*new*/);
        nChef++;
        cout << "Chef \n";
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
