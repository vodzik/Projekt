#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

#include "refreasher.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void rysuj();  //funkcja rysująca
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    refreasher *refreash; //obiekt inicjalizujący wątek zegara odświerzającego

public slots:

    void onTick();  // clot wylapujący stgnał zegara odświerzającego

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
