#ifndef ZADANIE_H
#define ZADANIE_H

#include <list>
#include "coordinates.h"
using namespace std;

class zadanie
{
public:
    zadanie(int p, int s, coordinates pxy, coordinates sxy);

    int liczbaKrokow;
    int numerPolki;
    int numerStanowiska;
    coordinates wpolrzednePolki;
    coordinates wspolrzedneStanowska;

    list<coordinates> krok;
};
#endif // ZADANIE_H
