#ifndef CHEF_H_INCLUDED
#define CHEF_H_INCLUDED

#include "ofMain.h"
#include "ball.h"
#include "verfolger.h"

class Chef : public Ball{

    public:

        Chef(ofPoint, int);
        void update(float, ofPoint);
        void draw();

    private:

        ofPoint saved_move_to;
        ofImage textur;
        double angle;
};


#endif
