#include "chef.h"


Chef::Chef(ofPoint _pos, ofImage _textur, float _speed/*NEW*/,float _texturWidth/*NEW*/, float _texturHeight/*NEW*/, float _rangeWidth/*NEW*/){

    pos = _pos;
    speed = _speed; /*NEW*/
    saved_move_to = ofPoint(0,0);
    textur = _textur;
    rangeWidth = ofGetWidth();
    flightAngle = 0;
    texturWidth = _texturWidth;
    texturHeight = _texturHeight;
}

void Chef::update(float dt, ofPoint move_to, float _speed/*NEW*/,float _texturWidth/*NEW*/, float _texturHeight/*NEW*/, float _par1/*NEW*/, float _rangeWidth/*new*/)
{
    par1= 0.4;
    par2= 1- par1;

    if(_texturWidth)
    {
        texturWidth = _texturWidth;/*NEW*/
    }

    if(_texturHeight)
    {
        texturHeight = _texturHeight; /*NEW*/
    }

    if(_speed)
    {
        speed = _speed; /*NEW*/
    }

    if (_par1)
    {
        par1 = _par1;  /*NEW*/
        par2= 1- par1; /*NEW*/
    }

    if (_rangeWidth)
    {
        rangeWidth = _rangeWidth; /*NEW*/
    }



    // Wenn kein move_to mitgegeben wurde oder -1, dann dem letzten Punkt folgen, also kein Update durchführen.
    if(move_to.x >= 0)
    {
        saved_move_to = move_to;
    }

    // steuert die bewegung vom Chef zur Maus - position= (aktuelle)position+richtung*geschwindigkeit*zeit
    dir = (saved_move_to - pos) * par1 + dir * par2;
    dir.normalize();
    pos += dir * speed * dt;

    // errechnet aus dem Richtungsvektor den Drehwinkel für die Flugrichtung
    flightAngle = ofVec2f(0, 1).angle(dir);
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

    ofSetColor(ofColor(238,18,137));

    ofPushMatrix();
    ofTranslate(drawPosX, drawPosY, 0);
    ofRotateZ(flightAngle + 180);
    textur.setAnchorPoint(35, 20);
    textur.draw(0, 0, texturWidth, texturHeight);
    ofPopMatrix();
}
