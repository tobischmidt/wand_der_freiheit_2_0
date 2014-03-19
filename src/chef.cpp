#include "chef.h"


Chef::Chef(ofPoint _pos, int _dim){

    pos = _pos;
    dim = _dim;
    speed = ofRandom(0.0002,0.0003);
    saved_move_to = ofPoint(0,0);
    textur.loadImage("vogelhell.png");
    rangeWidth = ofGetWidth();
    flightAngle = 0;
}

void Chef::update(float dt, ofPoint move_to){
    par1=0.4;
    par2=0.6;

    // Wenn kein move_to mitgegeben wurde oder -1, dann dem letzten Punkt folgen, also kein Update durchführen.
        if(move_to.x >= 0){
            saved_move_to = move_to;
        }

    // steuert die bewegung vom Chef zur Maus - position= (aktuelle)position+richtung*geschwindigkeit*zeit
        dir = (saved_move_to - pos) * par1 + dir * par2;
        dir.normalize();
        pos += dir * speed * dt;

        // errechnet aus dem Richtungsvektor den Drehwinkel für die Flugrichtung
        flightAngle = acos(dir.x / sqrt(pow(dir.x, 2) + pow(dir.y, 2))) * 180 / 3.14159265;

}

void Chef::draw(){

    int drawPosX = pos.x * ofGetWidth();
    int drawPosY = pos.y * ofGetHeight();

    // Bälle sollen den Bildschirm nicht verlassen
    if ((drawPosX > rangeWidth - dim)||(drawPosX < dim))
    {
        dir.x *= -1;
    }

    if ((drawPosY > ofGetHeight() - dim)||(drawPosY < 0 + dim))
    {
        dir.y *= -1;
    }

    //Kreis in Farbe Rosa wird gezeichnet
    ofSetColor(ofColor(238,18,137));
    //ofCircle( drawPosX, drawPosY, dim);

    ofPushMatrix();
    ofTranslate(drawPosX, drawPosY, 0);
    ofRotateZ(flightAngle);
    //textur.setAnchorPercent(35, 20);
    textur.draw(0, 0, 70, 40);
    ofPopMatrix();

    }
