#include "verfolger.h"


Verfolger::Verfolger(ofPoint _pos, float _texturWidth, float _texturHeight, float _rangeWidth){

    pos = _pos;
    speed = ofRandom(0.00005, 0.00008);
    saved_move_to = ofPoint(0,0);
    rangeWidth = _rangeWidth;
    texturWidth = _texturWidth;
    texturHeight = _texturHeight;
    frameCounterX = ofRandom(3);
    frameCounterY = ofRandom(7);
    frameCounterEndX = 0;
    frameCounterEndY = 0;
    abweichung = ofPoint(1, 1);
}

void Verfolger::setPos(ofPoint _pos)
{
    pos = _pos;
    drawPosX = pos.x*ofGetWidth();
    drawPosY = pos.y*ofGetHeight();
}

void Verfolger::setSpeed(float _speed)
{
    speed  =_speed * ofRandom(0.8, 1.2);
}

float Verfolger::getSpeed()
{
    return speed;
}

void Verfolger::newAbweichung()
{
    abweichung = ofPoint(ofRandom(-0.01, 0.01), ofRandom(-0.01, 0.01));
}

void Verfolger::setTexturWidth(float _texturWidth)
{
    texturWidth = _texturWidth;
}

void Verfolger::setTexturHeight(float _texturHeight)
{
    texturHeight = _texturHeight;
}

void Verfolger::setPar1(float _par1)
{
    par1 = _par1;
    par2 = 1 - par1;
}

void Verfolger::setRangeWidth(float _rangeWidth)
{
    rangeWidth = _rangeWidth;
}

void Verfolger::update(float dt, ofPoint move_to)
{
    // Wenn kein move_to mitgegeben wurde oder -1, dann dem letzten Punkt folgen, also kein Update durchführen.
    if(move_to.x >= 0){
       saved_move_to = move_to + abweichung;
    }

    // steuert die bewegung vom Verfolger - position= (aktuelle)position+richtung*geschwindigkeit*zeit
    dir = (saved_move_to - pos) * par1 + dir * par2;
    dir.normalize();
    pos += dir * speed * dt;

    // errechnet aus dem Richtungsvektor den Drehwinkel für die Flugrichtung
    flightAngle = ofVec2f(0, 1).angle(dir);

    //---------------------Darstellung
    drawPosX = pos.x*ofGetWidth();
    drawPosY = pos.y*ofGetHeight();

    // Bälle sollen den Bildschirm nicht verlassen
    /*if ((drawPosX > ofGetWidth() - rangeWidth)||(drawPosX < 0))
    {
        dir.x *= -1;
    }

    if ((drawPosY > ofGetHeight())||(drawPosY < 0 ))
    {
        dir.y *= -1;
    }*/

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

    frame.y = frameCounterY * 104 - 9;

    frameCounterX++;
}

void Verfolger::draw()
{
    ofSetColor(255);

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

void Verfolger::resetEnd()
{
    frameCounterEndX = 0;
    frameCounterEndY = 0;
}

void Verfolger::drawEnd(int _color){

    ofSetColor(1055-_color);

    if(_color>935)
    {
        ofSetColor(120);
    }

    if(frameCounterEndX/3 > 7)
    {
        frameCounterEndY++;
        frameCounterEndX = 0;
    }

    if(frameCounterEndY > 2)
    {
        frameCounterEndY = 2;
    }

    if(frameCounterEndX/3 > 4 && frameCounterEndY == 2)
    {
        frameCounterEndX = 12;
    }

    frame.x = frameCounterEndX/3 * 213;

    frame.y = frameCounterEndY * 104 - 9;

    frameCounterEndX++;

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
