// *************************
// A class for legged agents
//
// RDB 2/16/96
// *************************

#pragma once

#include "CTRNN.h"

// Global constants

const double Pi = 3.1415926;


// The Leg class declaration

class TLeg {
	public:
		// The constructor
		TLeg() {};
		// The destructor
		~TLeg() {};
		
		// Accessors
		
		double Angle, Omega, ForwardForce, BackwardForce;
		double FootX, FootY, JointX, JointY;
		double FootState;
};


// The LeggedAgent class declaration

class LeggedAgent {
	public:
		// The constructor
		LeggedAgent(double ix = 0.0, double iy = 0.0)
		{
			Reset(ix,iy);
		};
		// The destructor
		~LeggedAgent() {};
		
		// Accessors
		double PositionX(void) {return cx;};
		void SetPositionX(double newx) {cx = newx;};
		
		// Control
    void Reset(double ix, double iy, int randomize = 0);
    void Reset(double ix, double iy, int randomize, RandomState &rs);
		void Step(double StepSize);
		void Step2(double StepSize);
		void Step1(double StepSize);
		void PerfectStep(double StepSize);
		
		double cx, cy, vx;
		TLeg Leg;
		CTRNN NervousSystem;
};