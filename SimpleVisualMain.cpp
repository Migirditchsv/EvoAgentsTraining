#include "random.h"
#include "VisualAgent.h"

// Global constants

const double StepSize = 0.1;
const double RunDuration = 200;
const long RandomSeed = 1; 


// The main program

int main(int argc, char* argv[])
{
    VisualAgent Agent;
    Line Object;

    // Load the CTRNN into the agent
    char fname[] = "categorize.ns";
    ifstream ifs;
    ifs.open(fname);
    if (!ifs) {
        cerr << "File not found: " << fname << endl;
        exit(EXIT_FAILURE);
    }
    ifs >> Agent.NervousSystem; 
    
    // Run the agent
    SetRandomSeed(RandomSeed);
    Agent.Reset(0, 0);
    Agent.SetPositionX(-50);
    Object.SetPositionX(0);
    Object.SetPositionY(275);

    for (double t = 0; Object.PositionY() > BodySize/2; t += StepSize) {
        cout << Agent.PositionX() << " " << Agent.PositionY() << " ";
        cout << Object.PositionX() << " " << Object.PositionY() << " ";
        Agent.Step(StepSize, Object);
        Object.Step(StepSize);
    }

    return 0;
}