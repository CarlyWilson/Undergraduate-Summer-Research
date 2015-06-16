//-----------------------------------------
// Name: GATNeuron.hh
//
// Description: defines a neuron for a self organizing map (SOM)
//
// Author: Carly Wilson
//
//----------------------------------------

#ifndef GATNEURON_HH
#define GATNEURON_HH

#include <iostream>
#include <vector>

using namespace std;

enum DistanceCalcType
{
	Euclidean = 0,
	Chisquared = 1
};

class GATNeuron
{
	public:
		GATNeuron(vector<double> argPosition, size_t numWeights);
		void AdjustWeightClassic(vector<double> &input, double factor);
		void AdjustWeightBatch(vector<double> &input, double factor);
		void SetPosition(vector<double> argPosition);
		void SetWeight(vector<double> argWeight);
		void IncreasePopularity(double numOfWaveforms);
		vector<double> GetPosition();
		vector<double> GetWeight();
		double GetPopularity();
		double GetEuclideanDistance(vector<double> &argInput);
		double GetChiSquaredDistance(vector<double> &argInput);
		double GetWeightDistanceFrom(vector<double> &argInput, DistanceCalcType type);
		double GetPositionDistanceFrom(vector<double> argPosition);
		double GetDistanceFromNeuron(GATNeuron* argNeuron);

		friend ostream& operator<<(ostream & stream, GATNeuron *arg);
		friend ostream& operator<<(ostream & stream, GATNeuron &arg)
		{
			stream<<(&arg);
			return stream;
		};

		friend istream& operator>>(istream & stream, GATNeuron *arg);
		friend istream& operator>>(istream & stream, GATNeuron &arg)
		{
			stream>>(&arg);
			return stream;
		};
		
	private:
		size_t fVersion;
		double fPopularity; 
		vector<double> fPosition;
		vector<double> fWeight;
};

#endif
