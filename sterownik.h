#ifndef STEROWNIK_H
#define STEROWNIK_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QtCore>

class sterownik : public QThread
{
    Q_OBJECT
public:
    sterownik(QWidget *parent);

    void run();

    int **stan;

signals:
    void Wyslijstan(int**);

public slots:
};

#endif // STEROWNIK_H
