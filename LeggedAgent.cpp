// ***********************
// Methods for LeggedAgent
//
// RDB 2/16/96
// ***********************

#include "LeggedAgent.h"
#include "random.h"

// Constants

const int    LegLength = 15;
const double MaxLegForce = 0.05;
const double ForwardAngleLimit = Pi/6;
const double BackwardAngleLimit = -Pi/6;
const double MaxVelocity = 6.0;
const double MaxTorque = 0.5;
const double MaxOmega = 1.0;


// *******
// Control
// *******

// Reset the state of the agent

void LeggedAgent::Reset(double ix, double iy, int randomize)
{
	cx = ix; cy = iy; vx = 0.0;
	Leg.FootState = 0;
	if (randomize) Leg.Angle = UniformRandom(BackwardAngleLimit,ForwardAngleLimit);
	else Leg.Angle = ForwardAngleLimit;
	Leg.Omega = Leg.ForwardForce = Leg.BackwardForce = 0.0;
	Leg.JointX = cx; Leg.JointY = cy + 12.5;
	Leg.FootX = Leg.JointX + LegLength * sin(Leg.Angle);
	Leg.FootY = Leg.JointY + LegLength * cos(Leg.Angle);
	if (randomize) NervousSystem.RandomizeCircuitState(-0.1,0.1);
	else NervousSystem.RandomizeCircuitState(0.0,0.0);	
}

void LeggedAgent::Reset(double ix, double iy, int randomize, RandomState &rs)
{
	cx = ix; cy = iy; vx = 0.0;
	Leg.FootState = 0;
	if (randomize) Leg.Angle = rs.UniformRandom(BackwardAngleLimit,ForwardAngleLimit);
	else Leg.Angle = ForwardAngleLimit;
	Leg.Omega = Leg.ForwardForce = Leg.BackwardForce = 0.0;
	Leg.JointX = cx; Leg.JointY = cy + 12.5;
	Leg.FootX = Leg.JointX + LegLength * sin(Leg.Angle);
	Leg.FootY = Leg.JointY + LegLength * cos(Leg.Angle);
	if (randomize) NervousSystem.RandomizeCircuitState(-0.1,0.1,rs);
	else NervousSystem.RandomizeCircuitState(0.0,0.0,rs);	
}


// Step the insect using a general CTRNN CPG

void LeggedAgent::Step(double StepSize)
{
	double force = 0.0;
	
	// Update the nervous system
	NervousSystem.EulerStep(StepSize);
	// Update the leg effectors
	if (NervousSystem.NeuronOutput(1) > 0.5) {Leg.FootState = 1; Leg.Omega = 0;}
	else Leg.FootState = 0;
	Leg.ForwardForce = NervousSystem.NeuronOutput(2) * MaxLegForce;
	Leg.BackwardForce = NervousSystem.NeuronOutput(3) * MaxLegForce;
	// Compute the force applied to the body
	// *** This is a CHANGE from the original body model that allows a supporting leg that has
	// *** passed outside of the mechanical limits to apply force in a direction that moves it
	// *** back toward that mechanical limit but not in a direction that would move it further
	// *** away.  In effect, the mechanical limits become 1-way constraints for a supporting leg.
	double f = Leg.ForwardForce - Leg.BackwardForce;
	if (Leg.FootState == 1.0)
		if ((Leg.Angle >= BackwardAngleLimit && Leg.Angle <= ForwardAngleLimit) || 
		    (Leg.Angle < BackwardAngleLimit && f < 0) || 
		    (Leg.Angle > ForwardAngleLimit && f > 0))
		force = f;
	// *** The original code
//		if (Leg.FootState == 1.0 && Leg.Angle >= BackwardAngleLimit &&  Leg.Angle <= ForwardAngleLimit)
//			force = Leg.ForwardForce - Leg.BackwardForce;
	// ***
	// Update the position of the body
	vx = vx + StepSize * force;
	if (vx < -MaxVelocity) vx = -MaxVelocity;
	if (vx > MaxVelocity) vx = MaxVelocity;
	cx = cx + StepSize * vx;
	// Update the leg geometry
	Leg.JointX = Leg.JointX + StepSize * vx;
	if (Leg.FootState == 1.0) {
		double angle = atan2(Leg.FootX - Leg.JointX,Leg.FootY - Leg.JointY);
		Leg.Omega = (angle - Leg.Angle)/StepSize;
		Leg.Angle = angle;
	}
	else {
		vx = 0.0;
		Leg.Omega	= Leg.Omega + StepSize * MaxTorque * (Leg.BackwardForce - Leg.ForwardForce);
		if (Leg.Omega < -MaxOmega) Leg.Omega = -MaxOmega;
		if (Leg.Omega > MaxOmega) Leg.Omega = MaxOmega;
		Leg.Angle = Leg.Angle + StepSize * Leg.Omega;
		if (Leg.Angle < BackwardAngleLimit) {Leg.Angle = BackwardAngleLimit; Leg.Omega = 0;}
		if (Leg.Angle > ForwardAngleLimit) {Leg.Angle = ForwardAngleLimit; Leg.Omega = 0;}
		Leg.FootX = Leg.JointX + LegLength * sin(Leg.Angle);
		Leg.FootY = Leg.JointY + LegLength * cos(Leg.Angle);
	}
	// If the foot is too far back, the body becomes "unstable" and forward motion ceases
	if (cx - Leg.FootX > 20) vx = 0.0;
}


