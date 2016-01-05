#ifndef ZADANIE_H
#define ZADANIE_H

#include <list>
#include "coordinates.h"
using namespace std;

class zadanie
{
public:
    zadanie();
    int liczbaKrokow;
    list<coordinates> krok;
};

#endif // ZADANIE_H
