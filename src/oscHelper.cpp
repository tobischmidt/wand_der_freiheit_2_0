#include "oscHelper.h"
#include "testApp.h"

void oscHelper::setup()
{

    touchOsc.setup("192.168.1.101", 1300);

    arraySize = 29;

    cout << "listening for osc messages on port " << 1101 << "\n";
    receiver.setup(1101); // von Tablet

    cout << "listening for osc messages on port " << 4567 << "\n";
    herz.setup(4567); // von Antonio

    /*---Vögel----*/
    settings[0] = 0; //Speed
    settings[1] = 0; //texturWidth
    settings[16] = 0; //texturHight
    settings[2] = 0; //distance
    settings[3] = 0; //ausgabe pos vögel X
    settings[15] = 0; //ausgabe pos vögel Y
    settings[4] = 0; //vogel ausgeben

    settings[19] = 0; // Vogel ausgeben von Antonio


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
    settings[13] = 0; //Animation abspielenm
    settings[11] = 0; //Linien Verschiebung x
    settings[14] = 0; //Linien Verschiebung y
    settings[5] = 0; //Übergang

    /*-----Ausgaben----*/
    settings[22] = 0; // Framerate anzeigen
    settings[23] = 0; // Kinect 1
    settings[24] = 0; // Kinect 2
    settings[25] = 0; // Anzahl Verfolger

    for(int i=0; i<arraySize; i++)
    {
        settingsUpdate[i] = false;
    }


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
        herz.getNextMessage(&n);

        // Vogel ausgeben von Antonio
        if(n.getAddress() == "/generateBird")
        {
            settings[19] = 1;
            settingsUpdate[19] = true;
            cout << n.getAddress() << "\n";
        }
    }

    while(receiver.hasWaitingMessages())
    {
        // get the next message
        receiver.getNextMessage(&m);
        //cout << "Address: " << m.getAddress() << endl;



 /*----------------------------Vögel---------------------------*/
        // Speed
        if(m.getAddress() == "/0"){
            settings[0] = (m.getArgAsFloat(0));
            settingsUpdate[0] = true;
        }
        // texturWidth
        if(m.getAddress() == "/1"){
            settings[1] = (m.getArgAsFloat(0));
            settingsUpdate[1] = true;
        }
        // texturHeight
        if(m.getAddress() == "/16"){
            settings[16] = (m.getArgAsFloat(0));
            settingsUpdate[16] = true;
        }
        // distance par1
        if(m.getAddress() == "/2"){
            settings[2] = (m.getArgAsFloat(0));
            settingsUpdate[2] = true;
        }
        //ausgabe pos vögel
        if(m.getAddress() == "/3"){
            settings[3] = (m.getArgAsFloat(1));
            settings[15] = (m.getArgAsFloat(0));

            settingsUpdate[3] = true;
            settingsUpdate[15] = true;
        }
        // Vogel ausgeben
        if(m.getAddress() == "/4"){
            settings[4] = (m.getArgAsFloat(0));
            settingsUpdate[4] = true;
        }

/*------------------------Silhoutte/Kinect---------------------------*/
        // contourScaleWidth
        if(m.getAddress() == "/6"){
            settings[6] = (m.getArgAsFloat(0));
            settingsUpdate[6] = true;
        }
        // contourScaleHeight
        if(m.getAddress() == "/17"){
            settings[17] = (m.getArgAsFloat(0));
            settingsUpdate[17] = true;
        }
        // verschiebung 1 x
        if(m.getAddress() == "/7"){
            settings[7] = (m.getArgAsFloat(0));
            settingsUpdate[7] = true;
        }
        // verschiebung 1 y
        if(m.getAddress() == "/20"){
            settings[20] = (m.getArgAsFloat(0));
            settingsUpdate[20] = true;
        }
        // verschiebung 2 x
        if(m.getAddress() == "/18"){
            settings[18] = (m.getArgAsFloat(0));
            settingsUpdate[18] = true;
        }
        // verschiebung 2 y
        if(m.getAddress() == "/21"){
            settings[21] = (m.getArgAsFloat(0));
            settingsUpdate[21] = true;
        }
      // Interaktion starten
        if(m.getAddress() == "/8"){
            settings[8] = (m.getArgAsFloat(0));
            settingsUpdate[8] = true;
             cout << "Interaktion" << settings[8] << "\n" ;
        }
        // nearThreshold
         if(m.getAddress() == "/9"){
            settings[9] = (m.getArgAsFloat(0));
            settingsUpdate[9] = true;
        }
          // farThreshold
         if(m.getAddress() == "/10"){
            settings[10] = (m.getArgAsFloat(0));
            settingsUpdate[10] = true;
        }

/*------------------------------Animation------------------------------------*/
        // Grenze rechts
         if(m.getAddress() == "/12"){
            settings[12] = (m.getArgAsFloat(0)) ;
            settingsUpdate[12] = true;
        }
         // Animation starten
         if(m.getAddress() == "/13"){
            settings[13] = (m.getArgAsFloat(0)) ;
            settingsUpdate[13] = true;
        }

        //Übergang
        if(m.getAddress() == "/5"){
            settings[5] = (m.getArgAsFloat(0)) ;
            settingsUpdate[5] = true;
        }


        // Linien Verschiebung x
         if(m.getAddress() == "/11"){
            settings[11] = (m.getArgAsFloat(0)) ;
            settingsUpdate[11] = true;
        }

        // Linien Verschiebung y
         if(m.getAddress() == "/14"){
            settings[14] = (m.getArgAsFloat(0)) ;
            settingsUpdate[14] = true;
        }

        // Grauwert Silhouette
         if(m.getAddress() == "/26"){
            settings[26] = (m.getArgAsFloat(0)) ;
            settingsUpdate[26] = true;
        }

        // Konturdicke
         if(m.getAddress() == "/27"){
            settings[27] = (m.getArgAsFloat(0)) ;
            settingsUpdate[27] = true;
        }

        // Spurlänge
         if(m.getAddress() == "/28"){
            settings[28] = (m.getArgAsFloat(0)) ;
            settingsUpdate[28] = true;
        }

/*-------------------------------Lade/Speicher------------------------------*/


        // save einstellung
        if(m.getAddress() == "/3/push/2"){
            cout << "SaveButton pushed \n";
            save();
        }

        //laden
        if(m.getAddress() == "/3/push/1"){
            cout << "LoadButton pushed \n";
            load();
        }
    }
}

