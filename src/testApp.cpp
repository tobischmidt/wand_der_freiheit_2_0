#include "testApp.h"
#include "chef.h"
#include "verfolger.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

//--------------------------------------------------------------

void testApp::setup()
{
    ofSetWindowTitle("Wand der Freiheit");

    background.loadImage("left_wall_2.jpg");
    zitat.loadImage("Zitat3.png");

    timeOld = ofGetElapsedTimeMillis();
    timeCur = timeOld;

    ofSetFrameRate(60);

    //Fenstergröße
    windowWidth = ofGetScreenWidth();
    windowHeight = ofGetScreenHeight();

    ofSetVerticalSync(false);


//-------------------------TRACKING-----------------------------------------------


    //Tracking & Zeichnen der Endpunkte zunächst ausgeschalten
    tracking = false;
    enddraw = false;
    hasContours = false;

    ofSetLogLevel(OF_LOG_VERBOSE);

    angle = 0;


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



    // set the tilt to 0 on startup

    kinect.setCameraTiltAngle(angle);


#ifdef USE_TWO_KINECTS

    kinect2.init(false, false);
    kinect2.open();

    grayImage2.allocate(kinect2.width, kinect2.height);

    kinect2.setCameraTiltAngle(angle);

#endif

    //Fbo für Spur der Silhouette
    trace.allocate(windowWidth, windowHeight, GL_RGBA32F_ARB);

    trace.begin();
    ofClear(255,255,255, 0);
    trace.end();

    leftEnd[0].set(2000, 500);
    rightEnd[0].set(0, 500);
    leftEnd[1].set(2000, 500);
    rightEnd[1].set(0, 500);
    leftEnd[2].set(2000, 500);
    rightEnd[2].set(0, 500);
    leftEnd[3].set(2000, 500);
    rightEnd[3].set(0, 500);


//-------------------------OSC Variablen-----------------------------------

    doOscUpdate = false;

    osc.setup(); /*NEW*/

    speed = 0.00007;
    par1 = 0.4;
    texturWidth = 30;
    texturHeight = 16;
    grauwert = 255;
    startX = 0.9;
    startY = 0.3;

    adjustmentX = 0;
    adjustmentY = 0;
    adjustment2X = windowWidth/2;
    adjustment2Y = 0;

    contourScaleWidth = windowWidth/2;
    contourScaleHeight = windowHeight;

    rangeWidth = windowWidth;


//---------------------------VÖGEL------------------------------------------


    vogelTextur.loadImage("Vögel_weiß_Var3.png");
    drahtTextur.loadImage("Draht_weiß_neu.png");

    nVerfolger = 12;
    nChef = 4;

    theChef = new Chef*[nChef];
    theVerfolger = new Verfolger*[nVerfolger];

    //nChef Chefs werden vordefiniert
    for (int i = 0; i < nChef; i++)
    {
        theChef[i] = new Chef(ofPoint(ofRandom(1), ofRandom(1)), texturWidth, texturHeight, rangeWidth);
        theChef[i]->setSpeed(speed);
        theChef[i]->setPar1(par1);
    }


    //nVerfolger Verfolger wird vordefiniert
    for (int i = 0; i < nVerfolger; i++)
    {
        theVerfolger[i] = new Verfolger(ofPoint(ofRandom(1), ofRandom(1)), texturWidth, texturHeight, rangeWidth);
        theVerfolger[i]->setSpeed(speed);
        theVerfolger[i]->setPar1(par1);
    }


//--------------------------ABSCHLUSS-----------------------------------------------------

    counter = 0;

    vec.push_back(ofVec2f(0, 0));

    for(int i= 0; i<3; i++)
    {
        curveDefine.push_back(vec);
    }
    for(int i=0; i<curveDefine.size(); i++)
    {
        curveDefine[i].clear();
    }

    // 4 Linien vordefiniert
    for(float i=0; i<220; i++)
    {
        if(i <= 70)
        {
            curveDefine[0].push_back(ofVec2f(windowWidth - (i*10), sin(i/15) * 50 + 400));
        }
        else
        {
            curveDefine[0].push_back(ofVec2f(windowWidth - (i*10), 350));
        }
    }

    for(float i=0; i<220; i++)
    {
        if(i <= 63)
        {
            curveDefine[1].push_back(ofVec2f(windowWidth - (i*10), sin(i/20 + M_PI*3/2) * 125 + 470));
        }
        else
        {
            curveDefine[1].push_back(ofVec2f(windowWidth - (i*10), 595));
        }
    }

    for(float i=0; i<220; i++)
    {
        if(i <= 27)
        {
            curveDefine[2].push_back(ofVec2f(windowWidth - (i*10), sin(i/10 + M_PI*3200/2000) * 150 + 500));
        }
        else if(i > 27 && i <= 80)
        {
            curveDefine[2].push_back(ofVec2f(windowWidth - (i*10), sin(i/15 + M_PI*3620/2000) * 110 + 546));
        }
        else
        {
            curveDefine[2].push_back(ofVec2f(windowWidth - (i*10), 436));
        }
    }


    setzen = false;
    linien = false;
    createVerfolger = false;
    transformation = false;

    runCounter = -1;
    endCounter = 0;
    zitatCounter = 0;

    line.addVertex(ofPoint(vec[0]));
    line.clear();

    for(int i=0; i<3; i++)
    {
        curve.push_back(line);
    }

    //Hintergrundfarbe schwarz
    ofBackground(0);

    ofSleepMillis(2000);
}

