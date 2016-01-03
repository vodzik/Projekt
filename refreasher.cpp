#include "refreasher.h"

refreasher::refreasher(QWidget *parent)
{

}

void refreasher::run()
{
    while(1)
    {
        emit Tick();        //emitowanie sygnału Tick inicjującego odswierzanie
        this->msleep(200);  //czekanie 200 ms
    }
}
