#include "Usable.h"
#include "World.h"

#define TIME2DESTROY 3000
Usable::Usable(Spawner* expo, b2Vec2 pos):Cogible(expo,pos){
    timerCadencia = IrrMngr::Inst()->getTimer();
}
void Usable::actualiza(){
    Cogible::actualiza();
    if(!usos)CompruebaVida();
}
void Usable::CompruebaVida(){
    if(!cogido){
        if(!killCountDown){
            timer2Kill = IrrMngr::Inst()->getTimer();
            time2Kill = timer2Kill->getTime();
            killCountDown = true;
        }
        else if(IrrMngr::Inst()->getTime()-time2Kill>TIME2DESTROY){
            autoDestruir = true;
        }
    }
}
int Usable::getIdCogible(){};
