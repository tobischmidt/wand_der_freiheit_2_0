#include "testApp.h"
#include "chef.h"
#include "verfolger.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

//--------------------------------------------------------------

void testApp::setup()
{
    // Setting Vertical Sync ON in videocard driver and leaving out ofSetFrameRate helps!!
    //ofSetFrameRate(60);
    ofSetVerticalSync(false);

    ofSetWindowTitle("Wand der Freiheit");

    background.loadImage("left_wall_2.jpg");
    zitat.loadImage("zitat2.png");

    timeOld = ofGetElapsedTimeMillis();
    timeCur = timeOld;



    //Fenstergröße
    windowWidth = ofGetScreenWidth();
    windowHeight = ofGetScreenHeight();


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



    // set the tilt to value of "angle" on startup
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

    leftEnd[0].set(10000, 500);
    rightEnd[0].set(-10, 500);
    leftEnd[1].set(10000, 500);
    rightEnd[1].set(-10, 500);
    leftEnd[2].set(10000, 500);
    rightEnd[2].set(-10, 500);
    leftEnd[3].set(10000, 500);
    rightEnd[3].set(-10, 500);


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

    lineAdjustmentX = 0;
    lineAdjustmentY = 0;

    contourScaleWidth = windowWidth/2;
    contourScaleHeight = windowHeight;

    grauwert = 0;
    konturDicke = 3;
    spurLaenge = 8;

    rangeWidth = windowWidth;


//---------------------------VÖGEL------------------------------------------


    vogelTextur.loadImage("Vögel_weiß_Var3.png");
    drahtTextur.loadImage("Draht_weiß_neu.png");

    //nVerfolger = 24;
    //nChef = 8;
    nChef = 4;
    nVerfolger = 16;

    theChef.reserve(100);

    //nChef Chefs werden vordefiniert
    for (int i = 0; i < nChef; i++)
    {
        theChef.push_back( new Chef(ofPoint(ofRandom(1), ofRandom(1)), texturWidth, texturHeight, rangeWidth));
        theChef.back()->setSpeed(speed);
        theChef.back()->setPar1(par1);
    }


    theVerfolger.reserve(300);

    //nVerfolger Verfolger wird vordefiniert
    for (int i = 0; i < nVerfolger; i++)
    {
        theVerfolger.push_back( new Verfolger(ofPoint(ofRandom(1), ofRandom(1)), texturWidth, texturHeight, rangeWidth));
        theVerfolger.back()->setSpeed(speed);
        theVerfolger.back()->setPar1(par1);
    }


//--------------------------ABSCHLUSS-----------------------------------------------------

    blend = false;

    lineCounter = 0;

    // 4 Linien vordefiniert
    for(int i=0; i<420; i++)
    {
        if(i <= 70)
        {
            curveDefine1[i].set(3840 - (i*10), sin((float)i/15) * 50 + 400);
        }
        else
        {
            curveDefine1[i].set(3840 - (i*10), 350);
        }
    }

    for(int i=0; i<420; i++)
    {
        if(i <= 27)
        {
            curveDefine2[i].set(3840 - (i*10), sin((float)i/10 + M_PI*3200/2000) * 150 + 514);
        }
        else if(i > 27 && i <= 80)
        {
            curveDefine2[i].set(3840 - (i*10), sin((float)i/15 + M_PI*3620/2000) * 110 + 560);
        }
        else
        {
            curveDefine2[i].set(3840 - (i*10), 450);
        }
    }

    for(int i=0; i<420; i++)
    {
        if(i <= 63)
        {
            curveDefine3[i].set(3840 - (i*10), sin((float)i/20 + M_PI*3/2) * 125 + 425);
        }
        else
        {
            curveDefine3[i].set(3840 - (i*10), 550);
        }
    }

    for(int i=0; i<420; i++)
    {
        if(i <= 54)
        {
            curveDefine4[i].set(3840 - (i*10), sin((float)i/15 + M_PI) * 120 + 354);
        }
        else if(i > 54 && i <= 95)
        {
            curveDefine4[i].set(3840 - (i*10), sin((float)i/20 + M_PI) * 170 + 480);
        }
        else
        {
            curveDefine4[i].set(3840 - (i*10), 650);
        }
    }


    setzen = false;
    linien = false;
    createVerfolger = false;
    transformation = false;

    runCounter = -1;
    endCounter = 0;
    zitatCounter = 0;

    vec.push_back(ofVec2f(0, 0));

    blubbs.lineTo(ofPoint(0, 0));

    line.addVertex(vec[0]);
    line.clear();

    for(int i=0; i<4; i++)
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

    osc.listen();

    //Zähler, der alle 150 Durchläufe zurückgesetzt wird.
    runCounter++;
    if(runCounter > 150)
    {
        runCounter = 0;
        osc.sendToTablet(ofGetFrameRate(), kinect.isConnected(), kinect2.isConnected(), nVerfolger);
        //cout << "Verfolger: " << nVerfolger << "\n";
    }

//-------------------------------------------------------OSC----------------------------------------------

    for(int i=0; i<26; i++)
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
        leftEnd[0].set(10000, 500);
        rightEnd[0].set(-10, 500);
        leftEnd[1].set(10000, 500);
        rightEnd[1].set(-10, 500);
        leftEnd[2].set(10000, 500);
        rightEnd[2].set(-10, 500);
        leftEnd[3].set(10000, 500);
        rightEnd[3].set(-10, 500);

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
                contourFinder.findContours(grayImage, 1000, (kinect.width*kinect.height)/2, 1, false);
            }
        }

        if(contourFinder.blobs.size() > 0)//Wenn mindestens ein Objekt erkannt wird
        {
            //Bei einer Kinect nur 1 left & richt end jeweils
            //for (int j=0; j<2; j++)
            //{
                for( int i=0; i<contourFinder.blobs[0].nPts; i+=10 )//Iteriert durch die Punkte der Kontur, nimmt nur jeden dritten wegen Laufzeit
                {
                    if(contourFinder.blobs[0].pts[i].x < leftEnd[0].x)
                    {
                        //Wenn aktueller Punkt weiter links als gespeicherter Punkt wird dieser neu gespeichert
                        //x-Wert durch 2 geteilt, da nur auf linker Bidlschirmhälfte
                        leftEnd[0].set(contourFinder.blobs[0].pts[i].x, contourFinder.blobs[0].pts[i].y);
                    }

                    if(contourFinder.blobs[0].pts[i].x > rightEnd[0].x)
                    {
                        //Wenn aktueller Punkt weiter rechts als gespeicherter Punkt wird dieser neu gespeichert
                        rightEnd[0].set(contourFinder.blobs[0].pts[i].x, contourFinder.blobs[0].pts[i].y);
                    }
                }
            //}
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
                contourFinder2.findContours(grayImage2, 1000, (kinect2.width*kinect2.height)/2, 1, false);
            }
        }

        if(contourFinder2.blobs.size() > 0)
        {
            //for (int j=0; j<2; j++)
            //{
                for( int i=0; i<contourFinder2.blobs[0].nPts; i+=10 )//Iteriert durch die Punkte der Kontur, nimmt nur jeden dritten wegen Laufzeit
                {
                    if(contourFinder2.blobs[0].pts[i].x < leftEnd[0+1].x)
                    {
                        //Wenn aktueller Punkt weiter links als gespeicherter Punkt wird dieser neu gespeichert
                        //x-Wert durch 2 geteilt, da nur auf linker Bidlschirmhälfte
                        leftEnd[0+1].set(contourFinder2.blobs[0].pts[i].x, contourFinder2.blobs[0].pts[i].y);
                    }

                    if(contourFinder2.blobs[0].pts[i].x > rightEnd[0+1].x)
                    {
                        //Wenn aktueller Punkt weiter rechts als gespeicherter Punkt wird dieser neu gespeichert
                        rightEnd[0+1].set(contourFinder2.blobs[0].pts[i].x, contourFinder2.blobs[0].pts[i].y);
                    }
                }
            //}
        }
    }

