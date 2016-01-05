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
    Smith(QThread *parent);
    void run();
    int smithy[20];
    void slot(int id_robota);


signals:
    void sygnal(int);  //sygnal który wybywa
};

#endif // SMITH_H
