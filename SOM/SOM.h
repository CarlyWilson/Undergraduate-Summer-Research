//------------------------------
//
// Name: SOM.h
//
// Description: defines a self organizing map (SOM)
//
// Author: Carly Wilson
//
//-------------------------------

#ifndef SOM_H
#define SOM_H

#include <iostream>
#include <vector>

#include "Neuron.h"

using namespace std;

class SOM
{
	public: 
		SOM(vector<size_t> argDimensions, size_t numWeights);
		Neuron* FindBMU(vector<double> argInput);
		void TrainNetwork(vector<vector<double> > trainingData);
		void PrintNetwork();
		void SetNEpochs(size_t epochs);
		void SetInitialLearningRate(double initialLearningRate);

	private: 
		vector<size_t> fDimensions;
		double fDistance;
		double fInitialLearningRate;
		size_t fnEpochs;

		vector<Neuron*> fNeurons;
		Neuron* BMNeuron;
};

#endif
