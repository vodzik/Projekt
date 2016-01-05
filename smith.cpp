#include "smith.h"
#include <QtCore>
#include <QDebug>
#include <QtGlobal>

Smith::Smith(QThread *parent)
{
    qsrand(QTime::currentTime().msec()); // inicjalizacja ciągu pseudolosowego;
    for(int i=0; i<20; i++)
    {
        smithy[i]=0;
    }
}

void Smith::run()
{
    //tutaj piszę WSZYSTKO!!!! (co mam tu pisać)

    while(1)
    {
        for(int i=0; i<20; i++)
            if(smithy[i]>0)
                smithy[i]= smithy[i]-1;
        this->msleep(1);  //czekanie 200 ms
        for(int i=0; i<20; i++)
            if(smithy[i]==0)
                emit sygnal(i);
        }
}

void Smith::slot(int id_robota)
{
    if(qrand()%2==0)
        smithy[id_robota]=(5000+(qrand()%1000));
    else
        smithy[id_robota]=(5000+(qrand()%1000));
}
