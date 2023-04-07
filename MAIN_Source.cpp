// MAIN_IsingModel.cpp

#include <iostream>
#include "IsingSystem.h"

using namespace std;
// this is a global pointer, which is how we access the system itself
IsingSystem *sys;

int main() {


    sys = new IsingSystem();

    // this is the seed for the random numbers
    int seed = 6;
    int count = 0;
    cout << "setting seed " << seed << endl;
    cout << "inverse beta: " << sys->inverseTemperatureBeta << endl;
         sys->setSeed(seed);
    sys->isActive = 1;
    while (sys->isActive and count != 20) {
        sys->Update();
        count += 1;
    }

    cout << "the end" << endl;

    return 0;
}



