#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED

#include "ofMain.h"

class Ball{

    public:

        virtual void update(float, ofPoint) = 0;
        virtual void draw();
        ofPoint getPos();

    protected:

        float par1;
        float par2;
        ofPoint pos;
        ofPoint dir;
        float speed;
        float dim;
};

#endif
