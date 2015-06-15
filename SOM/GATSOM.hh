//------------------------------
//
// Name: GATSOM.hh
//
// Description: Defines a self organizing map (SOM)
//
// Author: Carly Wilson
//
//-------------------------------

#ifndef SOM_GATSOM_HH_
#define SOM_GATSOM_HH_

#include <iostream>
#include <vector>

#include "GATNeuron.hh"
#include <TH1F.h>

using namespace std;

class GATSOM
{
	public: 
		GATSOM(vector<size_t> dimensions, size_t numWeights);
		GATSOM();

		GATNeuron* FindBMU(vector<double> weights);
		TH1D PlotNeuron(size_t index);
		void TrainNetwork(vector<vector<double> > trainingData);
		void PrintNetwork();

		void SetNumEpochs(size_t epochs);
		void SetInitialLearningRate(double initialLearningRate);
		void SetDistCalcType(DistanceCalcType type);

		GATNeuron* GetNeuron(size_t index);
		vector<GATNeuron*> GetNeurons();
		DistanceCalcType GetDistCalcType();
		double GetPopularityOfBMU(vector<double> weights);

		friend ostream& operator<<(ostream & stream, GATSOM *som);
		friend ostream& operator<<(ostream & stream, GATSOM &som)
		{
			stream<<(&som);
			return stream;
		};

		friend istream& operator>>(istream & stream, GATSOM *som);
		friend istream& operator>>(istream & stream, GATSOM &som)
		{
			stream>>(&som);
			return stream;
		};

	private: 
		size_t numEpochs;
		size_t version;
		vector<size_t> dimensions;
		vector<GATNeuron*> neurons;
		double distance;
		double initialLearningRate;
		DistanceCalcType distanceCalc;
};

#endif
