#include "testApp.h"
#include "chef.h"
#include "verfolger.h"

//--------------------------------------------------------------

// TODO: BEI MEHR ALS ca. 20 Verfolgern stürzt das Programm ab! -> solution
// Wand MUSS schwarz sein, sonnst kann man das nicht auf eine Wand projezieren.

void testApp::setup()
{
//---------------------------VÖGEL------------------------------------------

    vogelTextur.loadImage("vogelhell.png");

    nVerfolger = 1;

    nChef = 8;

    ofSetWindowTitle("Wand der Freiheit");

    theChef = new Chef*[nChef];
    theVerfolger = new Verfolger*[nVerfolger];

    //nChef Chefs werden vordefiniert
    for (int i = 0; i < nChef; i++)
    {
        ofPoint pos = ofPoint(ofRandom(1), ofRandom(1));
        float speed = ofRandom(0.0001,0.0003);/*NEW*/
        float texturWidth =  70; /*NEW*/
        float texturHeight =  40; /*NEW*/

        //erstellt ein Objekt mit den Koordinaten und dim
        theChef[i] = new Chef(pos, vogelTextur, speed/*NEW*/, texturWidth/*NEW*/, texturHeight/*NEW*/, rangeWidth/*new*/);
    }


    //nVerfolger Verfolger wird vordefiniert
    for (int i = 0; i < nVerfolger; i++)
    {
        ofPoint pos = ofPoint(ofRandom(1), ofRandom(1));
        float speed = ofRandom(0.0001,0.0003);/*NEW*/
        float createVerfolger = 0; /*NEW*/
        float texturWidth =  70; /*NEW*/
        float texturHeight =  40; /*NEW*/
        float startX = 0.99;    /*NEW*/
        float startY = 0.5; /*NEW*/

        // erstellt ein Objekt mit den Koordinaten
        theVerfolger[i] = new Verfolger(pos, vogelTextur, speed/*NEW*/, texturWidth/*NEW*/, texturHeight/*NEW*/, rangeWidth/*new*/);
    }

    timeOld = ofGetElapsedTimeMillis();
    timeCur = timeOld;

//    startX = 0.99;
//    startY = 0.5;


//-------------------------TRACKING-----------------------------------------------


    //Tracking & Zeichnen der Endpunkte zunächst ausgeschalten
    tracking = 0;
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

    // set the tilt to 15 on startup
 //   angle = 15;
    angle = osc.settings[11];
    kinect.setCameraTiltAngle(angle);

    osc.setup(); /*NEW*/

#ifdef USE_TWO_KINECTS
    //kinect2.init();
    kinect2.init(false, false);
    kinect2.open();

    grayImage2.allocate(kinect2.width, kinect2.height);

    kinect2.setCameraTiltAngle(angle);
#endif

//    adjustmentX = 0;
//    adjustmentY = 0;
//    adjustment2X = ofGetWidth()/2;
//    adjustment2Y = 0;
    adjustmentX = osc.settings[7]; /*NEW*/
    adjustmentY = 0;        /*NEW*/
    adjustment2X = osc.settings[18] /2; /*NEW*/
    adjustment2Y = 0;   /*NEW*/

    contourScaleWidth = osc.settings[6];    /*NEW*/
    contourScaleHeight = osc.settings[17];  /*NEW*/
//    contourScaleWidth = ofGetWidth();     /*old*/
//    contourScaleHeight = ofGetHeight();   /*old*/

}

//--------------------------------------------------------------

