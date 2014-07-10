#ifndef CHEF_H_INCLUDED
#define CHEF_H_INCLUDED

#include "ofMain.h"
#include "ball.h"
#include "verfolger.h"

class Chef : public Ball{

    public:

        Chef(ofPoint, float, float, float);
        void update(float, ofPoint);
        void draw();

        void setSpeed(float);  //Fluggeschwindigkeit ändern

        //Texturgröße ändern
        void setTexturWidth(float);
        void setTexturHeight(float);

        void setPar1(float);  //par1 ändern
        void setRangeWidth(float);  //Flugbereich ändern

    private:

        ofPoint saved_move_to;  //alter Punkt, zu dem sich bewegt werden soll
        float flightAngle;  //Ausrichtung der Textur in Flugrichtung
        float rangeWidth;  //x-Koordinate des Flugbereichs

        float texturWidth;  //Texturbreite
        float texturHeight;  //Texturhöhe
        float speed;  //Fluggeschwindigkeit
        float par1;  //Parameter zur Orientierung in alte Bewegungsrichtung

        //Position, an der der Vogel gezeichnet wird
        int drawPosX;
        int drawPosY;

        ofPoint frame;  //Frame der Fluganimation

        int frameCounterX;  //Spalte im png der Fluganimation
        int frameCounterY;  //Zeile im png der Fluganimation
};


#endif
