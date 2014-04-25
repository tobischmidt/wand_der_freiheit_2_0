#ifndef VERFOLGER_H_INCLUDED
#define VERFOLGER_H_INCLUDED

#include "ofMain.h"
#include "ball.h"

class Verfolger : public Ball{

    public:

        Verfolger(ofPoint, float, float, float);
        void update(float, ofPoint);
        void draw();
        void drawEnd(int);

        void setPos(ofPoint);
        void setSpeed(float);
        void setTexturWidth(float);
        void setTexturHeight(float);
        void setPar1(float);
        void setRangeWidth(float);

        void newAbweichung();
        void resetEnd();

        float getSpeed();

    private:

        ofPoint saved_move_to;
        float flightAngle;
        float rangeWidth;

        float texturWidth;
        float texturHeight;
        float speed;
        float par1;

        ofPoint frame;

        int frameCounterX;
        int frameCounterY;

        int frameCounterEndX;
        int frameCounterEndY;

        int drawPosX;
        int drawPosY;

        ofPoint abweichung;
};

#endif