//--------------------------------------------------------------

void testApp::update()
{
    //Zähler, der alle 150 Durchläufe zurückgesetzt wird.
    runCounter++;
    if(runCounter > 150)
    {
        runCounter = 0;
    }

//-------------------------------------------------------OSC----------------------------------------------

    osc.listen();/*NEW*/

    for(int i=0; i<23; i++)
    {
        if(osc.settingsUpdate[i])
        {
            doOscUpdate = true;
            break;
        }
    }

    if(doOscUpdate)
    {
        updateOsc();
        doOscUpdate = false;
    }

//----------------------------------TRACKING------------------------------------------------------------

    if(kinect.isConnected())
    {
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

            grayThreshNear = grayImage;
            grayThreshFar = grayImage;
            grayThreshNear.threshold(nearThreshold, true);
            grayThreshFar.threshold(farThreshold);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);

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
    }

//selbe Funktionen wie für die erste Kinect, nur auf rechte Bildschirmhälfte bezogen
#ifdef USE_TWO_KINECTS

    if(kinect2.isConnected())
    {
        kinect2.update();

        // there is a new frame and we are connected
        if(kinect2.isFrameNew())
        {
            // load grayscale depth image from the kinect source
            grayImage2.setFromPixels(kinect2.getDepthPixels(), kinect2.width, kinect2.height);
            grayImage2.mirror(false,true);
            // we do two thresholds - one for the far plane and one for the near plane
            // we then do a cvAnd to get the pixels which are a union of the two thresholds

            grayThreshNear = grayImage2;
            grayThreshFar = grayImage2;
            grayThreshNear.threshold(nearThreshold, true);
            grayThreshFar.threshold(farThreshold);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage2.getCvImage(), NULL);


            // update the cv images
            grayImage2.flagImageChanged();

            // find contours which are between the size of 10 pixels and 1/2 the w*h pixels.
            // also, find holes is set to true so we will get interior contours as well....
            if (tracking)
            {
                contourFinder2.findContours(grayImage2, 10, (kinect2.width*kinect2.height)/2, 2, false);
            }
        }

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
    }

#endif


//-------------------------------------------Abschluss--------------------------------------------------------------------

    if(linien)
    {
        //Über die Laufzeit den Polylines die Punkte übergeben, sodass sie "ins Bild laufen"
        if((counter%2) == 0 && counter/2 < curveDefine[0].size())
        {
            curve[0].curveTo(ofPoint(curveDefine[0].at((counter)/2)));
        }

        if((counter%2) == 0 && counter/2 < curveDefine[1].size() && counter > 20)
        {
            curve[1].curveTo(ofPoint(curveDefine[1].at((counter/2) - 10)));
        }

        if((counter%2) == 0 && counter/2 < curveDefine[2].size() && counter > 40)
        {
            curve[2].curveTo(ofPoint(curveDefine[2].at((counter/2) - 20)));
        }

        counter++;
    }


