#ifndef AI_H_
#define AI_H_

#include "State.h"
#include "MainCar.h"
#include "AdversaryCar.h"
#include "DumbCar.h"
#include "Pedestrian.h"

// Location of entities in the simulation world
// Not sure how percise the simulation world locations
// can be, so using floats.

class AI {
public:
     // Generate a state based on information from the simulation
     State *generateState(void *info);

private:
     MainCar m_mainCar;
     AdversaryCar m_adversaryCar;
     DumbCar m_dumbCar; // One AI to control all dumb cars? Or separate AIs?
     // DumbCar *m_dumbCar;
     Pedestrian m_pedestrian; // One AI to control all pedestrians? Or separate AIs?
     // Pedestrian *m_pedestrians;
};

#endif /* AI_H_ */
