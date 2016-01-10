#include "robot.h"

Robot::Robot()
{
    rezerwacja_1.active=0;
    rezerwacja_2.active=0;
    polorzenie.active=0;
    pracuje=0;
    przeladunek=0;
    zaladowany=0;

}

void Robot::replace()
{
    polorzenie.X=rezerwacja_1.X;
    polorzenie.Y=rezerwacja_1.Y;
    polorzenie.active=rezerwacja_1.active;
    rezerwacja_1.X=rezerwacja_2.X;
    rezerwacja_1.Y=rezerwacja_2.Y;
    rezerwacja_1.active=rezerwacja_2.active;
    rezerwacja_2.active=0;
}

coordinates Robot::najdalszy_zaplanowany()
{
    if(rezerwacja_2.active)return rezerwacja_2;
    if(rezerwacja_1.active)return rezerwacja_1;
    return polorzenie;
}

bool Robot::canorder()
{
    if(rezerwacja_2.active)return 0;
    return 1;
}

void Robot::addorder(coordinates order)
{
    if(rezerwacja_1.active==0)
    {
        rezerwacja_1.X=order.X;
        rezerwacja_1.Y=order.Y;
        rezerwacja_1.active=1;
    }
    else
    {
        rezerwacja_2.X=order.X;
        rezerwacja_2.Y=order.Y;
        rezerwacja_2.active=1;
    }
}

bool Robot::cango()
{
    if((rezerwacja_1.active==1)&&(pracuje==0)) return 1;
    return 0;
}
