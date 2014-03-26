#include "oscHelper.h"
#include "testApp.h"

void oscHelper::setup()
{

    cout << "listening for osc messages on port " << 1100 << "\n";
    receiver.setup(1100); // von Tablet

    cout << "listening for osc messages on port " << 1919 << "\n";
    herz.setup(1919); // von Antonio

    /*---Vögel----*/
    settings[0] = 0; //Speed
    settings[1] = 0; //texturWidth
    settings[16] = 0; //texturHight
    settings[2] = 0; //distance
    settings[3] = 0; //ausgabe pos vögel
    settings[4] = 0; //vogel ausgeben (hängt sich bei 4 vögel auf)
    settings[19] = 0; // Vogel asugeben von Antonio


    /*---Silhoutte/Kinect----*/
    settings[6] = 0; //contourScaleWidth
    settings[17] = 0; //contourScaleHeight
    settings[7] = 0; //verschiebung 1
    settings[18] = 0; //verschiebung 2
    settings[8] = 0; //Interaktion an/aus

    settings[9] = 0; //nearThreshold
    settings[10] = 0; //farThreshold
    settings[11] = 0; //angle

     /*---Animation----*/
    settings[12] = 0; //Grenze rechts
    settings[13] = 0; //Ausgangspunkt Linien

    settings[14] = 0; //osc herz
    settings[15] = 0; //save Einstellung

}

void oscHelper::listen()
{
    // hide old messages
//        for(int i = 0; i < NUM_MSG_STRINGS; i++){
//            if(timers[i] < ofGetElapsedTimef()){
//                msg_strings[i] = "";
//            }
//        }

    // check for waiting messages
    while(herz.hasWaitingMessages())
    {
        ofxOscMessage n;
        herz.getNextMessage(&n);

        // Vogel ausgeben von Antonio
        if(n.getAddress() == "/generateBird")
        {
            settings[19] = (n.getArgAsFloat(0));
            cout << n.getAddress() << "\n";
        }
    }

    while(receiver.hasWaitingMessages())
    {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);



 /*----------------------------Vögel---------------------------*/
        // Speed
        if(m.getAddress() == "/1/fader1"){
            settings[0] = (m.getArgAsFloat(0)) * 0.0003;
        }
        // texturWidth
        if(m.getAddress() == "/1/fader2"){
            settings[1] = (m.getArgAsFloat(0));
        }
        // texturHeight
        if(m.getAddress() == "/1/fader4"){
            settings[16] = (m.getArgAsFloat(0));
        }
        // distance par1
        if(m.getAddress() == "/1/fader3"){
            settings[2] = (m.getArgAsFloat(0));
        }
        //ausgabe pos vögel
        if(m.getAddress() == "/1/xy1"){
            settings[3] = (m.getArgAsFloat(0));
        }
        // Vogel ausgeben
        if(m.getAddress() == "/1/push1"){
            settings[4] = (m.getArgAsFloat(0));
        }
//        // speed video
//        if(m.getAddress() == "/1/fader9"){
//            settings[5] = (m.getArgAsFloat(0));
//        }

/*------------------------Silhoutte/Kinect---------------------------*/
        // contourScaleWidth
        if(m.getAddress() == "/2/fader14"){
            settings[6] = (m.getArgAsFloat(0));
        }
        // contourScaleHeight
        if(m.getAddress() == "/2/fader2"){
            settings[17] = (m.getArgAsFloat(0));
        }
//        // verschiebung 1
        if(m.getAddress() == "/2/xy1"){
            settings[7] = (m.getArgAsFloat(0));
        }
//        // verschiebung 2
        if(m.getAddress() == "/2/xy2"){
            settings[18] = (m.getArgAsFloat(0));
        }
//      // Interaktion starten
        if(m.getAddress() == "/2/toggle3"){
            settings[8] = (m.getArgAsFloat(0));
             cout << settings[8] << "\n" ;
        }
//        // nearThreshold
         if(m.getAddress() == "/2/fader1"){
            settings[9] = (m.getArgAsFloat(0)) * 255;
        }
//          // farThreshold
         if(m.getAddress() == "/2/fader4"){
            settings[10] = (m.getArgAsFloat(0)) * 255;
        }
//         // angle
         if(m.getAddress() == "/2/fader5"){
            settings[11] = (m.getArgAsFloat(0)) * 30;
        }

/*------------------------------Animation------------------------------------*/
//        // Grenze rechts
         if(m.getAddress() == "/3/fader18"){
            settings[12] = (m.getArgAsFloat(0)) ;
        }
//         // ausgangspunkt linien
//         if(m.getAddress() == "/3/xy3"){
//            settings[13] = (m.getArgAsFloat(0)) ;
//        }
//         // osc herz
         if(m.getAddress() == "/3/led2"){
            settings[19] = (m.getArgAsFloat(0)) ;
        }
//         // save einstellung
//         if(m.getAddress() == "/3/toggle4"){
//            settings[15] = (m.getArgAsFloat(0)) ;
//        }
    }
}

float* oscHelper::getSettings()
{
    return settings;
}

