// ***************************************
// A very simple example program for evolving
// an oscilating 2 neuron CTRNN
// Based on Randy's code
//
// By Sam Migirditch
// To Do:
//
// ***************************************

#include "CTRNN.h"
#include "TSearch.h"
#include <math.h>


// Control Pannel
// Neuronal controls
#define NEURON_GAIN   0.0
#define RUN_DURATION  250
#define STEP_SIZE     0.01
// Search controls
#define POPULATION_SIZE  50
#define MAX_GENERATIONS  200 
#define VARIANCE         0.1


double Evaluate(TVector<double> &v, RandomState &)
{
    // Warm up
    double N1TimeSeries[RUN_DURATION];
    double N2TimeSeries[RUN_DURATION];
    int    TimeIndex = 0;
    // Set up the circuit
    CTRNN c(2);
    c.SetConnectionWeight(1, 1, v[1]);
    c.SetConnectionWeight(1, 2, v[2]);
    c.SetConnectionWeight(2, 1, v[3]);
    c.SetConnectionWeight(2, 2, v[4]);
    c.SetNeuronBias(1, v[5]); 
    c.SetNeuronBias(2, v[6]);
    c.SetNeuronGain(1, NEURON_GAIN );
    c.SetNeuronGain(2, NEURON_GAIN );


    // Run the circuit
    c.RandomizeCircuitState(-0.5,0.5);
    N1TimeSeries[0] = c.NeuronOutput(1);
    N2TimeSeries[0] = c.NeuronOutput(2);
    cout << "Simulating Network" << endl;
    for (double time = STEP_SIZE; time <= RUN_DURATION; time += STEP_SIZE) {
        TimeIndex += 1;
        c.EulerStep(STEP_SIZE);
        if( fmod(time, 10*STEP_SIZE ) ==0 )
            {
            cout<<time<<" "<<c.NeuronOutput(1)<<" "<<c.NeuronOutput(2)<< endl;
            }
        N1TimeSeries[TimeIndex] = c.NeuronOutput(1);
        N2TimeSeries[TimeIndex] = c.NeuronOutput(2);
    }
    // For now evaluate on a simple objective: maximize N1 while mining N2
    double evaluation = abs(N1TimeSeries[100]/N2TimeSeries[100]);
    return( evaluation );
}

// The main program

int main (int argc, const char* argv[]) {
  TSearch s(2);
    
  // Configure the search
  s.SetRandomSeed(87632455);
  s.SetEvaluationFunction(Evaluate);
  s.SetSelectionMode(RANK_BASED);
  s.SetReproductionMode(HILL_CLIMBING);
  s.SetPopulationSize(5);//1000
  s.SetMaxGenerations(20);
  s.SetMutationVariance(0.1);
  s.SetCrossoverProbability(0.5);
  //SVM: attempting to group points.
  //s.SetCrossoverPoints( TempVec );
  //s.SetCrossoverTemplate( TempVec );
  s.SetCrossoverMode(TWO_POINT);
  s.SetMaxExpectedOffspring(1.1);
  s.SetElitistFraction(0.1);
  s.SetSearchConstraint(1);
  s.SetCheckpointInterval(5);
    
  // Run the search
  s.ExecuteSearch();

  // Write out best indivudual simulation


  // Display the best individual found
  cout<<"Best Individual Performance:"<<s.BestPerformance()<<endl;

  // Time loop
  c.RandomizeCircuitState(-0.5,0.5);
  cout<<c.NeuronOutput(1)<<" "<<c.NeuronOutput(2)<<endl;
  ofstream dataFile;
  dataFile.open("data.txt");
  for (double time =0.0; time <= runTime; time+=stepSize) {
      c.EulerStep(stepSize);
      cout<<c.NeuronOutput(1)<<","<<c.NeuronOutput(2)<<endl;
      dataFile <<c.NeuronOutput(1)<<","<<c.NeuronOutput(2)<<"\n"; 

  return 0;
}
