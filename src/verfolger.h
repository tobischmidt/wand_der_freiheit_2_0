#ifndef VERFOLGER_H_INCLUDED
#define VERFOLGER_H_INCLUDED

#include "ofMain.h"
#include "ball.h"

class Verfolger : public Ball{

    public:

        Verfolger(ofPoint, int);
        void update(float, ofPoint);
        void draw();

    private:

        ofPoint saved_move_to;
        ofImage textur;
        double angle;
};

#endif
