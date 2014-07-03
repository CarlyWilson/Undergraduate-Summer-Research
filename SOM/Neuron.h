//-----------------------------------------
// Name: Neuron.h
//
// Description: defines a neuron for a self organizing map (SOM)
//
// Author: Carly Wilson
//
//----------------------------------------

#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <vector>

using namespace std;

class Neuron
{
	public:
		Neuron(vector<double> argPosition, size_t numWeights);//Constructor
		void AdjustWeight(vector<double> input, double factor);
		void SetPosition(vector<double> argPosition);
		void SetWeight(vector<double> argWeight);
		vector<double> GetPosition();
		vector<double> GetWeight();
		double GetWeightDistanceFrom(vector<double> argInput);
		double GetPositionDistanceFrom(vector<double> argPosition);
		double GetDistanceFromNeuron(Neuron* argNeuron);
		
	private:
		vector<double> fPosition;
		vector<double> fWeight;
		
};

//ostream& operator<<(ostream &os, const Neuron& obj);

#endif
