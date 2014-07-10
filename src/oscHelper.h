#ifndef OSCHELPER_H_INCLUDED
#define OSCHELPER_H_INCLUDED

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXMLSettings.h"

class oscHelper{

    public:

        void setup();
        void listen();  //Auf OSC Nachricht warten
        float* getSettings();  //Wert von Setting abfragen
        float settings[29];
        void save();  //Werte in xml Datei speichern
        void load();  //werte aus xml Datei laden
        void syncSettingToOsc();  //TouchOSC synchornisieren

        void sendToTablet(float, bool, bool, float);  //Daten an Tablet senden

        bool settingsUpdate[29];  //Setting hat neuen Wert erhalten

        ofxOscMessage n;
        ofxOscMessage m;


    private:

        ofxOscReceiver receiver;
        ofxOscReceiver herz;
        ofxOscSender touchOsc;
        ofxXmlSettings XML;
        ofxXmlSettings XMLloading;
        int tagNum, arraySize;

};

#endif
