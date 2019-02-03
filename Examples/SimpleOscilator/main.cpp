//****************************************************************
//A program to simulate a 2 neuron oscilator from known parameters
//
//Sam
//01/23/19: created
//
//To Do:
//1) Plot results
//***************************************************************

// Imports
#include "CTRNN.h"
#include <fstream>
// I don't understand why we dont' need <iostream> in the example...
// seems to be something about overloading cout?

// Global Constants
const double runTime = 300;
const double stepSize = 0.01;

// Main Simulation
int main()
{
    //Set parameters: Oscillate
    CTRNN c(2);
    c.SetNeuronBias(1, -2.75);
    c.SetNeuronBias(2, -1.75);
    c.SetConnectionWeight(1, 1, 4.5);
    c.SetConnectionWeight(1, 2, -1);
    c.SetConnectionWeight(2, 1, 1);
    c.SetConnectionWeight(2, 2, 4.5);

    // Time loop
    c.RandomizeCircuitState(-0.5,0.5);
    cout<<c.NeuronOutput(1)<<" "<<c.NeuronOutput(2)<<endl;
    ofstream dataFile;
    dataFile.open("data.txt");
    for (double time =0.0; time <= runTime; time+=stepSize) {
        c.EulerStep(stepSize);
        cout<<c.NeuronOutput(1)<<","<<c.NeuronOutput(2)<<endl;
        dataFile <<c.NeuronOutput(1)<<","<<c.NeuronOutput(2)<<"\n"; 
    }
    dataFile.close();
    //finished time loop
    return 0;
}