/*
 * SETTING SPEICHERN
 */

void oscHelper::save(){

        XML.popTag();
        XML.clear();

        // -------------------------

        for(int i=0; i<29; i++){

            if(i<22 || i>25)
            {
                tagNum = XML.addTag("NR" + ofToString(i) );
                XML.setValue("NR" + ofToString(i) + ":VALUE", settings[i], tagNum );
                XML.popTag();
            }
        }

        if(XML.saveFile("setting.xml")){
            cout << "Settings erfolgreich gespeichert" << endl;
        } else {
            cout << "Settings speichern erfolglos" << endl;
        }
    }


/*------------------------TouchOSC synchronisieren---------------------------------*/

void oscHelper::syncSettingToOsc(){

        ofxOscMessage m;
        for(int i=0; i<arraySize; i++)
        {
            m.clear();
            m.setAddress("/" + ofToString(i));
            m.addFloatArg(settings[i]);
            touchOsc.sendMessage(m);
        }
}

void oscHelper::sendToTablet(float _frameRate, bool _kinect1, bool _kinect2, float _nVerfolger){

        //cout << "Data sent to Tablet \n";

        ofxOscMessage m;

        m.clear();
        m.setAddress("/22");
        m.addFloatArg(_frameRate);
        touchOsc.sendMessage(m);

        m.clear();
        m.setAddress("/23");
        m.addFloatArg(_kinect1);
        touchOsc.sendMessage(m);

        m.clear();
        m.setAddress("/24");
        m.addFloatArg(_kinect2);
        touchOsc.sendMessage(m);

        m.clear();
        m.setAddress("/25");
        m.addFloatArg(_nVerfolger);
        touchOsc.sendMessage(m);
}


/*
 * SETTING LADEN
 */
void oscHelper::load(){

        if( XMLloading.loadFile("setting.xml") ){

            XMLloading.popTag();

            for(int i=0; i<29; i++){

                if(i<22 || i>25)
                {
                    XMLloading.popTag();
                    settings[i] = XMLloading.getValue("NR" + ofToString(i) + ":VALUE", 0.5, 0);
                    settingsUpdate[i] = true;
                }
            }

            syncSettingToOsc();

            cout << "Setting erfolgreich geladen!" << endl;

        } else {

            cout << "Datei -setting.xml- nicht gefunden!";
        }
    }


float* oscHelper::getSettings()
{
    return settings;
}