void testApp::update()
{
    //Hintergrundfarbe schwarz
    ofBackground(0);

    osc.listen();/*NEW*/

    if(osc.settings[8] == 1){
        tracking = 1;
    }
    else{
        tracking = 0;
    }
    cout << ofToString(tracking) << "\n" ;


    nearThreshold = osc.settings[9];
    farThreshold = osc.settings[10];
    angle = osc.settings[11];
    cout << "angle: " << angle << "\n" ;

    adjustmentX = osc.settings[7]; /*NEW*/
    adjustmentY = 0;        /*NEW*/
    adjustment2X = osc.settings[18] /2; /*NEW*/
    adjustment2Y = 0;   /*NEW*/

    contourScaleWidth = osc.settings[6];    /*NEW*/
    contourScaleHeight = osc.settings[17];  /*NEW*/

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
        if (tracking == 1) {
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
        if (tracking == 1) {
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

            attraktoren[0].x = (attraktoren[0].x/kinect.width + adjustmentX/ofGetWidth()) * contourScaleWidth/ofGetWidth();
            attraktoren[0].y = (attraktoren[0].y/kinect.height + adjustmentY/ofGetHeight()) * contourScaleHeight/ofGetHeight();

            attraktoren[1].x = (attraktoren[1].x/kinect.width + adjustmentX/ofGetWidth()) * contourScaleWidth/ofGetWidth();
            attraktoren[1].y = (attraktoren[1].y/kinect.height + adjustmentY/ofGetHeight()) * contourScaleHeight/ofGetHeight();

            attraktoren[2].x = (attraktoren[2].x/kinect.width + adjustmentX/ofGetWidth()) * contourScaleWidth/ofGetWidth();
            attraktoren[2].y = (attraktoren[2].y/kinect.height + adjustmentY/ofGetHeight()) * contourScaleHeight/ofGetHeight();

            attraktoren[3].x = (attraktoren[3].x/kinect.width + adjustmentX/ofGetWidth()) * contourScaleWidth/ofGetWidth();
            attraktoren[3].y = (attraktoren[3].y/kinect.height + adjustmentY/ofGetHeight()) * contourScaleHeight/ofGetHeight();

            attraktoren[4].x = (attraktoren[4].x/kinect.width + adjustment2X/ofGetWidth()) * contourScaleWidth/ofGetWidth();
            attraktoren[4].y = (attraktoren[4].y/kinect.height + adjustment2Y/ofGetHeight()) * contourScaleHeight/ofGetHeight();

            attraktoren[5].x = (attraktoren[5].x/kinect.width + adjustment2X/ofGetWidth()) * contourScaleWidth/ofGetWidth();
            attraktoren[5].y = (attraktoren[5].y/kinect.height + adjustment2Y/ofGetHeight()) * contourScaleHeight/ofGetHeight();

            attraktoren[6].x = (attraktoren[6].x/kinect.width + adjustment2X/ofGetWidth()) * contourScaleWidth/ofGetWidth();
            attraktoren[6].y = (attraktoren[6].y/kinect.height + adjustment2Y/ofGetHeight()) * contourScaleHeight/ofGetHeight();

            attraktoren[7].x = (attraktoren[7].x/kinect.width + adjustment2X/ofGetWidth()) * contourScaleWidth/ofGetWidth();
            attraktoren[7].y = (attraktoren[7].y/kinect.height + adjustment2Y/ofGetHeight()) * contourScaleHeight/ofGetHeight();

            theChef[i]->update(timeCur-timeOld, attraktoren[i], osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12]);

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
            theChef[i]->update(timeCur-timeOld, position, osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12]);
        }
    }

    for (int i=0; i<nVerfolger; i++)
    {
        // Die Verfolger werden nacheinander den n Chefs zugeordnet.
        theVerfolger[i]->update(timeCur-timeOld, theChef[i%nChef]->getPos(), osc.getSettings()[0], osc.getSettings()[1], osc.getSettings()[16], osc.getSettings()[2], osc.getSettings()[12]);
    }

    timeOld = timeCur;

    createVerfolger = osc.settings[4];
    createVerfolger = osc.settings[19];

    //cout << createVerfolger << "\n" ;

    if(createVerfolger == 1)
    {
        theVerfolger[nVerfolger] = new Verfolger(ofPoint(osc.getSettings()[3]/*startX,startY*/), vogelTextur, 0.0001/*speed*/, 70/*texturWidth*/, 40/*texturHeight*/, ofGetWidth()/*rangeWidth*/);
        nVerfolger++;

        createVerfolger = 0;
    }

}

