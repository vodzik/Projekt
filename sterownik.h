#ifndef STEROWNIK_H
#define STEROWNIK_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QtCore>
#include <QDebug>
#include <list>
#include <vector>
#include "shelf.h"
#include "zadanie.h"
#include "robot.h"


using namespace std;

class sterownik : public QThread
{
    Q_OBJECT
public:
    sterownik(QWidget *parent);  //konstruktor sterownika

    shelf *polki;    //wektor polek

    std::vector<Robot> robo_vector; // wektor zawiera roboty w systemie

    int iloscpolek; //dlogosc wektora polek

    void run(); //glowna petla watku

    int **stan;  //macierz stanu przesylana do wizualizacji;

    int ** mapa; // zmienna będzie zawierała mapę obszaru
                 // mapa jest wykorzystywana przy zdefiniowaniu listy kroków potrzebnych do wykonania zadania
    std::list<zadanie*> listaZadan;

signals:
    void Wyslijstan(int**);  //sygnal wysylajacy stan do wizualizacji


public slots:
    void OdbierzZadanie(int, int);  // pierwszy int - półka, drugi int - stanowisko

private:
    void InicjalizujWektorPolek();    //Przypisuje wszystkim polką stany poczatkowe
    void OdswierzMacierzStanu();      //odswierza macierz stanu na podstawie polorzenia polek (i w przyszlosci roborów i zarezerwowanych pól)
    void InicjalizujMacierzStanu();   //alokuje pamięc dla macierzy stanu
    coordinates AdresStanowsika(int); // zwraca adres stanowiska
};

#endif // STEROWNIK_H
