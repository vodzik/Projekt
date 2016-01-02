#ifndef REFREASHER_H
#define REFREASHER_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QtCore>

class refreasher : public QThread
{
    Q_OBJECT
public:
    refreasher(QWidget *parent);
    void run();    //funkcja run jest jakby mainem dla wątku. Wykonuje się po instrukcji KLASA->start()

signals:
    void Tick(); //sygnał inicjujący odświerzanie

public slots:
};

#endif // REFREASHER_H