//--------------------------------------------------------------

void testApp::draw()
{

//----------------------------------TRACKING----------------------------------------------


    ofSetColor(255, 255, 255);

    grayImage.draw(0, 0, ofGetWidth()/2, ofGetHeight());

    //Wenn Tracking aktiviert ist wird die Kontur gezeichnet
    if(tracking == 1) {
       contourFinder.draw(adjustmentX, adjustmentY, contourScaleWidth, contourScaleHeight);

       //Wenn enddraw = true werden die Attraktoren als rote Punkte dargestellt
       if(enddraw){
          if(contourFinder.blobs.size() > 0)    //wenn mindestens ein Körper erkannt wird
          {
               //zeichnet 4 Punkte an äußersten Punkten der beiden erkannten Körper
              //ofSetHexColor(0xFF0000);
              ofCircle(attraktoren[0].x*ofGetWidth(), attraktoren[0].y*ofGetHeight(), 7);
              ofCircle(attraktoren[1].x*ofGetWidth(), attraktoren[1].y*ofGetHeight(), 7);
              ofCircle(attraktoren[2].x*ofGetWidth(), attraktoren[2].y*ofGetHeight(), 7);
              ofCircle(attraktoren[3].x*ofGetWidth(), attraktoren[3].y*ofGetHeight(), 7);
          }
        }
    }

#ifdef USE_TWO_KINECTS

    ofSetHexColor(0xFFFFFF);

    grayImage2.draw(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());

    if (tracking == 1) {
       contourFinder2.setAnchorPoint(-adjustment2X, -adjustment2Y);
       contourFinder2.draw(0, 0, contourScaleWidth, contourScaleHeight);
       //cout << adjustment2X << " - ";

       if(enddraw){
          if(contourFinder2.blobs.size() > 0)    //wenn ein Körper erkannt wird
          {
              //zeichnet 4 Punkte an äußersten Punkten der beiden erkannten Körper
              //ofSetHexColor(0xFF0000);
              ofCircle(attraktoren[4].x*ofGetWidth(), attraktoren[4].y*ofGetHeight(), 7);
              ofCircle(attraktoren[5].x*ofGetWidth(), attraktoren[5].y*ofGetHeight(), 7);
              ofCircle(attraktoren[6].x*ofGetWidth(), attraktoren[6].y*ofGetHeight(), 7);
              ofCircle(attraktoren[7].x*ofGetWidth(), attraktoren[7].y*ofGetHeight(), 7);
          }
        }
    }

#endif


//----------------------------VÖGEL--------------------------------------------------------

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

    // Gibt Framerate in linker oberer Ecke aus
    ofDrawBitmapString(ofToString(ofGetFrameRate()),10,10);


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

    case 'f' :
        //Fullcreen
        ofToggleFullscreen();
        break;

//----------------------------------VÖGEL------------------------------------------------

    case 'v':

        //neuer verfolger wird erstellt
        theVerfolger[nVerfolger] = new Verfolger(ofPoint(osc.getSettings()[3]/*startX, startY*/), vogelTextur, 0.0001/*NEW*/, 70/*NEW*/, 40/*NEW*/, ofGetWidth()/*rangeWidth*/);
        nVerfolger++;
        break;

    case 'b':

        //neuer chef wird erstellt
        theChef[nChef] = new Chef(ofPoint(osc.getSettings()[3]/*startX, startY*/), vogelTextur, 0.0001/*NEW*/, 70/*NEW*/, 40/*NEW*/, ofGetWidth()/*rangeWidth*/);
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

    case 'w':
        kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
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

    case '1':
        kinect.setLed(ofxKinect::LED_GREEN);
        break;

    case '2':
        kinect.setLed(ofxKinect::LED_YELLOW);
        break;

    case '3':
        kinect.setLed(ofxKinect::LED_RED);
        break;

    case '4':
        kinect.setLed(ofxKinect::LED_BLINK_GREEN);
        break;

    case '5':
        kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
        break;

    case '0':
        kinect.setLed(ofxKinect::LED_OFF);
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
