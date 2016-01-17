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

    Smith *agent;    //watek symulatora akcji
    shelf *polki;    //wektor polek

    //std::vector<Robot> robo_vector; // wektor zawiera roboty w systemie

    Robot Roboty[20];

    bool flaga_nowych_zadan;   //daje znać czy nalezy przesłać liste zadań do qui

    int t;    //ilość iteracji czasu

    int dt;   //interwał czasu w ms

    int iloscpolek; //dlogosc wektora polek

    void run(); //glowna petla watku

    int **stan;  //macierz stanu przesylana do wizualizacji;

    int **mapa; // zmienna będzie zawierała mapę obszaru
                 // mapa jest wykorzystywana przy zdefiniowaniu listy kroków potrzebnych do wykonania zadania

    zadanie tablicazadan[1000]; //tablica wszystkich zadan

    int licznikzadan;           //licznik zadan
    int licznikzadanaktywnych;  //licznik zadan aktywnych

    int czas_rochu;
    int czas_zaladunku;
    int czas_obslogi;
    int czas_rozladunku;
    int czas_wyjscia;

    int kolejka[4]; //ilosc wozkow jadących do stanowisk


    void WyslijZadania();       //metoda inicjująca sygnał z listą zadań wysyłany do gui

signals:
    void Wyslijstan(int**, double);  //sygnal wysylajacy stan do wizualizacji
    void WyslijLogi(QString);  //wysłanie logów
    void WyslijZadania(QString); //wysyła liste wszystkich zadań
    void polecenie_dla_agenta(int,int);  //wysyłanie polecenia do symulatora akcji dla agenta o podanym id i podanym średnim czasie akcji


public slots:
    void OdbierzZadanie(int, int);  // pierwszy int - półka, drugi int - stanowisko
    void OdbierzZegar(int, int);  // t,dt
    void OdbierzRefreshera();     //inicjuje przesyłanie stanu do gui
    void sygnal_od_agenta(int);   //zdarzenia obserwowalne, czyli sygnały o zakończeniu akcji przez agenta o podanym id

private:
    QMutex Mutex_stan;
    QMutex Mutex_zadania;

    void InicjujStanRobotow(); //inicjujemy macierz robotow
    double time();             //funkcja zwraca czas symulacji z t i dt
    void InicjalizujWektorPolek();    //Przypisuje wszystkim polką stany poczatkowe
    void OdswierzMacierzStanu();      //odswierza macierz stanu na podstawie polorzenia polek (i w przyszlosci roborów i zarezerwowanych pól)
    void InicjalizujMacierzStanu();   //alokuje pamięc dla macierzy stanu
    coordinates AdresStanowsika(int); // zwraca adres stanowiska
    void InicjujMacierzZadan();       //inicjuje macierz zadan
    bool DodajZadanie(int,int);       //dodaje zadanie do macierzy
    void UsunZadanie(int);            //usuwa zadanie z macierzy i przesuwa kolejne zadania tak by zapełnić pustkę i utrzymać kolejność

    //obsluga zadan
    void ObslurzZadania1();  //oczekujące na realizacje
    void ObslurzZadania2();  //robot kieruje się do półki
    void ObslurzZadania3();  //robot jedzie z półka na przeładunek
    void ObslurzZadania4();  //robot wraca z przeładunku
    void ObslurzZadania5();  //robot kieruje się do wyjazdu z systemu
    void ObslurzUsuwanie();  //usuwanie zakonczonych zadan
};

#endif // STEROWNIK_H
