#ifndef ESCOPETA_H
#define ESCOPETA_H

#include "Usable.h"

class Escopeta: public Usable{
    public:
        Escopeta(Spawner* expo, int modelo,b2Vec2 pos);
        ~Escopeta();
        void usar();
};

#endif
