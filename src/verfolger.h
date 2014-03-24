#ifndef VERFOLGER_H_INCLUDED
#define VERFOLGER_H_INCLUDED

#include "ofMain.h"
#include "ball.h"

class Verfolger : public Ball{

    public:

        Verfolger(ofPoint, ofImage, float/*speed*/, float /*texturWidth*/, float /*texturHeight*/, float /*rangewidth*/);
        void update(float, ofPoint, float/*speed*/, float /*texturWidth*/, float /*texturHeight*/, float /*distance*/, float /*rangewidth*/);
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
