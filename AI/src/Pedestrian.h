#ifndef PEDESTRIAN_H_
#define PEDESTRIAN_H_

#include "State.h"
#include "ReflexAgent.h"

class Pedestrian : public ReflexAgent {
public:
     Action getAction(State *state);
};

#endif /* PEDESTRIAN_H_ */