//-----------------------------------VÖGEL-----------------------------------------------------------

    //alle 150 Durchläufe
    if(!runCounter)
    {
        for(int i=0; i<nVerfolger; i++)
        {
            theVerfolger[i]->setSpeed(ofRandom(0.00005, 0.00008));
            theVerfolger[i]->newAbweichung();
        }
    }

    timeCur = ofGetElapsedTimeMillis();
    timeDiff = timeCur - timeOld;
    position.set(0,0);

    hasContours = contourFinder.blobs.size();

    for (int i=0; i<nChef; i++)
    {
        // Wenn ein Körper von der Kinect getrackt wird
        if(!setzen)
        {
            if(hasContours)
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
                    attraktoren[j].x = (attraktoren[j].x/kinect.width + adjustmentX/windowWidth) * contourScaleWidth/windowWidth;
                    attraktoren[j].y = (attraktoren[j].y/kinect.height + adjustmentY/windowHeight) * contourScaleHeight/windowHeight;
                }

                for(int j=4; j<8; j++)
                {
                    attraktoren[j].x = (attraktoren[j].x/kinect.width + adjustment2X/windowWidth) * contourScaleWidth/windowWidth + 0.5;
                    attraktoren[j].y = (attraktoren[j].y/kinect.height + adjustment2Y/windowHeight) * contourScaleHeight/windowHeight;
                }

                theChef[i]->update(timeDiff, attraktoren[i]);

            }
            else
            {
                if(!runCounter && !setzen) // Alle 150 Durchläufe
                {
                    // Zufälliger Position folgen.
                    position.set(ofRandom(1), ofRandom(1)); // Bei schnellen Prozessoren pendeln die Kugeln sich in der Mitte aus. Hier müsste ein Timer eingebaut werden, damit die Chefs erstmal eine Zeit lang in eine Richtung fliegen.
                }
                else  // Ansonsten
                {
                    // Dem letzten Punkt folgen.
                    position.set(-1, -1);
                }
                theChef[i]->update(timeDiff, position);
            }
        }

        else
        {
            position.set(0.5, -0.3);
            theChef[i]->update(timeDiff, position);
        }
    }

    if(!setzen)
    {
        for (int i=0; i<nVerfolger; i++)
        {
            // Die Verfolger werden nacheinander den n Chefs zugeordnet.
            theVerfolger[i]->update(timeDiff, theChef[i%nChef]->getPos());
        }
    }
    else
    {
        endCounter++;
        if(endCounter > 600)
        {
            zitatCounter++;
        }

        for(int i=0; i<nVerfolger; i++)
        {
            if(i<12)
            {

                if(sqrt(pow(theVerfolger[i]->getPos().x - curveDefine[0].at(i*10 + 70).x/windowWidth, 2) + pow(theVerfolger[i]->getPos().y - curveDefine[0].at(i*10 + 70).y/windowHeight, 2)) < 0.005)
                {
                    theVerfolger[i]->setPos(ofPoint(curveDefine[0].at(i*10 + 70).x/windowWidth, (curveDefine[0].at(i*10 + 70).y)/windowHeight));
                }
                else
                {
                   theVerfolger[i]->update(timeDiff, ofPoint(curveDefine[0].at(i*10 + 70).x/windowWidth, curveDefine[0].at(i*10 + 70).y/windowHeight));
                }
            }

            else if(i>=12 && i<22)
            {
                if(sqrt(pow(theVerfolger[i]->getPos().x - curveDefine[1].at((i-12)*10 + 90).x/windowWidth, 2) + pow(theVerfolger[i]->getPos().y - curveDefine[1].at((i-12)*10 + 90).y/windowHeight, 2)) < 0.01)
                {
                    theVerfolger[i]->setPos(ofPoint(curveDefine[1].at((i-12)*10 + 90).x/windowWidth, curveDefine[1].at((i-12)*10 + 90).y/windowHeight));
                }
                else
                {
                    theVerfolger[i]->update(timeDiff, ofPoint(curveDefine[1].at((i-12)*10 + 90).x/windowWidth, curveDefine[1].at((i-12)*10 + 90).y/windowHeight));
                }

            }

            else
            {
                if(sqrt(pow(theVerfolger[i]->getPos().x - curveDefine[2].at((i-22)*10 + 90).x/windowWidth, 2) + pow(theVerfolger[i]->getPos().y - curveDefine[2].at((i-22)*10 + 90).y/windowHeight, 2)) < 0.01)
                {
                    theVerfolger[i]->setPos(ofPoint(curveDefine[2].at((i-22)*10 + 90).x/windowWidth, curveDefine[2].at((i-22)*10 + 90).y/windowHeight));
                }
                else
                {
                    theVerfolger[i]->update(timeDiff, ofPoint(curveDefine[2].at((i-22)*10 + 90).x/windowWidth, curveDefine[2].at((i-22)*10 + 90).y/windowHeight));
                }

            }

            theVerfolger[i]->setPar1(1);
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
        theVerfolger[nVerfolger] = new Verfolger(ofPoint(startX, startY), texturWidth, texturHeight, rangeWidth);
        theVerfolger[nVerfolger]->setSpeed(speed);
        theVerfolger[nVerfolger]->setPar1(par1);

        nVerfolger++;

        createVerfolger = false;
    }

    ofEnableAlphaBlending();

    trace.begin();
    drawContours();
    trace.end();
}

