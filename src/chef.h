#ifndef CHEF_H_INCLUDED
#define CHEF_H_INCLUDED

#include "ofMain.h"
#include "ball.h"
#include "verfolger.h"

class Chef : public Ball{

    public:

        Chef(ofPoint, float/*NEW*/, float/*texturWidth*/, float /*texturHeight*/, float /*rangeWidth*/, float /*grauwert*/);
        void update(float, ofPoint, float/*NEW*/, float/*texturWidth*/, float/*texturHeight*/, float /*distance*/, float /*rangeWidth*/, float /*grauwert*/);
        void draw();

    private:

        ofPoint saved_move_to;
        float flightAngle;
        float rangeWidth;
        float grauwert;

        float texturWidth;
        float texturHeight;
        float speed;
        float par1;

        ofPoint frame;

        int frameCounterX;
        int frameCounterY;
};


#endif
