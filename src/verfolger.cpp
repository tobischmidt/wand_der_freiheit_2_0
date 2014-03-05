#include "verfolger.h"


Verfolger::Verfolger(ofPoint _pos, int _dim){

    pos = _pos;
    dim = _dim;
    speed = ofRandom(0.0002,0.0003);
    saved_move_to = ofPoint(0,0);
}

void Verfolger::update(float dt, ofPoint move_to){

    par1=0.4;
    par2=0.6;

    // Wenn kein move_to mitgegeben wurde oder -1, dann dem letzten Punkt folgen, also kein Update durchf�hren.
        if(move_to.x >= 0){
            saved_move_to = move_to;
        }

    // steuert die bewegung vom Chef zur Maus - position= (aktuelle)position+richtung*geschwindigkeit*zeit
        dir = (saved_move_to - pos) * par1 + dir * par2;
        dir.normalize();
        pos += dir * speed * dt;
}

void Verfolger::draw(){

    int drawPosX = pos.x*ofGetWidth();
    int drawPosY = pos.y*ofGetHeight();

    // B�lle sollen den Bildschirm nicht verlassen
    if (drawPosX > ofGetWidth() - dim)
    {
        drawPosX = ofGetWidth() - dim;
    }
    if (drawPosX < dim)
    {
        drawPosX = dim;
    }

    if (drawPosY > ofGetHeight() - dim)
    {
        drawPosY = ofGetHeight() - dim;
    }
    if (drawPosY < 200)
    {
        drawPosY = 200;
    }

    ofSetColor(ofColor(112,112,112));

    ofCircle( drawPosX, drawPosY, dim);
}
