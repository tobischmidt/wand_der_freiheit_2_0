#ifndef VERFOLGER_H_INCLUDED
#define VERFOLGER_H_INCLUDED

#include "ofMain.h"
#include "ball.h"

class Verfolger : public Ball{

    public:

        Verfolger(ofPoint, float, float, float);
        void update(float, ofPoint);
        void draw();
        void drawEnd(int);

        void setPos(ofPoint);  //Position setzen
        void setSpeed(float);  //fluggeschwindigkeit �ndern

        //texturgr��e �ndern
        void setTexturWidth(float);
        void setTexturHeight(float);

        void setPar1(float);  //Par1 �ndern
        void setRangeWidth(float);  //Flugbereich �ndern

        void newAbweichung();  //Abweichung in Flugrichtung einbringen
        void resetEnd();  //Umwandlungsanimation in Stacheldraht zur�cksetzen

        float getSpeed();  //Fluggeschwindigkeit abfragen

    private:

        ofPoint saved_move_to;  //alter Punkt, zu dem sich bewegt werden soll
        float flightAngle;  //Ausrichtung der Textur in Flugrichtung
        float rangeWidth;

        float texturWidth;  //Texturbreite
        float texturHeight;  //Texturh�he
        float speed;  //Fluggeschwindigkeit
        float par1;  //Parameter zur Orientierung in alte Bewegungsrichtung

        ofPoint frame;  //Frame der Fluganimation

        int frameCounterX;  //Spalte im png der Fluganimation
        int frameCounterY;  //Zeile im png der Fluganimation

        int frameCounterEndX;  //Spalte im png der Verwandlungsanimation
        int frameCounterEndY;  //Spalte im png der Verwandlungsanimation

        //Position, an der der Vogel gezeichnet wird
        int drawPosX;
        int drawPosY;

        ofPoint abweichung;  //Abweichung vom eigentlichen Punkt, zu dem sich hinbewegt werden soll
};

#endif