// Step the LeggedAgent using a 2-neuron CTRNN CPG

void LeggedAgent::Step2(double StepSize)
{
	double force = 0.0;
	
	// Update the nervous system
	NervousSystem.EulerStep(StepSize);
	// Update the leg effectors
	if (NervousSystem.NeuronOutput(1) > 0.5) {Leg.FootState = 1; Leg.Omega = 0;}
	else Leg.FootState = 0;
	Leg.ForwardForce = NervousSystem.NeuronOutput(1) * MaxLegForce;
	Leg.BackwardForce = NervousSystem.NeuronOutput(2) * MaxLegForce;
    double f = Leg.ForwardForce - Leg.BackwardForce;
	if (Leg.FootState == 1.0)
		if ((Leg.Angle >= BackwardAngleLimit && Leg.Angle <= ForwardAngleLimit) || 
		    (Leg.Angle < BackwardAngleLimit && f < 0) || 
		    (Leg.Angle > ForwardAngleLimit && f > 0))
		force = f;
	// Update the position of the body
	vx = vx + StepSize * force;
	if (vx < -MaxVelocity) vx = -MaxVelocity;
	if (vx > MaxVelocity) vx = MaxVelocity;
	cx = cx + StepSize * vx;
	// Update the leg geometry
	Leg.JointX = Leg.JointX + StepSize * vx;
	if (Leg.FootState == 1.0) {
		double angle = atan2(Leg.FootX - Leg.JointX,Leg.FootY - Leg.JointY);
		Leg.Omega = (angle - Leg.Angle)/StepSize;
		Leg.Angle = angle;
	}
	else {
		vx = 0.0;
		Leg.Omega	= Leg.Omega + StepSize * MaxTorque * (Leg.BackwardForce - Leg.ForwardForce);
		if (Leg.Omega < -MaxOmega) Leg.Omega = -MaxOmega;
		if (Leg.Omega > MaxOmega) Leg.Omega = MaxOmega;
		Leg.Angle = Leg.Angle + StepSize * Leg.Omega;
		if (Leg.Angle < BackwardAngleLimit) {Leg.Angle = BackwardAngleLimit; Leg.Omega = 0;}
		if (Leg.Angle > ForwardAngleLimit) {Leg.Angle = ForwardAngleLimit; Leg.Omega = 0;}
		Leg.FootX = Leg.JointX + LegLength * sin(Leg.Angle);
		Leg.FootY = Leg.JointY + LegLength * cos(Leg.Angle);
	}
	// If the foot is too far back, the body becomes "unstable" and forward motion ceases
	if (cx - Leg.FootX > 20) vx = 0.0;
}


// Step the LeggedAgent using a 1-neuron CTRNN CPG

