#ifndef CHEF_H_INCLUDED
#define CHEF_H_INCLUDED

#include "ofMain.h"
#include "ball.h"
#include "verfolger.h"

class Chef : public Ball{

    public:

        Chef(ofPoint, float, float, float);
        void update(float, ofPoint);
        void draw();

        void setSpeed(float);
        void setTexturWidth(float);
        void setTexturHeight(float);
        void setPar1(float);
        void setRangeWidth(float);

    private:

        ofPoint saved_move_to;
        float flightAngle;
        float rangeWidth;

        float texturWidth;
        float texturHeight;
        float speed;
        float par1;

        int drawPosX;
        int drawPosY;

        ofPoint frame;

        int frameCounterX;
        int frameCounterY;
};


#endif
