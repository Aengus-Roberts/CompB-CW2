#pragma once

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>


#include <math.h>
#include <random>
#include <string>
#include <sstream>

#include "rnd.h"

using namespace std;

// IsingSystem class
// this is where the Ising simulation happens
class IsingSystem {
private:
    // size of grid: "static const" means that this number is a fixed constant
    static const int gridSize = 40;
    int **grid;  // this will be a 2d array that stores whether each site is occupied

    // random number generator, class name is rnd, instance is rgen
    rnd rgen;

    // this an output file (in case we need one)
    ofstream logfile;

public:
    // constructor
    IsingSystem();

    // destructor
    ~IsingSystem();

    // this sets the seed for the random numbers
    void setSeed(int s) { rgen.setSeed(s); }

    // these functions are used to update the system
    double computeLocalField(int pos[]);

    void attemptSpinFlip();

    void MCsweep();

    // increase or decrease the temperature (arbitrary increments)
    void Hotter() { inverseTemperatureBeta -= 0.05; }

    void Colder() { inverseTemperatureBeta += 0.05; }

    // this variable is 1 if the system is running, 0 for paused
    int isActive;

    // this is the inverse temperature parameter beta
    double inverseTemperatureBeta;

    void setTemperature(double TT) { inverseTemperatureBeta = 1.0 / TT; }

    // set and read grid entries associated with a given position
    // NOTE! the grid is not accessed directly, only via these functions
    void setGrid(int pos[], int val);

    int readGrid(int pos[]);

    void flipSpin(int pos[]);

    float getMagnetisation();


    // reset temperature to default value and re-initialise the grid
    void Reset();

    // "user interface" for the isActive variable
    int isRunning() { return isActive; }


    // set setpos to the position of a neighbour of pos
    // the neighbour to use is determined by val (=0,1,2,3)
    // beware: behavior is not defined for val>=4 or val<0
    void setPosNeighbour(int setpos[], int pos[], int val);

    // update the system: makes a Monte Carlo sweep
    void Update();

};

