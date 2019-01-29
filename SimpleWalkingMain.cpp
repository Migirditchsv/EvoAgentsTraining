// *************************************************************
// main.cpp
// 
// Run a given CTRNN walker
//
// 8/27/94	Created
// 5/25/02  Revised for OS X
// *************************************************************

#include "LeggedAgent.h"
#include "CTRNN.h"
#include "random.h"

// Global constants
const double StepSize = 0.1;
const double RunDuration = 200;
const long RandomSeed = 1; 

// The main program
int main(int argc, char* argv[])
{
    LeggedAgent Insect;

    // Load the CTRNN into the agent
    char fname[] = "cpg.ns";
    ifstream ifs;
    ifs.open(fname);
    if (!ifs) {
        cerr << "File not found: " << fname << endl;
        exit(EXIT_FAILURE);
    }
    ifs >> Insect.NervousSystem; 
    
    // Run the agent
    SetRandomSeed(RandomSeed);
    Insect.Reset(0, 0, 0);
    for (double time = 0; time < RunDuration; time += StepSize) {
        Insect.Step(StepSize);
        cout << Insect.Leg.JointX << " " << Insect.Leg.JointY << " ";
        cout << Insect.Leg.FootX << " " << Insect.Leg.FootY << " ";
        cout << Insect.Leg.FootState << endl;
    }
    
    // Display the fitness
    cout << "Average velocity = " << Insect.cx/RunDuration << endl;

    // Finished
    return 0;
}

