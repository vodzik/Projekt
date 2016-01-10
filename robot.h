#ifndef ROBOT_H
#define ROBOT_H

#include "coordinates.h"



class Robot
{

public:
coordinates polorzenie;
coordinates rezerwacja_1;
coordinates rezerwacja_2;
bool pracuje;
bool przeladunek;
bool zaladowany;
Robot();

void replace();
coordinates najdalszy_zaplanowany();
bool canorder();
void addorder(coordinates);
bool cango();

};

#endif // ROBOT_H
