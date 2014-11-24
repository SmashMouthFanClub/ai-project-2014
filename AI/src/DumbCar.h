#ifndef DUMBCAR_H_
#define DUMBCAR_H_

#include "State.h"
#include "ReflexAgent.h"

class DumbCar : public ReflexAgent {
public:
     Action getAction(State *state);
};

#endif /* DUMBCAR_H_ */
