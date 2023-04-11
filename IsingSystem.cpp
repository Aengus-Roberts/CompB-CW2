//
//  IsingSystem.cpp
//

#include "IsingSystem.h"

// constructor
IsingSystem::IsingSystem() {
    cout << "creating system, gridSize " << gridSize << endl;

    inverseTemperatureBeta = 1 / 4.0;
    isActive = 0;
    // Allocate memory for the grid, remember to free the memory in destructor
    //   the point here is that each row of the grid is an array
    //   the grid itself is an array of pointers, one for each row
    // Here we allocate the array of pointers
    grid = new int *[gridSize];
    // Now allocate the individual rows
    for (int i = 0; i < gridSize; i++) {
        grid[i] = new int[gridSize];
    }

    // this sets the temperature and initialises the spins grid
    Reset();
}

// resets object
void IsingSystem::Reset() {

    double initialTemp = 4.0;

    setTemperature(initialTemp);

    // set the grid to -1
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            // position is (i,j)
            int pos[2] = {i, j};
            // set this spin to state -1
            setGrid(pos, -1);
        }
    }
}

// destructor
IsingSystem::~IsingSystem() {
    // Close the file (if open)
    if (logfile.is_open())
        logfile.close();

    // Delete the grid
    // First we delete the individual rows
    for (int i = 0; i < gridSize; i++)
        delete[] grid[i];
    // Finally delete the array of pointers
    delete[] grid;
}

// returns Magnetisation of grid
float IsingSystem::getMagnetisation() {
    float M = 0;
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            M += grid[i][j];
        }
    }
    return (M / (gridSize * gridSize));
}

// returns Energy of grid
float IsingSystem::getEnergy() {
    float E = 0;
    int neighbour[2], current[2];

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            current[0] = i;
            current[1] = j;
            for (int k = 0; k < 4; k++) {
                setPosNeighbour(neighbour, current, k);
                E += grid[current[0]][current[1]] * grid[neighbour[0]][neighbour[1]];
            }
        }
    }
    return (-E);
}

std::vector<float> IsingSystem::getValues() {
    float E = 0, M = 0;
    std::vector<float> values;
    int neighbour[2], current[2];
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            current[0] = i;
            current[1] = j;
            M += grid[i][j];
            for (int k = 0; k < 4; k++) {
                setPosNeighbour(neighbour, current, k);
                E += grid[current[0]][current[1]] * grid[neighbour[0]][neighbour[1]];
            }
        }
    }
    values.push_back(M);
    values.push_back(E);
    return values;
}


// attempt N spin flips, where N is the number of spins
void IsingSystem::MCsweep() {
    for (int i = 0; i < gridSize * gridSize; i++)
        attemptSpinFlip();
}

// here we attempt to flip a spin and accept/reject with Metropolis rule
void IsingSystem::attemptSpinFlip() {
    int pos[2];

    // random site
    pos[0] = rgen.randomInt(gridSize);
    pos[1] = rgen.randomInt(gridSize);

    double hloc = computeLocalField(pos);

    double dE = 2.0 * hloc * readGrid(pos);
    if (dE < 0)
        flipSpin(pos);
    else if (rgen.random01() < exp(-dE))
        flipSpin(pos);

}

// NOTE: this returns the local field *divided by the temperature* (dimensionless quantity)
double IsingSystem::computeLocalField(int pos[]) {
    double result = 0.0;
    for (int i = 0; i < 4; i++) {
        int nborPos[2];
        setPosNeighbour(nborPos, pos, i);
        result += readGrid(nborPos);
    }
    result *= inverseTemperatureBeta;
    return result;
}

// set the value of a grid cell for a particular position
void IsingSystem::setGrid(int pos[], int val) {
    grid[pos[0]][pos[1]] = val;
}

// read the grid cell for a given position
int IsingSystem::readGrid(int pos[]) {
    return grid[pos[0]][pos[1]];
}

// read the grid cell for a given position
void IsingSystem::flipSpin(int pos[]) {
    grid[pos[0]][pos[1]] = -grid[pos[0]][pos[1]];
}


// send back the position of a neighbour of a given grid cell
// NOTE: we take care of periodic boundary conditions, also positions are integers now not doubles
void IsingSystem::setPosNeighbour(int setpos[], int pos[], int val) {
    switch (val) {
        case 0:
            setpos[0] = (pos[0] + 1) % gridSize;
            setpos[1] = pos[1];
            break;
        case 1:
            setpos[0] = (pos[0] - 1 + gridSize) % gridSize;
            setpos[1] = pos[1];
            break;
        case 2:
            setpos[0] = pos[0];
            setpos[1] = (pos[1] + 1) % gridSize;
            break;
        case 3:
            setpos[0] = pos[0];
            setpos[1] = (pos[1] - 1 + gridSize) % gridSize;
            break;
    }
}

// this is the update function which at the moment just does one mc sweep
void IsingSystem::Update() {
    MCsweep();
    vector<float> values;
    values = getValues();
    Magnetisation.push_back(values[0]);
    Energy.push_back(values[1]);
}

