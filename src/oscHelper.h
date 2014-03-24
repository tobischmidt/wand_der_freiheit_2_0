#ifndef OSCHELPER_H_INCLUDED
#define OSCHELPER_H_INCLUDED

#include "ofMain.h"
#include "ofxOsc.h"

class oscHelper{

    public:

        void setup();
        void listen();
        float* getSettings(); /*NEW*/
        float settings[19];    /*NEW*/

    private:

        ofxOscReceiver receiver;    /*NEW*/

};

#endif