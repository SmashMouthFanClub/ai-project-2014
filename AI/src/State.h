#ifndef STATE_H_
#define STATE_H_

typedef struct {
     float x;
     float y;
     float z;
} Location;

class State {
public:
     State();

     Location *mainCarLocation();
     Location *adversaryCarLocation();
     Location *parkingSpotLocation();
     Location *getPedestrianLocations();
     Location *getDumbCarLocations(); 

private:
     Location *m_mainCarLoc;
     Location *m_adversaryCarLoc;
     Location *m_parkingSpotLoc;
     Location *m_pedestrianLocs;
     Location *m_dumbCarLocs;

     // TODO: Decide how the simulation map should be handled as part of the state
     // A graph where each edge represents a road?
};

#endif /* STATE_H_ */
