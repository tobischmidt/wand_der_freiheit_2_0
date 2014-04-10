#include "chef.h"


Chef::Chef(ofPoint _pos, float _speed/*NEW*/,float _texturWidth/*NEW*/, float _texturHeight/*NEW*/, float _rangeWidth/*NEW*/, float _grauwert/*new*/){

    pos = _pos;
    speed = _speed; /*NEW*/
    saved_move_to = ofPoint(0,0);
    rangeWidth = ofGetScreenWidth();
    flightAngle = 0;
    texturWidth = _texturWidth;
    texturHeight = _texturHeight;
    frameCounterX = 0;
    frameCounterY = 0;
    grauwert = _grauwert;
}

void Chef::update(float dt, ofPoint move_to, float _speed/*NEW*/,float _texturWidth/*NEW*/, float _texturHeight/*NEW*/, float _par1/*NEW*/, float _rangeWidth/*new*/, float _grauwert/*NEW*/)
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
        rangeWidth = _rangeWidth * ofGetWidth(); /*NEW*/
    }
    else
    {
        rangeWidth = 0;
    }

    if (_grauwert)
    {
        grauwert = _grauwert ;
    }
    else
    {
        grauwert = 255;
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
    if ((drawPosX > ofGetWidth() - rangeWidth)||(drawPosX < 0))
    {
        dir.x *= -1;
    }

    if ((drawPosY > ofGetHeight())||(drawPosY < 0))
    {
        dir.y *= -1;
    }

//    ofSetColor(ofColor(238,18,137));
     ofSetColor(grauwert);

    if(frameCounterX > 7)
    {
        frameCounterY++;
        frameCounterX = 0;
    }

    frame.x = frameCounterX * 213;

    if(frameCounterY > 7)
    {
        frameCounterY = 0;
    }

    frame.y = frameCounterY * 104;

    frameCounterX++;

    glPushMatrix();

        glTranslated(drawPosX, drawPosY, 0);
        glRotatef(flightAngle + 180, 0, 0, 1);
        glBegin(GL_QUADS);

            glTexCoord2f(frame.x, frame.y);
            glVertex2f(-texturWidth, -texturHeight);

            glTexCoord2f(frame.x + 213, frame.y);
            glVertex2f(texturWidth, -texturHeight);

            glTexCoord2f(frame.x + 213, frame.y + 104);
            glVertex2f(texturWidth, texturHeight);

            glTexCoord2f(frame.x, frame.y + 104);
            glVertex2f(-texturWidth, texturHeight);

            glEnd();

    glPopMatrix();

}
