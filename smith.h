#ifndef SMITH_H
#define SMITH_H
#include <QtCore>
#include <QtGlobal>
#include <QObject>
#include <QWidget>
#include <QThread>

class Smith :public QThread
{
    Q_OBJECT

public:
    int dt;
    int t;
    Smith(QThread *parent);
    void run();
    int smithy[20];
    void slot(int id_robota);


signals:
    void sygnal(int);  //sygnal który wybywa
    void clock(int,int);
};

#endif // SMITH_H
