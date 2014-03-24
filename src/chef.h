#ifndef CHEF_H_INCLUDED
#define CHEF_H_INCLUDED

#include "ofMain.h"
#include "ball.h"
#include "verfolger.h"

class Chef : public Ball{

    public:

        Chef(ofPoint, ofImage, float/*NEW*/, float/*texturWidth*/, float /*texturHeight*/, float /*rangeWidth*/);
        void update(float, ofPoint, float/*NEW*/, float/*texturWidth*/, float/*texturHeight*/, float /*distance*/, float /*rangeWidth*/);
        void draw();

    private:

        ofPoint saved_move_to;
        ofImage textur;
        float flightAngle;
        float rangeWidth;

        float texturWidth;
        float texturHeight;
        float speed;
        float par1;
};


#endif
