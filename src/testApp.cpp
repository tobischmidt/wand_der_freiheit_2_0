#include "testApp.h"
#include "chef.h"
#include "verfolger.h"
#define nChef 4

//--------------------------------------------------------------

// TODO: BEI MEHR ALS ca. 20 Verfolgern stürzt das Programm ab! -> solution
// Wand MUSS schwarz sein, sonnst kann man das nicht auf eine Wand projezieren.

void testApp::setup()
{
//---------------------------VÖGEL------------------------------------------

    nVerfolger = 1;
//    nChef = 1;

    ofSetWindowTitle("Wand der Freiheit");

    theChef = new Chef*[nChef];
    theVerfolger = new Verfolger*[nVerfolger];

    //nChef = 4 Chefs werden vordefiniert
    for (int i = 0; i < nChef; i++)
    {

        int dim = 10;
        ofPoint pos = ofPoint(ofRandom(1), ofRandom(1));

        //erstellt ein Objekt mit den Koordinaten und dim
        theChef[i] = new Chef(pos, dim);
    }


    //nVerfolger = 1 Verfolger wird vordefiniert
    for (int i = 0; i < nVerfolger; i++)
    {

        int dim = 10;
        ofPoint pos = ofPoint(ofRandom(1), ofRandom(1));

        // erstellt ein Objekt mit den Koordinaten und dim
        theVerfolger[i] = new Verfolger(pos, dim);
    }

    timeOld = ofGetElapsedTimeMillis();
    timeCur = timeOld;

    /*attraktoren.push_back(ofPoint(200, 200));
    attraktoren.push_back(ofPoint(200, 600));
    attraktoren.push_back(ofPoint(1000 , 200));
    attraktoren.push_back(ofPoint(1000 , 600));*/


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

    // set the tilt to 15 on startup
    angle = 15;
    kinect.setCameraTiltAngle(angle);

#ifdef USE_TWO_KINECTS
    //kinect2.init();
    kinect2.init(false, false);
    kinect2.open();

    grayImage2.allocate(kinect2.width, kinect2.height);

    kinect2.setCameraTiltAngle(angle);
#endif

}

//--------------------------------------------------------------

void testApp::update()
{
    //Hintergrundfarbe schwarz
    ofBackground(0);

//-----------------------------------VÖGEL-----------------------------------------------------------

    timeCur = ofGetElapsedTimeMillis();
    ofPoint position = ofPoint(0,0);

    for (int i=0; i<nChef; i++)
    {
        // Wenn ein Körper von der Kinect getrackt wird
        if(contourFinder.blobs.size() > 0)
        {
            position.x = rightEnd[0].x/kinect.width;
            position.y = rightEnd[0].y/kinect.height;

            //attraktoren.push_back((ofPoint)rightEnd);
            //attraktoren.push_back(leftEnd);
            attraktoren[0] = rightEnd[0];
            attraktoren[1] = leftEnd[0];
            attraktoren[2] = rightEnd[1];
            attraktoren[3] = leftEnd[1];

            attraktoren[0].x /= kinect.width;
            attraktoren[0].y /= kinect.height;

            attraktoren[1].x /= kinect.width;
            attraktoren[1].y /= kinect.height;

            attraktoren[2].x /= kinect.width;
            attraktoren[2].y /= kinect.height;

            attraktoren[3].x /= kinect.width;
            attraktoren[3].y /= kinect.height;

            theChef[i]->update(timeCur-timeOld, attraktoren[i%4]);
            //cout << "Position X: " << position.x << " - Position Y: " << position.y;
            //cout << "Attraktor X: " << attraktoren[4].x << " - Attraktor Y: " << attraktoren[4].y;
            //theChef[i]->update(timeCur-timeOld, position);

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
            theChef[i]->update(timeCur-timeOld, position);
        }
        //theChef[i]->update(timeCur-timeOld, attraktoren[i]/1000);
        //cout << "attraktor X:" << ofToString(attraktoren[i].x) << "  Y: " << ofToString(attraktoren[i].y) ;
        //cout << "attraktor X:" << ofToString(position.x) << "  Y: " << ofToString(position.y) ;

    }
    for (int i=0; i<nVerfolger; i++)
    {
        // Die Verfolger werden nacheinander den n Chefs zugeordnet.
        theVerfolger[i]->update(timeCur-timeOld, theChef[i%nChef]->getPos());
    }
    timeOld = timeCur;


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

        // Wenn Trackign aktiviert wird
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
            for( int i=0; i<contourFinder.blobs[j].nPts; i+=3 )//Iteriert durch die Punkte der Kontur, nimmt nur jeden dritten wegen Laufzeit
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

    /*leftEnd2.set(5000, 500);
    rightEnd2.set(0, 500);

    if(contourFinder2.blobs.size() > 0)
    {
        for( int i=0; i<contourFinder2.blobs[0].nPts; i+=3 )
        {
            if(contourFinder2.blobs[0].pts[i].x/2 + ofGetWidth()/2 < leftEnd2.x)
            {
                leftEnd2.set(contourFinder2.blobs[0].pts[i].x/2 + ofGetWidth()/2, contourFinder2.blobs[0].pts[i].y);
            }

            if(contourFinder2.blobs[0].pts[i].x/2 + ofGetWidth()/2 > rightEnd2.x)
            {
                rightEnd2.set(contourFinder2.blobs[0].pts[i].x/2 + ofGetWidth()/2, contourFinder2.blobs[0].pts[i].y);
            }
        }
    }*/

#endif

}

