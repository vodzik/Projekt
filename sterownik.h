#ifndef STEROWNIK_H
#define STEROWNIK_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QtCore>
#include <QDebug>
#include "shelf.h"


class sterownik : public QThread
{
    Q_OBJECT
public:
    sterownik(QWidget *parent);  //konstruktor sterownika

    shelf *polki;    //wektor polek

    int iloscpolek; //dlogosc wektora polek

    void run(); //glowna petla watku

    int **stan;  //macierz stanu przesylana do wizualizacji;

signals:
    void Wyslijstan(int**);  //sygnal wysylajacy stan do wizualizacji

public slots:

private:
    void InicjalizujWektorPolek();    //Przypisuje wszystkim polką stany poczatkowe
    void OdswierzMacierzStanu();      //odswierza macierz stanu na podstawie polorzenia polek (i w przyszlosci roborów i zarezerwowanych pól)
    void InicjalizujMacierzStanu();   //alokuje pamięc dla macierzy stanu
};

#endif // STEROWNIK_H
