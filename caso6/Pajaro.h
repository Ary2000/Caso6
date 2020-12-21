#ifndef PAJARO_H
#define PAJARO_H

#include <vector>

class Pajaro{
    private:
        

    public:
        Pajaro(int id) { this->id = id; }

        int id;
        double maxVelocidad = 0;
        double minVelocidad = 99999;
        double totalDistancia = 0;
};

#endif