void LeggedAgent::Step1(double StepSize)
{
	double force = 0.0;
	
	// Update the sensory input
	NervousSystem.SetNeuronExternalInput(1,Leg.Angle * 5.0/ForwardAngleLimit);
	// Update the nervous system
	NervousSystem.EulerStep(StepSize);
	double o = NervousSystem.NeuronOutput(1);
	// Update the leg effectors
	if (o > 0.5) {
		Leg.FootState = 1; 
		Leg.Omega = 0;
		Leg.ForwardForce = 2 * (o - 0.5) * MaxLegForce;
	}
	else {
		Leg.FootState = 0;
		Leg.BackwardForce = 2 * (0.5 - o) * MaxLegForce;
	}
	// Compute the force applied to the body (*** USING THE "NEW" MODEL ***)
	double f = Leg.ForwardForce - Leg.BackwardForce;
	if (Leg.FootState == 1.0)
		if ((Leg.Angle >= BackwardAngleLimit && Leg.Angle <= ForwardAngleLimit) || 
		    (Leg.Angle < BackwardAngleLimit && f < 0) || 
		    (Leg.Angle > ForwardAngleLimit && f > 0))
		force = f;
	// Update the position of the body
	vx = vx + StepSize * force;
	if (vx < -MaxVelocity) vx = -MaxVelocity;
	if (vx > MaxVelocity) vx = MaxVelocity;
	cx = cx + StepSize * vx;
	// Update the leg geometry
	Leg.JointX = Leg.JointX + StepSize * vx;
	if (Leg.FootState == 1.0) {
		double angle = atan2(Leg.FootX - Leg.JointX,Leg.FootY - Leg.JointY);
		Leg.Omega = (angle - Leg.Angle)/StepSize;
		Leg.Angle = angle;
	}
	else {
		vx = 0.0;
		Leg.Omega	= Leg.Omega + StepSize * MaxTorque * (Leg.BackwardForce - Leg.ForwardForce);
		if (Leg.Omega < -MaxOmega) Leg.Omega = -MaxOmega;
		if (Leg.Omega > MaxOmega) Leg.Omega = MaxOmega;
		Leg.Angle = Leg.Angle + StepSize * Leg.Omega;
		if (Leg.Angle < BackwardAngleLimit) {Leg.Angle = BackwardAngleLimit; Leg.Omega = 0;}
		if (Leg.Angle > ForwardAngleLimit) {Leg.Angle = ForwardAngleLimit; Leg.Omega = 0;}
		Leg.FootX = Leg.JointX + LegLength * sin(Leg.Angle);
		Leg.FootY = Leg.JointY + LegLength * cos(Leg.Angle);
	}
	// If the foot is too far back, the body becomes "unstable" and forward motion ceases
	if (cx - Leg.FootX > 20) vx = 0.0;
}


// Step the LeggedAgent using the optimal pattern generator

void LeggedAgent::PerfectStep(double StepSize)
{
	double force = 0.0;

	// Update the leg effectors
	if (Leg.FootState == 0.0 && Leg.Angle >= ForwardAngleLimit) {Leg.FootState = 1; Leg.Omega = 0;}
	else if (Leg.FootState == 1.0 && (cx - Leg.FootX > 20)) Leg.FootState = 0;
	// Compute the force applied to the body
	if (Leg.FootState == 1.0 && Leg.Angle >= BackwardAngleLimit && Leg.Angle <= ForwardAngleLimit)
		force = MaxLegForce;
	// Update the position of the body
	vx = vx + StepSize * force;
	if (vx < -MaxVelocity) vx = -MaxVelocity;
	if (vx > MaxVelocity) vx = MaxVelocity;
	cx = cx + StepSize * vx;
	// Update the leg geometry
	Leg.JointX = Leg.JointX + StepSize * vx;
	if (Leg.FootState == 1.0) {
		double angle = atan2(Leg.FootX - Leg.JointX,Leg.FootY - Leg.JointY);
		Leg.Omega = (angle - Leg.Angle)/StepSize;
		Leg.Angle = angle;
	}
	else {
		vx = 0.0;
		Leg.Omega	= Leg.Omega + StepSize * MaxTorque * MaxLegForce;
		if (Leg.Omega < -MaxOmega) Leg.Omega = -MaxOmega;
		if (Leg.Omega > MaxOmega) Leg.Omega = MaxOmega;
		Leg.Angle = Leg.Angle + StepSize * Leg.Omega;
		if (Leg.Angle < BackwardAngleLimit) {Leg.Angle = BackwardAngleLimit; Leg.Omega = 0;}
		if (Leg.Angle > ForwardAngleLimit) {Leg.Angle = ForwardAngleLimit; Leg.Omega = 0;}
		Leg.FootX = Leg.JointX + LegLength * sin(Leg.Angle);
		Leg.FootY = Leg.JointY + LegLength * cos(Leg.Angle);
	}
	// If the foot is too far back, the body becomes "unstable" and forward motion ceases
	if (cx - Leg.FootX > 20) vx = 0.0;
}

