#include "zadanie.h"

zadanie::zadanie(int p, int s, coordinates pxy, coordinates sxy)
{
    numerPolki = p;
    numerStanowiska = s;
    wpolrzednePolki.X = pxy.X;
    wpolrzednePolki.Y = pxy.Y;
    wspolrzedneStanowska.X = sxy.X;
    wspolrzedneStanowska.Y = sxy.Y;

}
