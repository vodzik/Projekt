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
    QMutex Mutex_time;
    int dt;
    int t;
    Smith(QThread *parent);
    void run();
    int smithy[20];

public slots:
    void slot1(int id_robota);


signals:
    void sygnal1(int);  //sygnal który wybywa
    void clock(int,int);
};

#endif // SMITH_H
