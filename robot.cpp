#include "robot.h"

Robot::Robot(int i)
 : id(i){}

int Robot::get_id() { return id; }

int Robot::get_shelf() { return polka; }

bool Robot::set_shelf(int s) { polka = s; }
