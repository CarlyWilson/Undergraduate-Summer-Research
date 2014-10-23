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

class GATSOM
{
	public: 
		GATSOM(vector<size_t> argDimensions, size_t numWeights);
		GATSOM(); //constructor with no arguements
		GATNeuron* FindBMU(vector<double> argInput);
		GATNeuron* GetNeuron(size_t arg);
		GATNeuron* SetDistCalType(DistanceCalcType type);
		vector<GATNeuron*> GetNeurons();
		TH1D PlotNeuron(size_t arg);
		void TrainNetwork(vector<vector<double> > trainingData);
		void PrintNetwork();
		void SetNEpochs(size_t epochs);
		void SetInitialLearningRate(double initialLearningRate);
		void SetDistCalcType(DistanceCalcType type);
		double GetPopularityOfBMU(vector<double> argInput);

		friend ostream& operator<<(ostream & stream, GATSOM *arg);
		friend ostream& operator<<(ostream & stream, GATSOM &arg)
		{
			stream<<(&arg);
			return stream;
		};

		friend istream& operator>>(istream & stream, GATSOM *arg);
		friend istream& operator>>(istream & stream, GATSOM &arg)
		{
			stream>>(&arg);
			return stream;
		};
		vector<GATNeuron*> fNeurons;

	private: 
		vector<size_t> fDimensions;
		double fDistance;
		double fInitialLearningRate;
		size_t fnEpochs;
		size_t fVersion;
		DistanceCalcType fDistanceCalc;

		GATNeuron* BMNeuron;
};

#endif
