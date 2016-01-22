#ifndef ROBOT_H
#define ROBOT_H

#include "coordinates.h"



class Robot
{

public:
coordinates polorzenie;      //aktualne polorzenie robota
coordinates rezerwacja_1;    //zarezerwowane polorzenie robota
coordinates rezerwacja_2;    //2 zarezerwowane polorzenie
bool pracuje;                //flaga informująca czy robot wykonuje jakąś czynność
bool przeladunek;            //flaga informująca o tym czy trwa przeladunel robota (rozumiany jako załadowanie lub rozładowanie pułki)
bool zaladowany;             //flaga informująca o tym czy robot ma załadowaną półkę
bool przetwarzanie;          //flaga informująca o tym czy ładunek robota jest przetwarzany na stanowisku
bool przetworzone;           //flaga informująca o tym czy ładunek został przetworzony (czyli czy może już wracać)
bool usuwanie;               //flaga mowiaca o tym że robot wychodzi z systemu
bool usun;                   //flaga mowiaca o tym że robot wyszedl z systemu i należy usunąć go z listy aktywnych
int id_polki;                //nr przypisanej półki
Robot();


void clear();                //czyści stan robota (w praktyce usuwa go)
void replace();              //zmiana pozycji robota na zerezerwowaną pozycje. kolejka rezerwacji orzechodzi na następną pozycje
coordinates najdalszy_zaplanowany();  //zwraca najdalszą zaplanowaną pozycję
bool canorder();             //funkcja informuje czy w kolejce fifo dla następnych pozycji robota jest miejsce na nową
void addorder(coordinates);  //dodaje kolejną pozycje do kolejki zarezerwowanych
bool cango();                //funkcja mówiąca o tym, czy są jakieś rezerwacje, na ktore robot może się przemieścić

};

#endif // ROBOT_H
