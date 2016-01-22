#include "shelf.h"

shelf::shelf()
{


}

bool shelf::CzyNaMiejscu()
{
    if((polorzenie_aktualne.X==polorzenie_bazowe.X)&&(polorzenie_aktualne.Y==polorzenie_bazowe.Y))return 1;
    else return 0;
}

int shelf::AlejkaBazowa()
{
    if(polorzenie_bazowe.Y<=3)return 2;
    if(polorzenie_bazowe.Y<=6)return 5;
    if(polorzenie_bazowe.Y<=9)return 8;
    if(polorzenie_bazowe.Y<=12)return 11;
    return 14;
}
