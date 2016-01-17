#ifndef SMITH_H
#define SMITH_H
#include <QtCore>
#include <QtGlobal>
#include <QObject>
#include <QWidget>
#include <QThread>
#include <QMutex>
#include <QDebug>

class Smith :public QThread
{
    Q_OBJECT

public:
    QMutex Mutex_time; //zabezpieczenie pamięci
    int dt;            //interwał czasowy w ms
    int t;             //ilość iteracji czasu
    Smith(QThread *parent);
    void run();        //gółwna pętla
    int smithy[20];    //tablica odliczająca symulująca procesy agentów

public slots:
    void slot1(int id_robota, int czas);    // slot inicjujący symulacje procesu agenta o podanym id


signals:
    void sygnal1(int);  //sygnał emitowany po zakończeniu symulacji procesu przez agenta o podanym id
    void clock(int,int);  //sygnał emitowany do sterownika po karzdym ticku zegara. Przesyła interwał czasowy w ms i ilość iteracji
};

#endif // SMITH_H
