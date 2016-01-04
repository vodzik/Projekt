#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

#include "refreasher.h"
#include "sterownik.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    int stan[21][26];  //tablica przechowująca stan do wyswietlania
    void rysuj();  //funkcja rysująca
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    refreasher *refreash; //obiekt inicjalizujący wątek zegara odświerzającego
    sterownik *driver; //obiekt inicjalizujący wątek sterownika

signals:

    void WyslijZadanie(int, int);  //sygnał wysyłający zadanie

public slots:

    void onTick();  // slot wylapujący sygnał zegara odświerzającego
    void Odbierzstan(int**); //slot wdbierający stan planszy

private slots:
    void on_pushButton_clicked();

private:

    //inicjalizacja linków do grafik
    QPixmap mpusty;
    QPixmap mzarezerwowany;
    QPixmap mwozek;
    QPixmap mpolka;
    QPixmap mwozekpolka;
    QPixmap mwozekpolkadok;


    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
