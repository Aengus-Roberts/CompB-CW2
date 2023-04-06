// MAIN_IsingModel.cpp
//

#include <iostream>
#include "IsingSystem.h"

using namespace std;
// this is a global pointer, which is how we access the system itself
IsingSystem *sys;

int main(int argc, char **argv) {


	sys = new IsingSystem(); //EDIT

	// this is the seed for the random numbers
	int seed = 6;
	cout << "setting seed " << seed << endl;
	sys->setSeed(seed);

	while(sys->isRunning()){
        sys->Update();
    }

	cout << "the end" << endl;
	cin.get();

	return 0;
}



