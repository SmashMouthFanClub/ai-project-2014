#ifndef AGENT_H_
#define AGENT_H_

// The action retruned will be an index into a the AI's action list
// Not sure if this is the best way to set this up, but each AI type
// will have a separate list of actions. For the sake of abastraction,
// an index makes sense.

typedef int Action;

class Agent {
public:
     // Returns an action based on the current state of the environment
     virtual Action getAction(State *state) = 0;
};

#endif /* AGENT_H_ */
