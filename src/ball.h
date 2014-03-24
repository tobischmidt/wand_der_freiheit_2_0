#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED

#include "ofMain.h"

class Ball{

    public:

        virtual void update(float, ofPoint, float/*speed*/, float/*texturWidth*/, float/*texturHeight*/, float /*distance*/, float) = 0;
        virtual void draw();
        ofPoint getPos();

    protected:

        float par1;
        float par2;
        ofPoint pos;
        ofPoint dir;
        float dim;
        float texturWidth;
        float texturHeight;
        float speed;
};

#endif
