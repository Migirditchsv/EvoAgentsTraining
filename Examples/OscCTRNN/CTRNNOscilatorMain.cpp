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
const int    TRANSIENT_STEPS = 2000;
const int    RUN_DURATION    = 250; // be reasonable now, make sure these divide nice
const double STEP_SIZE       = 0.01;
const int    STEP_NUM        = ceil(1+ (RUN_DURATION / STEP_SIZE) );
// Search controls
const double POPULATION_SIZE = 100;
const int    MAX_GENERATIONS = 80; 
const double VARIANCE        = 0.35;


// Write out best indivudual simulation
void WriteOut(TVector<double> &v)
{
  // CTRNN Init and Set Up

  CTRNN c(2);

  double w11 =  v[1];
  double w12 =  v[2];
  double w21 =  v[3];
  double w22 =  v[4];
  double b1  =  v[5];
  double b2  =  v[6];
  double g1  =  v[7];
  double g2  =  v[8];
  
  // Set up the circuit
  c.SetConnectionWeight( 1, 1, w11 );
  c.SetConnectionWeight( 1, 2, w12 );
  c.SetConnectionWeight( 2, 1, w21 );
  c.SetConnectionWeight( 2, 2, w22 );
  c.SetNeuronBias( 1, b1 ); 
  c.SetNeuronBias( 2, b2 );
  c.SetNeuronGain( 1, g1 );
  c.SetNeuronGain( 2, g2 );


   
  //Safe and Sane file IO  
  system("rm ./data.txt");
  ofstream dataFile;
  dataFile.open("data.txt");
  //Write loop
  for (int time =0; time <= STEP_NUM; time+=1) 
  {
      c.EulerStep(STEP_SIZE);
      dataFile <<c.NeuronOutput(1)<<","<<c.NeuronOutput(2)<<"\n"; 
  }
  //Always close out!
  dataFile.close();
}


// Evaluaiton function should maximize output for oscilitory behavior
double EvalFxn(double N1TS[], double N2TS[])
{
    
    double theta[ STEP_NUM-TRANSIENT_STEPS];
    double radius[ STEP_NUM-TRANSIENT_STEPS];
    double MeanN1 = 0.0;
    double MeanN2 = 0.0;
    double SumOmega = 0.0;
    double SumDRadius = 0.0;
    int    dt = min(30, TRANSIENT_STEPS);
    // Look for wiggly elipses.  
    theta[0] = atan( N1TS[0] / N2TS[0] );
    radius[0]= sqrt( pow(N1TS[0],2) + pow(N2TS[0],2) );
    for(int ti=TRANSIENT_STEPS; ti<STEP_NUM; ti += 1)
    {
//        cout<<"N1[ti="<<ti<<"]="<<N1TS[ti]<<endl;//DEBUG
        theta[ti] =0.0; //atan( N1TS[ti] / N2TS[ti] );
        //radius[ti]= sqrt( pow(N1TS[ti],2) + pow(N2TS[ti],2) );
        // compute dependants
        MeanN1 += N1TS[ti]; // //N1TS[ti];
        MeanN2 += N2TS[ti];
        SumOmega += sqrt( pow(N1TS[ti] - N1TS[ti-dt],2) +  pow(N2TS[ti] - N2TS[ti-dt],2) ); //theta[ti]-theta[ti-dt];//prefers CCW rotation but ok. 
        SumDRadius += abs( radius[ti]-radius[ti-dt] );
    }
    //build final value
    double MeanRad = sqrt(pow(MeanN1/STEP_NUM,2) + pow(MeanN2/STEP_NUM,2)) ;
    SumOmega /= STEP_NUM;
    SumDRadius /= STEP_NUM;
    
    return( SumOmega);}

// Use EvalFxn to evaluate the individual 
double Evaluate(TVector<double> &v, RandomState &)
{
    //init object
    CTRNN c(2);

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


    // Initialize the circuit
    c.RandomizeCircuitState(-0.5,0.5);
    N1TimeSeries[0] = c.NeuronOutput(1);
    N2TimeSeries[0] = c.NeuronOutput(2);
    for (double time = STEP_SIZE; time <= RUN_DURATION; time += STEP_SIZE) {
        TimeIndex += 1;
        c.EulerStep(STEP_SIZE);
        
//        if( fmod(time, 10000*STEP_SIZE ) ==0 )
//            {
//            cout<<time<<" "<<c.NeuronOutput(1)<<" "<<c.NeuronOutput(2)<< endl;
//            }
        N1TimeSeries[TimeIndex] = c.NeuronOutput(1);
        N2TimeSeries[TimeIndex] = c.NeuronOutput(2);
    }
    // evaluate  fittness 
    double evaluation = EvalFxn( N1TimeSeries, N2TimeSeries );
    return( evaluation );
}

// The main program

int main (int argc, const char* argv[]) {
  TSearch s(8);
    
  // Configure the search
  s.SetRandomSeed(87632455);
  s.SetEvaluationFunction(Evaluate);
  s.SetSelectionMode(RANK_BASED);
  s.SetReproductionMode(HILL_CLIMBING);
  s.SetPopulationSize(POPULATION_SIZE);
  s.SetMaxGenerations(MAX_GENERATIONS);
  s.SetMutationVariance(VARIANCE);
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

  // Write Best Individual to datafile.txt
  WriteOut(s.BestIndividual());
  
  // Plot it w/ python
  system("python plotter.py");
  return 0;
}
