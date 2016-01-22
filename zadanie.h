#ifndef ZADANIE_H
#define ZADANIE_H


#include "coordinates.h"

using namespace std;

class zadanie
{
public:
    zadanie();

    zadanie(int p, int s);

    int id_robota;
    int id_wyswietlane; //określa jako otóre pojawiło się zadanie, nie odpowiada jego adresowi
    int numerPolki;
    int numerStanowiska;
    int stan;  //0 - nieaktywne/puste
               //1 - oczekujące na wysłanie agenta
               //2 - agent jedzie do pułki
               //3 - agent jedzie z pułką do przeładunku
               //4 - agent jedzie z pułką do jej bazowej pozycji
               //5 - agent wraca do wyjazdu



};
#endif // ZADANIE_H
