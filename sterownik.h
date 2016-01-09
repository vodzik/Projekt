#ifndef STEROWNIK_H
#define STEROWNIK_H

#include <QMutex>

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QtCore>
#include <QDebug>
#include <QString>
//#include <vector>
#include "shelf.h"
#include "zadanie.h"
#include "robot.h"
#include "smith.h"
#include "coordinates.h"


using namespace std;

class sterownik : public QThread
{
    Q_OBJECT
public:
    sterownik(QWidget *parent);  //konstruktor sterownika

    Smith *agent;
    shelf *polki;    //wektor polek

    //std::vector<Robot> robo_vector; // wektor zawiera roboty w systemie

    coordinates tablicRobotow[20];
    bool RobotZajety[20];

    int t;

    int dt;

    int iloscpolek; //dlogosc wektora polek

    void run(); //glowna petla watku

    int **stan;  //macierz stanu przesylana do wizualizacji;

    int **mapa; // zmienna będzie zawierała mapę obszaru
                 // mapa jest wykorzystywana przy zdefiniowaniu listy kroków potrzebnych do wykonania zadania

    zadanie tablicazadan[100];

    int licznikzadan;
    int licznikzadanaktywnych;

    void WyslijZadania();

signals:
    void Wyslijstan(int**, double);  //sygnal wysylajacy stan do wizualizacji
    void WyslijLogi(QString);  //wysłanie logów
    void WyslijZadania(QString); //wysyła liste wszystkich zadań

public slots:
    void OdbierzZadanie(int, int);  // pierwszy int - półka, drugi int - stanowisko
    void OdbierzZegar(int, int);  // t,dt
    void OdbierzRefreshera();

private:
    QMutex Mutex_stan;
    QMutex Mutex_zadania;
    void InicjujStanRobotow();
    double time();
    void InicjalizujWektorPolek();    //Przypisuje wszystkim polką stany poczatkowe
    void OdswierzMacierzStanu();      //odswierza macierz stanu na podstawie polorzenia polek (i w przyszlosci roborów i zarezerwowanych pól)
    void InicjalizujMacierzStanu();   //alokuje pamięc dla macierzy stanu
    coordinates AdresStanowsika(int); // zwraca adres stanowiska
    void InicjujMacierzZadan();
    bool DodajZadanie(int,int);
    void UsunZadanie(int);
    void ObslurzZadania0();
    void ObslurzZadania1();
    void ObslurzZadania2();
    void ObslurzZadania3();
    void ObslurzZadania4();
    void ObslurzZadania5();
};

#endif // STEROWNIK_H
