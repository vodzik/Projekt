#include "smith.h"


Smith::Smith(QThread *parent)
{
    dt=10;
    t=0;
    qsrand(QTime::currentTime().msec()); // inicjalizacja ciągu pseudolosowego;
    for(int i=0; i<20; i++)
    {
        smithy[i]=-1;
    }
}

void Smith::run()
{
    //tutaj piszę WSZYSTKO!!!! (co mam tu pisać)
    while(1)
    {
        this->msleep(dt);  //czekanie 1 ms
       Mutex_time.lock();
        t++;

        for(int i=0; i<20; i++) if(smithy[i]>=0)  smithy[i]= smithy[i]-1;
        for(int i=0; i<20; i++) if(smithy[i]==0)
        {
            emit sygnal1(i);

        }
        emit clock(t,dt);
        Mutex_time.unlock();
    }
}

void Smith::slot1(int id_robota)
{
    Mutex_time.lock();
    if(qrand()%2==0)
        smithy[id_robota]=(50+(qrand()%10));
    else
        smithy[id_robota]=(50-(qrand()%10));
    Mutex_time.unlock();

}
