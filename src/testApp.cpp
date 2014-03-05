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


    ofBackground(0,0,0);
    ofSetWindowTitle("Wand der Freiheit");

    theChef = new Chef*[nChef];
    theVerfolger = new Verfolger*[nVerfolger];

    for (int i = 0; i < nChef; i++)
    {

        int dim = 10;
        ofPoint pos = ofPoint(ofRandom(1), ofRandom(1));

        //erstellt ein Objekt mit den Koordinaten und dim
        theChef[i] = new Chef(pos, dim);
    }


    for (int i = 0; i < nVerfolger; i++)
    {

        int dim = 10;
        ofPoint pos = ofPoint(ofRandom(1), ofRandom(1));

        // erstellt ein Objekt mit den Koordinaten und dim
        theVerfolger[i] = new Verfolger(pos, dim);
    }

    timeOld = ofGetElapsedTimeMillis();
    timeCur = timeOld;


//-------------------------TRACKING-----------------------------------------------


    tracking = false;
    enddraw = false;

    ofSetLogLevel(OF_LOG_VERBOSE);

    // enable depth->video image calibration
    kinect.setRegistration(false);

    kinect.init();
    //kinect.init(false, false); // disable video image (faster fps)

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

/*#ifdef USE_TWO_KINECTS
    kinect2.init();
    kinect2.open();
#endif*/

    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);

    nearThreshold = 255;
    farThreshold = 70;
    bThreshWithOpenCV = true;

    ofSetFrameRate(60);

    // set the tilt to 15 on startup
    angle = 15;
    kinect.setCameraTiltAngle(angle);
}

//--------------------------------------------------------------

void testApp::update()
{
    ofBackground(0);

//-----------------------------------VÖGEL-----------------------------------------------------------

    timeCur = ofGetElapsedTimeMillis();
    ofPoint position = ofPoint(0,0);

    for (int i=0; i<nChef; i++)
    {
        // Wenn ein Körper von der Kinect erkannt wird
        if(contourFinder.blobs.size() > 0)
        {
            position.x = rightEnd.x/kinect.width;
            position.y = rightEnd.y/kinect.height;

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
        }
        theChef[i]->update(timeCur-timeOld, position);
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

        // find contours which are between the size of 10 pixels and 1/2 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        if (tracking) {
           contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 2, false);
        }
    }



    leftEnd.set(1000, 500);
    rightEnd.set(100, 500);

    if(contourFinder.blobs.size() > 0)
    {
        for( int i=0; i<contourFinder.blobs[0].nPts; i+=2 )
        {
            if(contourFinder.blobs[0].pts[i].x < leftEnd.x)
            {
                leftEnd.set(contourFinder.blobs[0].pts[i].x, contourFinder.blobs[0].pts[i].y);
            }

            if(contourFinder.blobs[0].pts[i].x > rightEnd.x)
            {
                rightEnd.set(contourFinder.blobs[0].pts[i].x, contourFinder.blobs[0].pts[i].y);
            }
        }
    }

}

//--------------------------------------------------------------

void testApp::draw()
{
    ofSetColor(255, 255, 255);

//----------------------------------TRACKING----------------------------------------------

    if (tracking) {
       contourFinder.draw(0, 0, ofGetWidth(), ofGetHeight());

       if(enddraw){
          if(contourFinder.blobs.size() > 0)    //wenn ein Körper erkannt wird
          {
              ofSetHexColor(0xFF0000);
              ofCircle(leftEnd.x*ofGetWidth()/640, leftEnd.y*ofGetHeight()/480, 7);   //zeichnet 2 Punkte an äußersten Punkten des Körpers
              ofCircle(rightEnd.x*ofGetWidth()/640, rightEnd.y*ofGetHeight()/480, 7);
          }
        }
    }


//----------------------------VÖGEL--------------------------------------------------------

    for (int i=0; i<nChef; i++)
    {
        theChef[i]->draw();
    }

    for (int i=0; i<nVerfolger; i++)
    {
        theVerfolger[i]->draw();
    }

    //ofDrawBitmapString(ofToString(ofGetFrameRate()),10,10);


/*#ifdef USE_TWO_KINECTS
    kinect2.draw(420, 320, 400, 300);
#endif*/

    // draw instructions
    //ofSetColor(0);
    /*stringstream reportStream;

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
    }*/

    //ofDrawBitmapString(reportStream.str(), 20, 652);

}


//--------------------------------------------------------------
void testApp::exit()
{
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();

/*#ifdef USE_TWO_KINECTS
    kinect2.close();
#endif*/
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if(!button){
        tracking = !tracking;
        cout << "tracking" << ofToString(tracking);
    }

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
        ofToggleFullscreen();
        break;

//----------------------------------VÖGEL------------------------------------------------

    case 'v':

        theVerfolger[nVerfolger] = new Verfolger(ofPoint(ofRandom(1), ofRandom(1)), 10);
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
        farThreshold ++;
        if (farThreshold > 255) farThreshold = 255;
        break;

    case '<':
    case ',':
        farThreshold --;
        if (farThreshold < 0) farThreshold = 0;
        break;

    case '+':
    case '=':
        nearThreshold ++;
        if (nearThreshold > 255) nearThreshold = 255;
        break;

    case '-':
        nearThreshold --;
        if (nearThreshold < 0) nearThreshold = 0;
        break;

    case 'w':
        kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
        break;

    case 'o':
        kinect.setCameraTiltAngle(angle); // go back to prev tilt
        kinect.open();
        break;

    case 'c':
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
        angle++;
        if(angle>30) angle=30;
        kinect.setCameraTiltAngle(angle);
        break;

    case OF_KEY_DOWN:
        angle--;
        if(angle<-30) angle=-30;
        kinect.setCameraTiltAngle(angle);
        break;


    default:
        break;
    }
}