//--------------------------------------------------------------

void testApp::updateOsc()
{
    if(osc.settingsUpdate[8] && osc.settings[8] == 1)
    {
        tracking = true;
    }
    //else
    //{
     //   tracking = false;
    //}
    //cout << ofToString(tracking) << "\n" ;


    if(osc.settingsUpdate[3] && osc.settings[3] != startX)
    {
        startX = osc.settings[3];
        osc.settingsUpdate[3] = false;
    }

    if(osc.settingsUpdate[15] && osc.settings[15] != startY)
    {
        startY = osc.settings[15];
        osc.settingsUpdate[15] = false;
    }

    if(osc.settingsUpdate[9] && osc.settings[9] != nearThreshold)
    {
        nearThreshold = osc.settings[9];
        osc.settingsUpdate[9] = false;
    }

    if(osc.settingsUpdate[10] && osc.settings[10] != farThreshold)
    {
        farThreshold = osc.settings[10];
        osc.settingsUpdate[10] = false;
    }

    // verschiebung 1 x
    if(osc.settingsUpdate[7] && osc.settings[7] * windowWidth != adjustmentX)
    {
        adjustmentX = osc.settings[7] * windowWidth;
        osc.settingsUpdate[7] = false;
    }

    // verschiebung 1 y
    if(osc.settingsUpdate[20] && osc.settings[20] * windowHeight != adjustmentY)
    {
        adjustmentY = osc.settings[20] * windowHeight;
        osc.settingsUpdate[20] = false;
    }

    if(osc.settingsUpdate[18] && osc.settings[18] * windowWidth != adjustment2X)
    {
        adjustment2X = osc.settings[18] * windowWidth;
        osc.settingsUpdate[18] = false;
    }

    if(osc.settingsUpdate[21] && osc.settings[21] * windowHeight != adjustment2Y)
    {
        adjustment2Y = osc.settings[21] * windowHeight;
        osc.settingsUpdate[21] = false;
    }

    if(osc.settingsUpdate[6] && osc.settings[6] * windowWidth != contourScaleWidth)
    {
        contourScaleWidth = osc.settings[6] * windowWidth;
        osc.settingsUpdate[6] = false;
    }

    if(osc.settingsUpdate[17] && osc.settings[17] * windowHeight != contourScaleHeight)
    {
        contourScaleHeight = osc.settings[17] * windowHeight;
        osc.settingsUpdate[17] = false;
    }

    if(osc.settingsUpdate[1] && osc.settings[1] * 25 != texturWidth)
    {
        texturWidth = osc.settings[1] * 25;
        osc.settingsUpdate[1] = false;

        for(int i=0; i<nVerfolger; i++)
        {
            theVerfolger[i]->setTexturWidth(texturWidth);
        }
        for(int i=0; i<nChef; i++)
        {
            theChef[i]->setTexturWidth(texturWidth);
        }
    }

    if(osc.settingsUpdate[16] && osc.settings[16] * 12 != texturHeight)
    {
        texturHeight = osc.settings[16] * 12;
        osc.settingsUpdate[16] = false;

        for(int i=0; i<nVerfolger; i++)
        {
            theVerfolger[i]->setTexturHeight(texturHeight);
        }
        for(int i=0; i<nChef; i++)
        {
            theChef[i]->setTexturHeight(texturHeight);
        }
    }

    if(osc.settingsUpdate[2] && osc.settings[2] != par1)
    {
        par1 = osc.settings[2];
        osc.settingsUpdate[2] = false;

        for(int i=0; i<nVerfolger; i++)
        {
            theVerfolger[i]->setPar1(par1);
        }
        for(int i=0; i<nChef; i++)
        {
            theChef[i]->setPar1(par1);
        }
    }

    if(osc.settingsUpdate[12] && osc.settings[12] != 0)
    {
        blubb = windowWidth - (osc.settings[12] * windowWidth);
        osc.settingsUpdate[12] = false;
    }
}