#endif


//-------------------------------------------Abschluss--------------------------------------------------------------------

    if(linien)
    {
        //Über die Laufzeit den Polylines die Punkte übergeben, sodass sie "ins Bild laufen"
        if((lineCounter%2) == 0 && lineCounter/2 < 420)
        {
            curve[0].curveTo(curveDefine1[lineCounter/2].x - lineAdjustmentX, curveDefine1[lineCounter/2].y - lineAdjustmentY);
        }

        if((lineCounter%2) == 0 && lineCounter/2 < 420 && lineCounter > 20)
        {
            curve[1].curveTo(curveDefine2[lineCounter/2 - 10].x - lineAdjustmentX, curveDefine2[lineCounter/2 - 10].y - lineAdjustmentY);
        }

        if((lineCounter%2) == 0 && lineCounter/2 < 420 && lineCounter > 40)
        {
            curve[2].curveTo(curveDefine3[lineCounter/2 - 20].x - lineAdjustmentX, curveDefine3[lineCounter/2 - 20].y - lineAdjustmentY);
        }

        if((lineCounter%2) == 0 && lineCounter/2 < 420 && lineCounter > 30)
        {
            curve[3].curveTo(curveDefine4[lineCounter/2 - 15].x - lineAdjustmentX, curveDefine4[lineCounter/2 - 15].y - lineAdjustmentY);
        }

        lineCounter++;
    }

    if(lineCounter == 900)
    {
        setzen = true;

        for(int i=0; i<nVerfolger; i++)
        {
            theVerfolger[i]->resetEnd();
        }

        endCounter = 0;
        zitatCounter = 0;
    }



