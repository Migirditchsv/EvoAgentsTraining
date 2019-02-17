//****************************************************************
//A program to evolve a 2 neuron oscilator from known parameters
//
//Sam
//02/12/19: Restarted
//
//To Do:
//1) evlolve one param
//***************************************************************

// Libs
#include "CTRNN.h" // global, eval(), evalFunc(), 
#include "random.h"
#include "TSearch.h"
#include <math.h> // eval()
#include <fstream> // output()

//*****************************************************************************
// Control Pannel 
//*****************************************************************************

// Neuronal controls
const int    TRANSIENT_STEPS = 5000;
const int    RUN_DURATION    = 250; // make sure these divide
const double STEP_SIZE       = 0.01;
const int    STEP_NUM        = ceil(1+ (RUN_DURATION / STEP_SIZE) );
// Search controls
const double POPULATION_SIZE = 500;
const int    MAX_GENERATIONS = 100; 
const double VARIANCE        = 0.15;
const double TERM_LIMIT      = 0.45;
// Metric controls
const int BOX_RES            = 10;



//*****************************************************************************
// Support  Functions
//*****************************************************************************

// Simulate CTRNN
void simulate(TVector<double> &v, double* N1TS, double* N2TS)
{
    // CTRNN Init and Set Up
    CTRNN c(2);

    double w11 = MapSearchParameter( v[1],-10,10);
    double w12 = MapSearchParameter( v[2], -10,10);
    double w21 = MapSearchParameter( v[2], -10,10);
    double w22 = 4.5; //MapSearchParameter( v[2], -10,10);
    double b1  =  -2.75; //MapSearchParameter( v[2], -10,10);
    double b2  =  -1.75; //MapSearchParameter( v[2], -10,10);
  //double g1  =  //MapSearchParameter( v[2], -10,10);
  //double g2  =  //MapSearchParameter( v[2], -10,10);
  
    c.SetConnectionWeight( 1, 1, w11 );
    c.SetConnectionWeight( 1, 2, w12 );
    c.SetConnectionWeight( 2, 1, w21 );
    c.SetConnectionWeight( 2, 2, w22 );
    c.SetNeuronBias( 1, b1 ); 
    c.SetNeuronBias( 2, b2 );
  //c.SetNeuronGain( 1, g1 );
  //c.SetNeuronGain( 2, g2 );

    // Turn the crank
    c.RandomizeCircuitState(-0.5,0.5);
    for( int t=0; t<STEP_NUM; t+=1  )
    {
        c.EulerStep(STEP_SIZE);
        N1TS[t] = c.NeuronState(1);
        N2TS[t] = c.NeuronState(2);
    }
 }  

// Terminate if good
int terminator(int generation,
               double bestPerf,
               double avgPerf,
               double perfVar)
{
    int condition = bestPerf > TERM_LIMIT * BOX_RES * BOX_RES;
    return( condition );
}

// Write out an individual
void WriteOut(TVector<double> &v)
{
    // Allocate time series space 
    double N1[STEP_NUM];
    double N2[STEP_NUM];
    
    // Simulate Individual
    simulate(v, N1, N2);
     
    //Safe and Sane file IO  
    system("rm ./data.txt");
    ofstream dataFile;
    dataFile.open("data.txt");
    //Write loop
    for (int t =0; t < STEP_NUM; t+=1) 
    {
        dataFile<<N1[t]<<","<<N2[t]<<endl;
    }
    //Always close out!
    dataFile.close();
}

double metric(double* N1, double* N2)
{
    // Init vars
    int boardScore = 0;
    int L1, L2, boxIndx;
    double maxVal = 0.0;
    int offset = 0;
    
    // Find shift so bottom left corner on origin
    for (int t =TRANSIENT_STEPS; t < STEP_NUM; t+=1)
    {
        if(abs(N1[t])>maxVal){maxVal=N1[t];}
        if(abs(N2[t])>maxVal){maxVal=N2[t];}
    }
    offset = ceil(maxVal);
    
    double inverseGridWidth = (double) BOX_RES / (double)(2.0*offset);
    int boardSize = pow( BOX_RES , 2 );

    //Declare state space array
    int* boxScore[boardSize];
   
    // N# values in [-1,1], scale to boxnum then floor
    for (int t=TRANSIENT_STEPS; t<STEP_NUM; t+=1) 
    {
       // Discretize on axis
        L1 = floor( ( N1[t] + maxVal ) / inverseGridWidth );
        L2 = floor( ( N2[t] + maxVal ) / inverseGridWidth );
        
        // raster index bottom left is 0 top right is (boxnum-1)^2 
        boxIndx = offset*L1 + L2;

        // Check if new box, ifso tally, and update visited boxes
        if(boxScore[boxIndx]==0){boardScore++;}
        boxScore[boxIndx]+=1;
    }
    // Exit returning score
    return(boardScore);
}

// Evaluate a single individual
double Evaluate(TVector<double> &v, RandomState &rs)
{
    // Declare neuron time series arrays
    double N1[STEP_NUM];
    double N2[STEP_NUM];
    double score;

    //Simulate system behaivor
    //cout<<"Simulateing individual"<<endl;
    simulate(v, N1, N2);

    // Apply metric
    //cout<<"Applying metric"<<endl;
    score = metric(N1, N2);

    //Map parameters
    return( score );
}



//*****************************************************************************
// Main
//*****************************************************************************
int main()
{
    // Init static neuron properites
    CTRNN c(2);
    c.SetNeuronBias(1, -2.75);
    c.SetNeuronBias(2, -1.75);
    //c.SetConnectionWeight(1, 1, 4.5);
    //c.SetConnectionWeight(1, 2, -1);
    //c.SetConnectionWeight(2, 1, 1);
    //c.SetConnectionWeight(2, 2, 4.5);
    cout<<"Brain Initialized"<<endl;

    // Configure the search
    TSearch s(3);
    s.SetRandomSeed(87635455);
    s.SetEvaluationFunction(Evaluate);
    s.SetSelectionMode(RANK_BASED);
    s.SetReproductionMode(HILL_CLIMBING);
    s.SetPopulationSize( POPULATION_SIZE );
    s.SetMaxGenerations( MAX_GENERATIONS );
    s.SetMutationVariance( VARIANCE );
    s.SetCrossoverProbability(0.0);
    //SVM: attempting to group points 
    //s.SetCrossoverTemplate( TempVec );
    //s.SetCrossoverMode(TWO_POINT);
    s.SetMaxExpectedOffspring(1.1);
    s.SetElitistFraction(0.1);
    s.SetSearchConstraint(1);
    //s.SetCheckpointInterval(5);
    s.SetSearchTerminationFunction(terminator);
    cout<<"Search Parameters Set"<<endl;
    
    // Run the search
    s.ExecuteSearch();
    cout<<"Search Executed"<<endl;
 
    // writeout best individual
    WriteOut(s.BestIndividual());
    cout<<"Best Individual Written to File"<<endl;

    // Plot results w/ Python plotter.py
    // Plot it w/ python
  system("python plotter.py");

    return(0);
}
