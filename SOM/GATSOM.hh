//------------------------------
//
// Name: GATSOM.hh
//
// Description: defines a self organizing map (SOM)
//
// Author: Carly Wilson
//
//-------------------------------

#ifndef GATSOM_HH
#define GATSOM_HH

#include <iostream>
#include <vector>
#include <TH1F.h>

#include "GATNeuron.hh"

using namespace std;

class SOM
{
	public: 
		SOM(vector<size_t> argDimensions, size_t numWeights);
		Neuron* FindBMU(vector<double> argInput);
		TH1D PlotNeuron(size_t arg);
		void TrainNetwork(vector<vector<double> > trainingData);
		void PrintNetwork();
		void SetNEpochs(size_t epochs);
		void SetInitialLearningRate(double initialLearningRate);

		friend ostream& operator<<(ostream & stream, SOM *arg);
		friend ostream& operator<<(ostream & stream, SOM &arg)
		{
			stream<<(&arg);
			return stream;
		};

		friend istream& operator>>(istream & stream, SOM *arg);
		friend istream& operator>>(istream & stream, SOM &arg)
		{
			stream>>(&arg);
			return stream;
		};
		vector<Neuron*> fNeurons;

	private: 
		vector<size_t> fDimensions;
		double fDistance;
		double fInitialLearningRate;
		size_t fnEpochs;
		size_t fVersion;

		Neuron* BMNeuron;
};

#endif