void testApp::drawContours()
{
    ofFill();
    ofSetColor(0,0,0,30);
    ofRect(0, 0, windowWidth, windowHeight);

    ofPushStyle();
    //ofSetLineWidth(3);

    // ---------------------------- draw the contours

    ofFill();
    ofSetColor(255, 255, 255);

    for( int i=0; i<(int)contourFinder.blobs.size(); i++ )
    {

        contours.push_back(contourFinder.blobs[i].pts);
        contours[i].clear();

        for(int j=0; j<contourFinder.blobs[i].nPts; j++)
        {
            contours[i].addVertex((contourFinder.blobs[i].pts[j].x*windowWidth/640/2 + adjustmentX) * contourScaleWidth/windowWidth, (contourFinder.blobs[i].pts[j].y*windowHeight/480 + adjustmentY) * contourScaleHeight/windowHeight);
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
            contours[i].addVertex((contourFinder2.blobs[i].pts[j].x*windowWidth/640/2 + adjustment2X) * contourScaleWidth/windowWidth, (contourFinder2.blobs[i].pts[j].y*windowHeight/480 + adjustment2Y) * contourScaleHeight/windowHeight);
        }

        contours[i].draw();

    }
    contours.clear();

    ofPopStyle();
}

void testApp::draw()
{
    //ofSetColor(255);
    //background.draw(0, 0, windowWidth, windowHeight);
    ofSetColor(120);

    //ofRect(0, windowHeight/2, windowWidth, 3);

    if(linien)
    {
        ofPushStyle();
        ofSetLineWidth(4);

        for(int i=0; i<curve.size(); i++)
        {
            curve[i].draw();
        }

        ofPopStyle();
    }

//----------------------------------TRACKING----------------------------------------------


    ofSetColor(255, 255, 255);

    //grayImage.draw(0, 0, windowWidth, windowHeight);

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
                ofCircle(attraktoren[0].x*windowWidth, attraktoren[0].y*windowHeight, 7);
                ofCircle(attraktoren[1].x*windowWidth, attraktoren[1].y*windowHeight, 7);
                ofCircle(attraktoren[2].x*windowWidth, attraktoren[2].y*windowHeight, 7);
                ofCircle(attraktoren[3].x*windowWidth, attraktoren[3].y*windowHeight, 7);

            }
        }
    }

