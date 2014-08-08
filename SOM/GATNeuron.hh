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

class Neuron
{
	public:
		Neuron(vector<double> argPosition, size_t numWeights);
		void AdjustWeight(vector<double> input, double factor);
		void SetPosition(vector<double> argPosition);
		void SetWeight(vector<double> argWeight);
		void IncreasePopularity(double numOfWaveforms);
		vector<double> GetPosition();
		vector<double> GetWeight();
		double GetPopularity();
		double GetWeightDistanceFrom(vector<double> argInput);
		double GetPositionDistanceFrom(vector<double> argPosition);
		double GetDistanceFromNeuron(Neuron* argNeuron);

		friend ostream& operator<<(ostream & stream, Neuron *arg);
		friend ostream& operator<<(ostream & stream, Neuron &arg)
		{
			stream<<(&arg);
			return stream;
		};

		friend istream& operator>>(istream & stream, Neuron *arg);
		friend istream& operator>>(istream & stream, Neuron &arg)
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