#ifndef ROBOT_H
#define ROBOT_H

#include "coordinates.h"
#include "shelf.h"


/**
 * @brief The Robot class
 *  Klasa reprezentuje robota kontrolowanego przez sterownik
 *
 */
class Robot
{
  /*!
  * \brief polorzenie
  * Akturalne połorzenie robota
  */
 coordinates polorzenie;


 /**
  * @brief polka
  * Numer półki która jest przworzona
  */
 int polka;

 /**
  * @brief id
  * Jednoznacznie okreslajacy numer robota
  */
 int id;

public:
    Robot(int i);
    int get_id(); // id robota
    int get_shelf(); // szawke ktora przywiuz na miejsce
    bool set_shelf(int s); // szawke która przewozi

};

#endif // ROBOT_H
