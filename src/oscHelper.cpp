#include "oscHelper.h"
#include "testApp.h"

void oscHelper::setup()
{

    cout << "listening for osc messages on port " << 1100 << "\n";
    receiver.setup(1100); // von Tablet

    cout << "listening for osc messages on port " << 4567 << "\n";
    herz2.setup(4567); // von Antonio

    /*---Vögel----*/
    settings[0] = 0; //Speed
    settings[1] = 0; //texturWidth
    settings[16] = 0; //texturHight
    settings[2] = 0; //distance
    settings[3] = 0; //ausgabe pos vögel X
    settings[15] = 0; //ausgabe pos vögel Y
    settings[4] = 0; //vogel ausgeben
    settings[5] = 0; //grauwert
    settings[19] = 0; // Vogel asugeben von Antonio


    /*---Silhoutte/Kinect----*/
    settings[6] = 0; //contourScaleWidth
    settings[17] = 0; //contourScaleHeight
    settings[7] = 0; //verschiebung 1 x
    settings[20] = 0; //verschiebung 1 y
    settings[18] = 0; //verschiebung 2 x
    settings[21] = 0; // verschiebung 2 y
    settings[8] = 0; //Interaktion an/aus

    settings[9] = 0; //nearThreshold
    settings[10] = 0; //farThreshold

     /*---Animation----*/
    settings[12] = 0; //Grenze rechts
    settings[13] = 0; //Linien ausgeben

    //settings[14] = 0; //osc herz

}

void oscHelper::listen()
{
    //cout << "bin da" << "\n";
   // sendeLeben();
    // hide old messages
//        for(int i = 0; i < NUM_MSG_STRINGS; i++){
//            if(timers[i] < ofGetElapsedTimef()){
//                msg_strings[i] = "";
//            }
//        }

    // check for waiting messages
    while(herz2.hasWaitingMessages())
    {
        ofxOscMessage n;
        herz2.getNextMessage(&n);
        cout << "sers" << n.getAddress() << "\n";
        // Vogel ausgeben von Antonio
        if(n.getAddress() == "/generateBird")
        {
            settings[19] = 1;
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
            settings[0] = (m.getArgAsFloat(0)) * 0.00015;
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
            settings[3] = (m.getArgAsFloat(1));
            settings[15] = (m.getArgAsFloat(0));
        }
        // Vogel ausgeben
        if(m.getAddress() == "/1/push1"){
            settings[4] = (m.getArgAsFloat(0));
        }
        // grauwert
        if(m.getAddress() == "/1/fader5"){
            settings[5] = (m.getArgAsFloat(0) * 255);
        }

/*------------------------Silhoutte/Kinect---------------------------*/
        // contourScaleWidth
        if(m.getAddress() == "/2/fader14"){
            settings[6] = (m.getArgAsFloat(0));
        }
        // contourScaleHeight
        if(m.getAddress() == "/2/fader2"){
            settings[17] = (m.getArgAsFloat(0));
        }
        // verschiebung 1 x
        if(m.getAddress() == "/2/fader3"){
            settings[7] = (m.getArgAsFloat(0));
        }
        // verschiebung 1 y
        if(m.getAddress() == "/2/fader5"){
            settings[20] = (m.getArgAsFloat(0));
        }
        // verschiebung 2 x
        if(m.getAddress() == "/2/fader6"){
            settings[18] = (m.getArgAsFloat(0));
        }
        // verschiebung 2 y
        if(m.getAddress() == "/2/fader7"){
            settings[21] = (m.getArgAsFloat(0));
        }
      // Interaktion starten
        if(m.getAddress() == "/2/toggle3"){
            settings[8] = (m.getArgAsFloat(0));
             cout << settings[8] << "\n" ;
        }
        // nearThreshold
         if(m.getAddress() == "/2/fader1"){
            settings[9] = (m.getArgAsFloat(0)) * 255;
        }
          // farThreshold
         if(m.getAddress() == "/2/fader4"){
            settings[10] = (m.getArgAsFloat(0)) * 255;
        }

/*------------------------------Animation------------------------------------*/
        // Grenze rechts
         if(m.getAddress() == "/3/fader18"){
            settings[12] = (m.getArgAsFloat(0)) ;
        }
         // Linien ausgeben
         if(m.getAddress() == "/3/toggle1"){
            settings[13] = (m.getArgAsFloat(0)) ;
        }
         // osc herz
         if(m.getAddress() == "/3/led2"){
            settings[19] = (m.getArgAsFloat(0)) ;
        }
         // save einstellung
         if(m.getAddress() == "/3/push1"){
            save();
        }
        //laden
        if(m.getAddress() == "/3/push2"){
            load();
        }
    }
}

/*
 * SETTING SPEICHERN
 */
    void oscHelper::sendeLeben(){
        cout << "sers \n";
    }

    void oscHelper::save(){

        XML.popTag();
        XML.clear();

        // -------------------------

        for(int i=0; i<22; i++){

            tagNum = XML.addTag( ofToString(i) );
            XML.setValue( ofToString(i) + ":VALUE", settings[i], tagNum );
            XML.popTag();
        }

        if(XML.saveFile("setting.xml")){
            cout << "alles gut" << endl;
        } else {
            cout << "geht net" << endl;
        }
    }

/*
 * SETTING LADEN
 */
void oscHelper::load(){

        XML.popTag();
        XML.clear();

        if( XML.loadFile("setting.xml") ){

            XML.popTag();

            for(int i=0; i<22; i++){

                XML.popTag();
                settings[i] = XML.getValue( ofToString(i) + ":VALUE", 0.5, 0);
            }

            cout << "Setting erfolgreich geladen!" << endl;

        } else {

            cout << "Datei -setting.xml- nicht gefunden!";
        }
    }


float* oscHelper::getSettings()
{
    return settings;
}

