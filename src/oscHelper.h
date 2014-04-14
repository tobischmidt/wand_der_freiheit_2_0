#ifndef OSCHELPER_H_INCLUDED
#define OSCHELPER_H_INCLUDED

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXMLSettings.h"

class oscHelper{

    public:

        void setup();
        void listen();
        float* getSettings(); /*NEW*/
        float settings[23];    /*NEW*/
        void save();
        void load();
        void sendeLeben();


    private:

        ofxOscReceiver receiver;    /*NEW*/
        ofxOscReceiver herz2;
        ofxXmlSettings XML;
        int tagNum;

};

#endif
