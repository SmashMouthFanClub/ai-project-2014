#ifndef ADVERSARYCAR_H_
#define ADVERSARYCAR_H_

#include "State.h"
#include "Agent.h"

class AdversaryCar : public Agent {
public:
     Action getAction(State *state);
};

#endif /* ADVERSARYCAR_H_ */
