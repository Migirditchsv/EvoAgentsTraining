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
#include "random.h"
#include "TSearch.h"
#include <math.h>


// Control Pannel
// Neuronal controls
const double NEURON_GAIN     = 0.0;
const int    RUN_DURATION    = 250; // be reasonable now, make sure these divide nice
const double STEP_SIZE       = 0.01;
const int    STEP_NUM        = ceil(1+ (RUN_DURATION / STEP_SIZE) );
// Search controls
const double POPULATION_SIZE = 50;
const int    MAX_GENERATIONS = 200; 
const double VARIANCE        = 0.1;

// Global objects oh wow these are probably a bad idea
CTRNN c(2);

double Evaluate(TVector<double> &v, RandomState &)
{
    // Warm up
    double N1TimeSeries[STEP_NUM];
    double N2TimeSeries[STEP_NUM];
    int    TimeIndex = 0;

    // Map search Vectors
    double w11 = MapSearchParameter( v[1], -10,10 );
    double w12 = MapSearchParameter( v[2], -10,10 );
    double w21 = MapSearchParameter( v[3], -10,10 );
    double w22 = MapSearchParameter( v[4], -10,10 );
    double b1  = MapSearchParameter( v[5], -10,10 );
    double b2  = MapSearchParameter( v[6], -10,10 );
    double g1  = MapSearchParameter( v[7], -10,10 );
    double g2  = MapSearchParameter( v[8], -10,10 );
    // Okay wow that's a very very large parameter space to search
    // lets see how well this works. 

    // Set up the circuit
    c.SetConnectionWeight( 1, 1, w11 );
    c.SetConnectionWeight( 1, 2, w12 );
    c.SetConnectionWeight( 2, 1, w21 );
    c.SetConnectionWeight( 2, 2, w22 );
    c.SetNeuronBias( 1, b1 ); 
    c.SetNeuronBias( 2, b2 );
    c.SetNeuronGain( 1, g1 );
    c.SetNeuronGain( 2, g2 );


    // Run the circuit
    c.RandomizeCircuitState(-0.5,0.5);
    N1TimeSeries[0] = c.NeuronOutput(1);
    N2TimeSeries[0] = c.NeuronOutput(2);
    cout << "Simulating Network" << endl;
    for (double time = STEP_SIZE; time <= RUN_DURATION; time += STEP_SIZE) {
        //cout<<"Attempting step to time:"<<time<<endl;
        TimeIndex += 1;
        c.EulerStep(STEP_SIZE);
        //cout<<"Step complete."<<endl;
        if( fmod(time, 1000*STEP_SIZE ) ==0 )
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
  for (double time =0.0; time <= RUN_DURATION; time+=STEP_SIZE) 
  {
      c.EulerStep(STEP_SIZE);
      cout<<c.NeuronOutput(1)<<","<<c.NeuronOutput(2)<<endl;
      dataFile <<c.NeuronOutput(1)<<","<<c.NeuronOutput(2)<<"\n"; 
  }
  return 0;
}
