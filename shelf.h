#ifndef SHELF_H
#define SHELF_H

#include "coordinates.h"
class shelf
{
public:
    int id;
    coordinates polorzenie_aktualne;
    coordinates polorzenie_bazowe;
    bool zarezerwowana;
    shelf();

    bool CzyNaMiejscu();
    int AlejkaBazowa();


};


#endif // SHELF_H