//-----------------------------------VÖGEL-----------------------------------------------------------

    //alle 150 Durchläufe
    if(!runCounter)
    {
        for(int i=0; i<nVerfolger; i++)
        {
            theVerfolger[i]->setSpeed(speed);
            theVerfolger[i]->newAbweichung();
        }
    }

    timeCur = ofGetElapsedTimeMillis();
    timeDiff = timeCur - timeOld;
    //position.set(0,0);

    hasContours = contourFinder.blobs.size();

    for (int i=0; i<nChef; i++)
    {
        // Wenn ein Körper von der Kinect getrackt wird
        if(!setzen)
        {
            if(hasContours)
            {
//                attraktoren[0] = rightEnd[0];
//                attraktoren[1] = leftEnd[0];
//                attraktoren[2] = rightEnd[1];
//                attraktoren[3] = leftEnd[1];
//                attraktoren[4] = rightEnd[2];
//                attraktoren[5] = leftEnd[2];
//                attraktoren[6] = rightEnd[3];
//                attraktoren[7] = leftEnd[3];
//
//                for(int j=0; j<4; j++)
//                {
//                    attraktoren[j].x = (attraktoren[j].x/kinect.width + adjustmentX/windowWidth) * contourScaleWidth/windowWidth;
//                    attraktoren[j].y = (attraktoren[j].y/kinect.height + adjustmentY/windowHeight) * contourScaleHeight/windowHeight;
//                }
//
//                for(int j=4; j<8; j++)
//                {
//                    if(kinect2.isConnected())
//                    {
//                        attraktoren[j].x = (attraktoren[j].x/kinect.width + adjustment2X/windowWidth) * contourScaleWidth/windowWidth;
//                        attraktoren[j].y = (attraktoren[j].y/kinect.height + adjustment2Y/windowHeight) * contourScaleHeight/windowHeight;
//                    }
//                    else
//                    {
//                        if(!runCounter)
//                        {
//                            attraktoren[j].set(ofRandom(0.5), ofRandom(0.35));
//                        }
//                        else
//                        {
//                            attraktoren[j].set(-1, -1);
//                        }
//                    }
//                }
//
//                if(i<8)
//                {
//                    theChef[i]->update(timeDiff, attraktoren[i]);
//                }
//
//                else
//                {
//                    if(!runCounter) // Alle 150 Durchläufe
//                    {
//                        // Zufälliger Position folgen.
//                        position.set(ofRandom(0.5), ofRandom(0.35)); // Bei schnellen Prozessoren pendeln die Kugeln sich in der Mitte aus. Hier müsste ein Timer eingebaut werden, damit die Chefs erstmal eine Zeit lang in eine Richtung fliegen.
//                    }
//                    else  // Ansonsten
//                    {
//                        // Dem letzten Punkt folgen.
//                        position.set(-1, -1);
//                    }
//
//                    theChef[i]->update(timeDiff, position);
//                }

//-----------------------ALTERNAT FÜR 1 PERSON----------------------------------------------------------

                attraktoren[0] = rightEnd[0];
                attraktoren[1] = leftEnd[0];
                attraktoren[2] = rightEnd[1];
                attraktoren[3] = leftEnd[1];

                for(int j=0; j<2; j++)
                {
                    attraktoren[j].x = (attraktoren[j].x/kinect.width + adjustmentX/windowWidth) * contourScaleWidth/windowWidth;
                    attraktoren[j].y = (attraktoren[j].y/kinect.height + adjustmentY/windowHeight) * contourScaleHeight/windowHeight;
                }

                for(int j=2; j<4; j++)
                {
                    if(kinect2.isConnected())
                    {
                        attraktoren[j].x = (attraktoren[j].x/kinect.width + adjustment2X/windowWidth) * contourScaleWidth/windowWidth;
                        attraktoren[j].y = (attraktoren[j].y/kinect.height + adjustment2Y/windowHeight) * contourScaleHeight/windowHeight;
                    }
                    else
                    {
                        if(!runCounter)
                        {
                            attraktoren[j].set(ofRandom(0.5), ofRandom(0.35));
                        }
                        else
                        {
                            attraktoren[j].set(-1, -1);
                        }
                    }
                }

                if(i<4)
                {
                    theChef[i]->update(timeDiff, attraktoren[i]);
                }

                else
                {
                    if(!runCounter) // Alle 150 Durchläufe
                    {
                        // Zufälliger Position folgen.
                        position.set(ofRandom(0.5), ofRandom(0.35)); // Bei schnellen Prozessoren pendeln die Kugeln sich in der Mitte aus. Hier müsste ein Timer eingebaut werden, damit die Chefs erstmal eine Zeit lang in eine Richtung fliegen.
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
                if(!runCounter) // Alle 150 Durchläufe
                {
                    // Zufälliger Position folgen.
                    position.set(ofRandom(0.5), ofRandom(1)); // Bei schnellen Prozessoren pendeln die Kugeln sich in der Mitte aus. Hier müsste ein Timer eingebaut werden, damit die Chefs erstmal eine Zeit lang in eine Richtung fliegen.
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
            position.set(0.5, -0.5);
            theChef[i]->update(timeDiff, position);
        }
    }

    if(!setzen)
    {
        if(createVerfolger && nVerfolger < 200)
        {
            //neuer Verfolger wird erstellt
            verfolgerIt = theVerfolger.begin();
            verfolgerIt += ofRandom(0, platzhalter);
            theVerfolger.insert(verfolgerIt, new Verfolger(ofPoint(startX, startY), texturWidth, texturHeight, rangeWidth));

            position.set(ofRandom(1), ofRandom(1));
            (*verfolgerIt)->update(timeDiff, position);
            (*verfolgerIt)->setSpeed(speed);
            (*verfolgerIt)->setPar1(par1);

            nVerfolger = theVerfolger.size();
            //cout << "Verfolger \n";
            cout << "Verfolger: " << nVerfolger << "\n";

            if(nVerfolger > 28 && nVerfolger%7==1)
            //if(nVerfolger > 40 && nVerfolger%5==1)
            {
                //neuer chef wird erstellt
                theChef.push_back( new Chef(ofPoint(startX, startY), texturWidth, texturHeight, rangeWidth));
                theChef.back()->setSpeed(speed);
                theChef.back()->setPar1(par1);

                nChef = theChef.size();
            }

            createVerfolger = false;
        }

        for (int i=0; i<nVerfolger; i++)
        {
            // Die Verfolger werden nacheinander den n Chefs zugeordnet.
            theVerfolger[i]->update(timeDiff, theChef[i%nChef]->getPos());

        }
    }
    else
    {
        endCounter++;
        if(endCounter > 1200)
        {
            zitatCounter++;
        }

        for(int i=0; i<nVerfolger; i++)
        {
            if(i<32)
            {

                if(sqrt(pow(theVerfolger[i]->getPos().x - (curveDefine1[i*10 + 70].x - lineAdjustmentX)/windowWidth, 2) + pow(theVerfolger[i]->getPos().y - (curveDefine1[i*10 + 70].y - lineAdjustmentY)/windowHeight, 2)) < 0.01)
                {
                    theVerfolger[i]->setPos(ofPoint((curveDefine1[i*10 + 70].x - lineAdjustmentX)/windowWidth, (curveDefine1[i*10 + 70].y - lineAdjustmentY)/windowHeight));
                }
                else
                {
                   theVerfolger[i]->update(timeDiff, ofPoint((curveDefine1[i*10 + 70].x - lineAdjustmentX)/windowWidth, (curveDefine1[i*10 + 70].y - lineAdjustmentY)/windowHeight));
                }
            }

            else if(i>=32 && i<63)
            {
                if(sqrt(pow(theVerfolger[i]->getPos().x - (curveDefine2[(i-32)*10 + 77].x - lineAdjustmentX)/windowWidth, 2) + pow(theVerfolger[i]->getPos().y - (curveDefine2[(i-32)*10 + 77].y - lineAdjustmentY)/windowHeight, 2)) < 0.01)
                {
                    theVerfolger[i]->setPos(ofPoint((curveDefine2[(i-32)*10 + 77].x - lineAdjustmentX)/windowWidth, (curveDefine2[(i-32)*10 + 77].y - lineAdjustmentY)/windowHeight));
                }
                else
                {
                    theVerfolger[i]->update(timeDiff, ofPoint((curveDefine2[(i-32)*10 + 77].x - lineAdjustmentX)/windowWidth, (curveDefine2[(i-32)*10 + 77].y - lineAdjustmentY)/windowHeight));
                }

            }

            else if(i>=63 && i<94)
            {
                if(sqrt(pow(theVerfolger[i]->getPos().x - (curveDefine3[(i-63)*10 + 80].x - lineAdjustmentX)/windowWidth, 2) + pow(theVerfolger[i]->getPos().y - (curveDefine3[(i-63)*10 + 80].y - lineAdjustmentY)/windowHeight, 2)) < 0.01)
                {
                    theVerfolger[i]->setPos(ofPoint((curveDefine3[(i-63)*10 + 80].x - lineAdjustmentX)/windowWidth, (curveDefine3[(i-63)*10 + 80].y - lineAdjustmentY)/windowHeight));
                }
                else
                {
                    theVerfolger[i]->update(timeDiff, ofPoint((curveDefine3[(i-63)*10 + 80].x - lineAdjustmentX)/windowWidth, (curveDefine3[(i-63)*10 + 80].y - lineAdjustmentY)/windowHeight));
                }

            }
            else if(i>=94 && i<124)
            {
                if(sqrt(pow(theVerfolger[i]->getPos().x - (curveDefine4[(i-94)*10 + 93].x - lineAdjustmentX)/windowWidth, 2) + pow(theVerfolger[i]->getPos().y - (curveDefine4[(i-94)*10 + 93].y - lineAdjustmentY)/windowHeight, 2)) < 0.01)
                {
                    theVerfolger[i]->setPos(ofPoint((curveDefine4[(i-94)*10 + 93].x - lineAdjustmentX)/windowWidth, (curveDefine4[(i-94)*10 + 93].y - lineAdjustmentY)/windowHeight));
                }
                else
                {
                    theVerfolger[i]->update(timeDiff, ofPoint((curveDefine4[(i-94)*10 + 93].x - lineAdjustmentX)/windowWidth, (curveDefine4[(i-94)*10 + 93].y - lineAdjustmentY)/windowHeight));
                }
            }
            else
            {
                theVerfolger[i]->update(timeDiff, ofPoint(0.5, -500));
            }

            theVerfolger[i]->setPar1(1);
        }
    }


    timeOld = timeCur;

    platzhalter = nVerfolger;

    if(platzhalter>40)
    {
        platzhalter = 40;
    }

    ofEnableAlphaBlending();

    trace.begin();
    drawContours();
    trace.end();

    if(blend)
    {
        blendCounter++;
    }

    if(blendCounter > 255)
    {
        setzen = false;
        endCounter = 0;

        linien = false;

        par1 = osc.settings[2];

        for(int i=nVerfolger; i>16; i--)
        //for(int i=nVerfolger; i>24; i--)
        {
            delete theVerfolger.back();
            theVerfolger.pop_back();
            nVerfolger--;
        }
        cout << "Verfolger: " << nVerfolger << "\n";

        for(int i=nChef; i>4; i--)
        //for(int i=nChef; i>8; i--)
        {
            delete theChef.back();
            theChef.pop_back();
            nChef--;
        }

        for(int i=0; i<nChef; i++)
        {
            theChef[i]->setPar1(par1);
        }
        for(int i=0; i<nVerfolger; i++)
        {
            theVerfolger[i]->setPar1(par1);
            theVerfolger[i]->resetEnd();
        }

    }

    if(blendCounter > 1555)
    {
        blend = false;
        blendCounter = 0;

        for(int i=0; i<curve.size(); i++)
        {
            curve[i].clear();
        }

        lineCounter = 0;
    }
}

//--------------------------------------------------------------

void testApp::updateOsc()
{
    if(osc.settingsUpdate[0] && osc.settings[0] != speed)
    {
        speed = osc.settings[0] * 0.00007;
        osc.settingsUpdate[0] = false;

        for(int i=0; i<nChef; i++)
        {
            theChef[i]->setSpeed(speed);
        }

        for(int i=0; i<nVerfolger; i++)
        {
            theVerfolger[i]->setSpeed(speed);
        }
    }

    if(osc.settingsUpdate[8] && osc.settings[8] == 1)
    {
        tracking = true;
    }

    if(osc.settingsUpdate[8] && osc.settings[8] == 0)
    {
        tracking = false;
    }

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
        nearThreshold = osc.settings[9] * 255;
        osc.settingsUpdate[9] = false;
    }

    if(osc.settingsUpdate[10] && osc.settings[10] != farThreshold)
    {
        farThreshold = osc.settings[10] * 255;
        osc.settingsUpdate[10] = false;
    }

    // verschiebung 1 x
    if(osc.settingsUpdate[7] && osc.settings[7] * 5000 != adjustmentX)
    {
        adjustmentX = osc.settings[7] * 5000;
        osc.settingsUpdate[7] = false;
    }

    // verschiebung 1 y
    if(osc.settingsUpdate[20] && osc.settings[20] * 5000 != adjustmentY)
    {
        adjustmentY = osc.settings[20] * 5000;
        osc.settingsUpdate[20] = false;
    }

    if(osc.settingsUpdate[18] && osc.settings[18] * 5000 != adjustment2X)
    {
        adjustment2X = osc.settings[18] * 5000 + windowWidth/2;
        osc.settingsUpdate[18] = false;
    }

    if(osc.settingsUpdate[21] && osc.settings[21] * 5000 != adjustment2Y)
    {
        adjustment2Y = osc.settings[21] * 5000;
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

    if(osc.settingsUpdate[1] && osc.settings[1] * 0.3 != texturWidth)
    {
        texturWidth = osc.settings[1] * 0.3;
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

    if(osc.settingsUpdate[16] && osc.settings[16] * 0.3 != texturHeight)
    {
        texturHeight = osc.settings[16] * 0.3;
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

    //Linienverschiebung x
    if(osc.settingsUpdate[11] && osc.settings[11] * windowWidth != lineAdjustmentX)
    {
        lineAdjustmentX = osc.settings[11] * windowWidth;
        osc.settingsUpdate[11] = false;
    }
    //Linienverschiebung y
    if(osc.settingsUpdate[14] && osc.settings[14] * windowHeight != lineAdjustmentY)
    {
        lineAdjustmentY = osc.settings[14] * windowHeight;
        osc.settingsUpdate[14] = false;
    }

    //Animation abspielen
    if(osc.settingsUpdate[13] && osc.settings[13] == 1)
    {
        linien = true;
        osc.settingsUpdate[13] = false;
    }

    if(osc.settingsUpdate[13] && osc.settings[13] == 0)
    {
        linien = false;
        lineCounter = 0;
        osc.settingsUpdate[13] = false;

        for(int i=0; i<curve.size(); i++)
        {
            curve[i].clear();
        }
    }

    //Übergang
    if(osc.settingsUpdate[5] && osc.settings[5] == 1)
    {
        blend = true;
        osc.settingsUpdate[5] = false;
    }
    if(osc.settingsUpdate[5] && osc.settings[5] == 0)
    {
        blend = false;
        blendCounter = 0;
        osc.settingsUpdate[5] = false;
    }

    if(osc.settingsUpdate[12] && osc.settings[12] != blubb)
    {
        blubb = windowWidth - (osc.settings[12] * windowWidth);
        osc.settingsUpdate[12] = false;
    }

    // Grauwert Kontur
    if(osc.settingsUpdate[26] && osc.settings[26] * 255 != grauwert)
    {
        grauwert = osc.settings[26] * 255;
        osc.settingsUpdate[26] = false;
    }

    // Konturdicke
    if(osc.settingsUpdate[27] && osc.settings[27] * 10!= konturDicke)
    {
        konturDicke = osc.settings[27] * 10;
        osc.settingsUpdate[27] = false;
    }

    // Spurlänge
    if(osc.settingsUpdate[28] && osc.settings[28] * 100 != spurLaenge)
    {
        spurLaenge = osc.settings[28] * 100 + 5;
        osc.settingsUpdate[28] = false;
    }

    if((osc.settingsUpdate[4] && osc.settings[4] == 1) || (osc.settingsUpdate[19] && osc.settings[19] == 1))
    {
        createVerfolger = true;
        osc.settings[4] = 0;
        osc.settings[19] = 0;
        osc.settingsUpdate[4] = false;
        osc.settingsUpdate[19] = false;
    }

    //---------------Rückgaben an Tablet------------------------------

    if(osc.settingsUpdate[25] && osc.settings[25] != nVerfolger)
    {
        nVerfolger = osc.settings[25];
        osc.settingsUpdate[25] = false;
    }
}


void testApp::drawContours()
{
    //ofFill();
    ofSetColor(0,0,0,spurLaenge);
    ofRect(0, 0, 3940, 1200);

    ofPushStyle();
    //ofSetLineWidth(3);

    // ---------------------------- draw the contours

    //ofFill();
    ofNoFill();
    ofSetColor(255, 255, 255);

    for( int i=0; i<(int)contourFinder.blobs.size(); i++ )
    {

        //contours.push_back(contourFinder.blobs[i].pts);
        contours.push_back(blubbs);
        contours[i].clear();

        for(int j=0; j<contourFinder.blobs[i].nPts; j+=2)
        {
            //contours[i].addVertex((contourFinder.blobs[i].pts[j].x*windowWidth/640 + adjustmentX) * contourScaleWidth/windowWidth, (contourFinder.blobs[i].pts[j].y*windowHeight/480 + adjustmentY) * contourScaleHeight/windowHeight);
            contours[i].lineTo((contourFinder.blobs[i].pts[j].x*windowWidth/640 + adjustmentX) * contourScaleWidth/windowWidth, (contourFinder.blobs[i].pts[j].y*windowHeight/480 + adjustmentY) * contourScaleHeight/windowHeight);
        }

        //contours[i].close();
        contours[i].draw();
    }
    contours.clear();

    for( int i=0; i<(int)contourFinder2.blobs.size(); i++ )
    {

        //contours.push_back(contourFinder2.blobs[i].pts);
        contours.push_back(blubbs);
        contours[i].clear();

        for(int j=0; j<contourFinder2.blobs[i].nPts; j+=2)
        {
            //contours[i].addVertex((contourFinder2.blobs[i].pts[j].x*windowWidth/640 + adjustment2X) * contourScaleWidth/windowWidth, (contourFinder2.blobs[i].pts[j].y*windowHeight/480 + adjustment2Y) * contourScaleHeight/windowHeight);
            contours[i].lineTo((contourFinder2.blobs[i].pts[j].x*windowWidth/640 + adjustment2X) * contourScaleWidth/windowWidth, (contourFinder2.blobs[i].pts[j].y*windowHeight/480 + adjustment2Y) * contourScaleHeight/windowHeight);
        }

        //contours[i].close();
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

        ofSetColor(255);

        for( int i=0; i<(int)contourFinder.blobs.size(); i++ )
        {

            //contours.push_back(contourFinder.blobs[i].pts);
            contours.push_back(blubbs);
            contours[i].clear();

            for(int j=0; j<contourFinder.blobs[i].nPts; j+=2)
            {
                //contours[i].addVertex((contourFinder.blobs[i].pts[j].x*windowWidth/640 + adjustmentX) * contourScaleWidth/windowWidth, (contourFinder.blobs[i].pts[j].y*windowHeight/480 + adjustmentY) * contourScaleHeight/windowHeight);
                contours[i].lineTo((contourFinder.blobs[i].pts[j].x*windowWidth/640 + adjustmentX) * contourScaleWidth/windowWidth, (contourFinder.blobs[i].pts[j].y*windowHeight/480 + adjustmentY) * contourScaleHeight/windowHeight);
            }

            contours[i].close();
            contours[i].setFillColor(grauwert);
            contours[i].setStrokeWidth(konturDicke);
            ofSetColor(255);
            contours[i].draw();
        }
        contours.clear();

        for( int i=0; i<(int)contourFinder2.blobs.size(); i++ )
        {

            //contours.push_back(contourFinder2.blobs[i].pts);
            contours.push_back(blubbs);
            contours[i].clear();

            for(int j=0; j<contourFinder2.blobs[i].nPts; j+=2)
            {
                //contours[i].addVertex((contourFinder2.blobs[i].pts[j].x*windowWidth/640 + adjustment2X) * contourScaleWidth/windowWidth, (contourFinder2.blobs[i].pts[j].y*windowHeight/480 + adjustment2Y) * contourScaleHeight/windowHeight);
                contours[i].lineTo((contourFinder2.blobs[i].pts[j].x*windowWidth/640 + adjustment2X) * contourScaleWidth/windowWidth, (contourFinder2.blobs[i].pts[j].y*windowHeight/480 + adjustment2Y) * contourScaleHeight/windowHeight);
            }

            contours[i].close();
            contours[i].setFillColor(grauwert);
            contours[i].setStrokeWidth(konturDicke);
            ofSetColor(255);
            contours[i].draw();

        }
        contours.clear();

        //Wenn enddraw = true werden die Attraktoren als rote Punkte dargestellt
        if(enddraw)
        {
            if(contourFinder.blobs.size() > 0)    //wenn mindestens ein Körper erkannt wird
            {
                //zeichnet 4 Punkte an äußersten Punkten der beiden erkannten Körper
                ofSetHexColor(0xFF0000);
                ofCircle(attraktoren[0].x*windowWidth, attraktoren[0].y*windowHeight, 7);
                ofCircle(attraktoren[1].x*windowWidth, attraktoren[1].y*windowHeight, 7);
                //Nur wenn 2 Körper, sonst nicht
                //ofCircle(attraktoren[2].x*windowWidth, attraktoren[2].y*windowHeight, 7);
                //ofCircle(attraktoren[3].x*windowWidth, attraktoren[3].y*windowHeight, 7);

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
                //mur Wenn 2 Körper, sonst nicht
                //ofCircle(attraktoren[6].x*windowWidth, attraktoren[6].y*windowHeight, 7);
                //ofCircle(attraktoren[7].x*windowWidth, attraktoren[7].y*windowHeight, 7);
            }
        }
    }

#endif

    contourFinder.blobs.clear();
    contourFinder2.blobs.clear();


//----------------------------VÖGEL--------------------------------------------------------

    //Bindet die Textur auf die Festplatte
    vogelTextur.getTextureReference().bind();

    //Zeichnet alle Chefs
    for (int i=0; i<nChef; i++)
    {
        theChef[i]->draw();
    }

    vogelTextur.getTextureReference().unbind();

    if(setzen && endCounter > 1200)
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

    if(blend)
    {
        if(blendCounter<1300)
        {
            ofSetColor(0, 0, 0, blendCounter);
        }

        else
        {
            ofSetColor(0, 0, 0, 1555-blendCounter);
        }

        ofRect(0, 0, windowWidth, windowHeight);
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

    case 'm' :

        blend = !blend;
        blendCounter = 0;
        break;

    case 'f' :
        //Fullcreen
        ofToggleFullscreen();
        windowWidth = ofGetWidth();
        windowHeight = ofGetHeight();
        break;

    case 'a' :

        lineAdjustmentX += 5;
        break;

    case 'd' :

        lineAdjustmentX -= 5;
        break;

    case 'w' :

        lineAdjustmentY += 5;
        break;

    case 'e' :

        lineAdjustmentY -= 5;
        break;

//----------------------------------VÖGEL------------------------------------------------

    case 'l' :

        for(int i=0; i<curve.size(); i++)
        {
            curve[i].clear();
        }

        lineCounter = 0;

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
                theVerfolger[i]->resetEnd();
                theVerfolger[i]->setPar1(par1);
            }
        }
        break;

    case 'v':

        createVerfolger = true;

        break;

    case 'b':

        //neuer chef wird erstellt
        theChef.push_back( new Chef(ofPoint(startX, startY), texturWidth, texturHeight, rangeWidth));
        theChef.back()->setSpeed(speed);
        theChef.back()->setPar1(par1);

        nChef = theChef.size();
        cout << "Chef \n";
        break;


//-----------------------------TRACKING-------------------------------------------

    case 'y':
        adjustmentX -= 20;
        break;

    case 'x':
        adjustmentX += 20;
        break;

    case 'j':
        adjustment2X -= 20;
        break;

    case 'k':
        adjustment2X += 20;
        break;

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