#ifdef USE_TWO_KINECTS

    ofSetHexColor(0xFFFFFF);

    //grayImage2.draw(windowWidth/2, 0, windowWidth/2, windowHeight);

    if (tracking)
    {
        if(enddraw)
        {
            if(contourFinder2.blobs.size() > 0)    //wenn ein Körper erkannt wird
            {
                //zeichnet 4 Punkte an äußersten Punkten der beiden erkannten Körper
                ofSetHexColor(0xFF0000);
                ofCircle(attraktoren[4].x*windowWidth, attraktoren[4].y*windowHeight, 7);
                ofCircle(attraktoren[5].x*windowWidth, attraktoren[5].y*windowHeight, 7);
                ofCircle(attraktoren[6].x*windowWidth, attraktoren[6].y*windowHeight, 7);
                ofCircle(attraktoren[7].x*windowWidth, attraktoren[7].y*windowHeight, 7);
            }
        }
    }

#endif


//----------------------------VÖGEL--------------------------------------------------------

    //Bindet die Textur auf die Festplatte
    //ofSetColor(255);

    vogelTextur.getTextureReference().bind();

    //Zeichnet alle Chefs
    for (int i=0; i<nChef; i++)
    {
        theChef[i]->draw();
    }

    vogelTextur.getTextureReference().unbind();

    if(setzen && endCounter > 600)
    {
        drahtTextur.getTextureReference().bind();

        //Zeichnet alle Verfolger
        for (int i=0; i<nVerfolger; i++)
        {
            theVerfolger[i]->drawEnd(endCounter);
        }

        drahtTextur.getTextureReference().unbind();
    }

    else
    {
        vogelTextur.getTextureReference().bind();

        //Zeichnet alle Verfolger
        for (int i=0; i<nVerfolger; i++)
        {
            theVerfolger[i]->draw();
        }

        vogelTextur.getTextureReference().unbind();
    }


    // Gibt Framerate in linker oberer Ecke aus
    ofDrawBitmapString(ofToString(round(ofGetFrameRate())),10,10);

    if(setzen)
    {
        ofSetColor(120, 120, 120, zitatCounter);
        zitat.draw(300, 800, 800, 200);
    }

    //Linie, um Fluggrenze rechts zu zeigen
    //ofSetColor(255);
    //ofRect(blubb, 0, 5, windowHeight);

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
        windowWidth = ofGetWidth();
        windowHeight = ofGetHeight();
        break;

//----------------------------------VÖGEL------------------------------------------------

    case 'l' :

        for(int i=0; i<curve.size(); i++)
        {
            curve[i].clear();
        }

        counter = 0;

        linien = !linien;
        break;

    case 's' :

        endCounter = 0;
        zitatCounter = 0;

        setzen = !setzen;

        if(!setzen)
        {
            for(int i=0; i<nVerfolger; i++)
            {
                theVerfolger[i]->setPar1(par1);
            }
        }
        break;

    case 'v':

        //neuer verfolger wird erstellt
        theVerfolger[nVerfolger] = new Verfolger(ofPoint(startX, startY), texturWidth, texturHeight, rangeWidth);
        theVerfolger[nVerfolger]->setSpeed(speed);
        theVerfolger[nVerfolger]->setPar1(par1);

        nVerfolger++;
        cout << "Verfolger \n";
        break;

    case 'b':

        //neuer chef wird erstellt
        theChef[nChef] = new Chef(ofPoint(startX, startY), texturWidth, texturHeight, rangeWidth);
        theChef[nChef]->setSpeed(speed);
        theChef[nChef]->setPar1(par1);

        nChef++;
        cout << "Chef \n";
        break;


//-----------------------------TRACKING-------------------------------------------

    case ' ':
        ofSetFrameRate(60);
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

        kinect2.setCameraTiltAngle(angle); // go back to prev tilt
        kinect2.open();
        break;

    case 'c':
        //Kinect ausschalten
        kinect.setCameraTiltAngle(0); // zero the tilt
        kinect.close();

        kinect2.setCameraTiltAngle(0); // zero the tilt
        kinect2.close();
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