//--------------------------------------------------------------

void testApp::draw()
{

//----------------------------------TRACKING----------------------------------------------



    ofSetColor(255, 255, 255);

    //grayImage.draw(0, 0, ofGetWidth()/2, ofGetHeight());
    //kinect.draw(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetWidth()/2*480/640);

    //Wenn Tracking aktiviert ist wird die Kontur gezeichnet
    if(tracking) {
       contourFinder.draw(0, 0, ofGetWidth()/2, ofGetHeight());

       //Wenn enddraw = true werden die Attraktoren als rote Punkte dargestellt
       if(enddraw){
          if(contourFinder.blobs.size() > 0)    //wenn mindestens ein Körper erkannt wird
          {
              ofSetHexColor(0xFF0000);
              ofCircle(leftEnd[0].x*ofGetWidth()/640, leftEnd[0].y*ofGetHeight()/480, 7);   //zeichnet 2 Punkte an äußersten Punkten des Körpers
              ofCircle(rightEnd[0].x*ofGetWidth()/640, rightEnd[0].y*ofGetHeight()/480, 7);
              ofCircle(leftEnd[1].x*ofGetWidth()/640, leftEnd[1].y*ofGetHeight()/480, 7);   //zeichnet 2 Punkte an äußersten Punkten des Körpers
              ofCircle(rightEnd[1].x*ofGetWidth()/640, rightEnd[1].y*ofGetHeight()/480, 7);
          }
        }
    }

#ifdef USE_TWO_KINECTS

    ofSetColor(255, 255, 255);

    //kinect2.draw(0, 0, ofGetWidth()/2, ofGetWidth()/2*480/640);
    //grayImage2.draw(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());

    if (tracking) {
       contourFinder2.draw(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());

       if(enddraw){
          if(contourFinder2.blobs.size() > 0)    //wenn ein Körper erkannt wird
          {
              ofSetHexColor(0xFF0000);
//              ofCircle(leftEnd2.x*ofGetWidth()/640, leftEnd2.y*ofGetHeight()/480, 7);   //zeichnet 2 Punkte an äußersten Punkten des Körpers
  //            ofCircle(rightEnd2.x*ofGetWidth()/640, rightEnd2.y*ofGetHeight()/480, 7);
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

    for(int i = 0; i<4; i++)
    {
        ofSetColor(255);
        ofCircle(attraktoren[i], 10);
    }

}


//--------------------------------------------------------------
void testApp::exit()
{
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();

#ifdef USE_TWO_KINECTS
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

        //neuer verfolger wird an zufälliger Position erstellt
        theVerfolger[nVerfolger] = new Verfolger(ofPoint(0.99, 0.6), 10);
        nVerfolger++;
        break;
//
//        case 'c':
//
//            theChef[nChef] = new Chef(ofPoint(ofRandom(1), ofRandom(1)), 20);
//            nChef++;
//            break;


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
