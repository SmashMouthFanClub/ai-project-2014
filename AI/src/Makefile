# Build All
all: AdversaryCar.o AI.o Agent.o DumbCar.o MainCar.o Pedestrian.o ReflexAgent.o State.o

# Adversary car AI
AdversaryCar.o: State.o AdversaryCar.cpp

# Agent abtsract class
Agent.o: State.o

# AI <--> Simulation interface
AI.o: State.o AI.cpp

# Dumbcar AI
DumbCar.o: State.o DumbCar.cpp

# MainCar AI
MainCar.o: State.o MainCar.cpp

# Pedestrian AI
Pedestrian.o: State.o Pedestrian.cpp

# Reflex agent abtsract class
ReflexAgent.o: State.o ReflexAgent.cpp

# State representation of the simulation for the AI
State.o: State.cpp

# Clean for clean builds
clean:
	rm -f *.o *~ \#*
