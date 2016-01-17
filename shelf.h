#ifndef SHELF_H
#define SHELF_H

#include "coordinates.h"
class shelf
{
public:
    int id;                            //identyfikator
    coordinates polorzenie_aktualne;   //bierzace położenie
    coordinates polorzenie_bazowe;     //położenie domyślne
    bool zarezerwowana;                //czy jakiś robot jest już przypisany do półki?
    shelf();

    bool CzyNaMiejscu();               //czy półka jest w pozycji bazowej?
    int AlejkaBazowa();                //zwraca współżędną Y alejki przy ktorej jest pozycja bazowa półki


};


#endif // SHELF_H
