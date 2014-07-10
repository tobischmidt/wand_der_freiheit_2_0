#include "chef.h"


Chef::Chef(ofPoint _pos, float _texturWidth, float _texturHeight, float _rangeWidth){

    pos = _pos;
    speed = 0.00005;
    saved_move_to = ofPoint(0,0);
    rangeWidth = ofGetScreenWidth();
    flightAngle = 0;
    texturWidth = _texturWidth;
    texturHeight = _texturHeight;
    frameCounterX = ofRandom(3);
    frameCounterY = ofRandom(7);
}

void Chef::setSpeed(float _speed)
{
    speed  =_speed;
}

void Chef::setTexturWidth(float _texturWidth)
{
    texturWidth = _texturWidth;
}

void Chef::setTexturHeight(float _texturHeight)
{
    texturHeight = _texturHeight;
}

void Chef::setPar1(float _par1)
{
    par1 = _par1;
    par2 = 1 - par1;
}

void Chef::setRangeWidth(float _rangeWidth)
{
    rangeWidth = _rangeWidth;
}

void Chef::update(float dt, ofPoint move_to)
{
    // Wenn kein move_to mitgegeben wurde oder -1, dann dem letzten Punkt folgen, also kein Update durchführen.
    if(move_to.x >= 0)
    {
        saved_move_to = move_to;
    }

    // steuert die bewegung vom Chef - position= (aktuelle)position+richtung*geschwindigkeit*zeit
    dir = (saved_move_to - pos) * par1 + dir * par2;
    dir.normalize();
    pos += dir * speed * dt;


    // errechnet aus dem Richtungsvektor den Drehwinkel für die Flugrichtung
    flightAngle = ofVec2f(0, 1).angle(dir);

    //--------------------Darstellung

    //Zeichenposition des Vogels
    drawPosX = pos.x * ofGetWidth();
    drawPosY = pos.y * ofGetHeight();

    //aktuellen Frame der Fluganimation bestimmen

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
}

void Chef::draw()
{

    ofSetColor(255);

    //aktuellen Frame aus dem png, das auf der Grafikarte liegt, herausnehmen und zeichnen
    glPushMatrix();

        glTranslated(drawPosX, drawPosY, 0);  //Ursprung auf Position des Vogels setzen
        glRotatef(flightAngle + 180, 0, 0, 1);  //Textur in Flugrichtung ausrichten
        glBegin(GL_QUADS);

            //Aktuellen Frame aus dem png auswählen und anhand der Eckpunkte auf Texturgröße skalieren

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
