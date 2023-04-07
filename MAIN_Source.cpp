// MAIN_IsingModel.cpp

#include <iostream>
#include "IsingSystem.h"

using namespace std;
// this is a global pointer, which is how we access the system itself
IsingSystem *sys;

void instance(float b,int N){
    // this is the seed for the random numbers
    int seed = 6;
    // This will be used to run sweeps of the system
    int count = 0;
    // Initialising system
    sys = new IsingSystem();
    sys->inverseTemperatureBeta = b;
    cout << "inverse beta: " << sys->inverseTemperatureBeta << endl;
    sys->setSeed(seed);
    cout << "setting seed " << seed << endl;
    sys->isActive = 1;
    //Runs N sweeps updating E and M arrays
    while (count < N) {
        sys->Update();
        count += 1;
    }
    //Outputting Energy and Magnetisation to file
    stringstream title;
    title << "data/" << b << ".csv";
    ofstream runOutput(title.str());
    // iterates through Energy to write to file
    runOutput << "Energy, Magnetisation" << endl;
    for (int i = 0; i < int(sys->Energy.size()); i++){
        runOutput << sys->Energy[i] << "," << sys->Magnetisation[i] << endl;
    }
    runOutput.close();
    delete sys;
}


int main() {
    //variables required to read setup
    string values, word;
    int column, N;
    float b;

    //opens setup file
    ifstream ReadFile("setup.csv");

    // iterates through lines in setup file
    while (getline(ReadFile, values)) {
        cout << "parameters: " << values << endl;
        std::stringstream s(values);
        column = 0;

        //reads through line to get beta and N
        while (getline(s, word, ',')) {
            if (column == 0) {
                b = stof(word);
            } else if (column == 1) {
                N = stoi(word);
            }
            column += 1;
        }
        // runs instance with parameters b,N
        instance(b, N);
    }
    ReadFile.close();
    cout << "the end" << endl;
    return 0;
}