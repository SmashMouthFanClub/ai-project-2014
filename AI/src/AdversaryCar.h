#ifndef ADVERSARYCAR_H_
#define ADVERSARYCAR_H_

#include "AI.h"

class AdversaryCar : public Agent {
public:
     Action getAction(State *state);
};

#endif /* ADVERSARYCAR_H_ */
