#include "verfolger.h"


Verfolger::Verfolger(ofPoint _pos, float _speed/*NEW*/,float _texturWidth/*NEW*/, float _texturHeight/*NEW*/, float _rangeWidth/*NEW*/){

    pos = _pos;
    speed = _speed; /*NEW*/
    saved_move_to = ofPoint(0,0);
    rangeWidth = ofGetScreenWidth();
    texturWidth = _texturWidth;
    texturHeight = _texturHeight;
    frameCounter = 0;
}

void Verfolger::update(float dt, ofPoint move_to, float _speed/*NEW*/,float _texturWidth/*NEW*/, float _texturHeight/*NEW*/, float _par1/*NEW*/, float _rangeWidth/*new*/)
{
    par1=0.4;
    par2= 1- par1;

    if(_texturWidth)
    {
        texturWidth = _texturWidth * 0.8;/*NEW*/
    }

    if(_texturHeight)
    {
        texturHeight = _texturHeight * 0.8;   /*NEW*/
    }

    if(_speed)
    {
        speed = _speed;   /*NEW*/
    }

    if (_par1)
    {
        par1 = _par1;  /*NEW*/
        par2 = 1- par1;
    }

    if (_rangeWidth)
    {
        rangeWidth = _rangeWidth; /*NEW*/
    }


    // Wenn kein move_to mitgegeben wurde oder -1, dann dem letzten Punkt folgen, also kein Update durchführen.
    if(move_to.x >= 0){
        saved_move_to = move_to;
    }

    // steuert die bewegung vom Chef zur Maus - position= (aktuelle)position+richtung*geschwindigkeit*zeit
    dir = (saved_move_to - pos) * par1 + dir * par2;
    dir.normalize();
    pos += dir * speed * dt;

    // errechnet aus dem Richtungsvektor den Drehwinkel für die Flugrichtung
    flightAngle = ofVec2f(0, 1).angle(dir);
}

void Verfolger::draw(){

    int drawPosX = pos.x*ofGetWidth();
    int drawPosY = pos.y*ofGetHeight();

    // Bälle sollen den Bildschirm nicht verlassen
    if ((drawPosX > rangeWidth - dim)||(drawPosX < dim))
    {
        dir.x *= -1;
    }

    if ((drawPosY > ofGetHeight() - dim)||(drawPosY < 0 + dim))
    {
        dir.y *= -1;
    }

    ofSetColor(255);

    /*ofPushMatrix();

    ofTranslate(drawPosX, drawPosY, 0);
    ofRotateZ(flightAngle + 180);
    textur.setAnchorPoint(35, 20);
    textur.draw(0, 0, texturWidth, texturHeight);

    ofPopMatrix();*/

    //textur.getTextureReference().bind();

    switch (frameCounter)
    {
        case 0:
            frame = ofPoint(0,0);
            break;

        case 1:
            frame = ofPoint(676,0);
            break;

        case 2:
            frame = ofPoint(1352,0);
            break;

        case 3:
            frame = ofPoint(0,279);
            break;

        case 4:
            frame = ofPoint(676,279);
            break;

        case 5:
            frame = ofPoint(1352,279);
            break;

        case 6:
            frame = ofPoint(0,558);
            break;

        case 7:
            frame = ofPoint(676,558);
            break;

        case 8:
            frame = ofPoint(1352,558);
            break;

        case 9:
            frame = ofPoint(0,837);
            break;

        case 10:
            frame = ofPoint(676,837);
            break;

        case 11:
            frame = ofPoint(1352,837);
            break;
    }

    glPushMatrix();

        glTranslated(drawPosX, drawPosY, 0);
        glRotatef(flightAngle + 180, 0, 0, 1);
        glBegin(GL_QUADS);

            glTexCoord2f(frame.x, frame.y);
            glVertex2f(-texturWidth, -texturHeight);

            glTexCoord2f(frame.x + 676, frame.y);
            glVertex2f(texturWidth, -texturHeight);

            glTexCoord2f(frame.x + 676, frame.y + 279);
            glVertex2f(texturWidth, texturHeight);

            glTexCoord2f(frame.x, frame.y + 279);
            glVertex2f(-texturWidth, texturHeight);

            glEnd();

    glPopMatrix();

    frameCounter ++;

    if(frameCounter > 11)
    {
        frameCounter = 0;
    }

    //textur.getTextureReference().unbind();
}
