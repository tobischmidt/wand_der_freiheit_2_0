#ifndef VERFOLGER_H_INCLUDED
#define VERFOLGER_H_INCLUDED

#include "ofMain.h"
#include "ball.h"

class Verfolger : public Ball{

    public:

        Verfolger(ofPoint, float/*speed*/, float /*texturWidth*/, float /*texturHeight*/, float /*rangewidth*/, float /*grauwert*/);
        void update(float, vector<ofPoint>, float /*texturWidth*/, float /*texturHeight*/, float /*distance*/, float /*rangewidth*/, float /*grauwert*/);
        void update(float, ofPoint, float /*texturWidth*/, float /*texturHeight*/, float /*distance*/, float /*rangewidth*/, float /*grauwert*/);
        void draw();
        void drawEnd();
        void setSpeed(float);
        void newAbweichung();

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

        int drawPosX;
        int drawPosY;

        ofPoint abweichung;
};

#endif
