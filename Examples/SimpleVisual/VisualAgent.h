// ***********************************************************
//  A class for visual agents
//
//  Matthew Setzler 4/19/17
// ************************************************************

#include "CTRNN.h"
#include "VisualObject.h"

// Global constants

const double Pi = 3.1415926535897;
const double BodySize = 30.0; // diameter of agent
const double EnvWidth = 400.0;
const double MaxRayLength = 220.0;
const double InputGain = 10.0;
const double VisualAngle = Pi/6;
const double VelGain = 5;

// The VisualAgent class declaration

class VisualAgent {
	public:
		// The constructor 
		VisualAgent(double ix = 0.0, double iy = 0.0, int NumRays_ = 7) {
			NumRays = NumRays_;
			Rays.SetBounds(1, NumRays);
			Reset(ix,iy);
		};

		// The destructor
		~VisualAgent() {}

		// Accessors
		double PositionX() {return cx;};
		void SetPositionX(double newx);
		double PositionY() {return cy;};

		// Control
		void Reset(double ix, double iy, int randomize = 0);
    void Reset(RandomState &rs, double ix, double iy, int randomize);
		void Step(double StepSize, VisualObject &object);

		CTRNN NervousSystem;

	private:
		void ResetRays();

		int NumRays;
		double cx, cy, vx;
		TVector<Ray> Rays;
};