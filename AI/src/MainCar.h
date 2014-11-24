#ifndef MAINCAR_H_
#define MAINCAR_H_

#include "State.h"
#include "Agent.h"

class MainCar : public Agent {
public:
     Action getAction(State *state);
};

#endif /* MAINCAR_H_ */
