#ifndef OSCHELPER_H_INCLUDED
#define OSCHELPER_H_INCLUDED

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXMLSettings.h"

class oscHelper{

    public:

        void setup();
        void listen();
        float* getSettings();
        float settings[29];
        void save();
        void load();
        void syncSettingToOsc();

        void sendToTablet(float, bool, bool, float);

        bool settingsUpdate[29];

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
