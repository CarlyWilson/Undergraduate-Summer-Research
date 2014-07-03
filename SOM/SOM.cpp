//----------------------------
//
// Name: SOM.c
//
// Description: contains implementations of functions listed in SOM.h
//
// Author: Carly Wilson
//
//----------------------------

#include <math.h>

#include <TRandom3.h>
#include "SOM.h"

SOM::SOM(vector<size_t> argDimensions, size_t numWeights)
{
	size_t ndim = argDimensions.size();

	fDimensions = argDimensions;
	fInitialLearningRate = 0.9;
	fnEpochs = 10000;
	
	if(ndim > 2)
	{
		cout<<"Error: Dimensions must be either 1 or 2"<<endl;
		return;
	}
	if(ndim == 1)
	{
		size_t nx = argDimensions[0];
		fNeurons.resize(nx);
		vector<double> position(ndim);

		for(size_t i = 0; i < nx; i++)
		{
			fNeurons[i] = new Neuron(position, numWeights);
			position[0] = double(i);
			fNeurons[i]->SetPosition(position);
		}
	}
	if(ndim == 2)
	{
		size_t nx = argDimensions[0];
		size_t ny = argDimensions[1];
		size_t count = 0;
		fNeurons.resize(nx*ny);
		vector<double> position(ndim);
		
		for(size_t i = 0; i < nx; i++)
		{
			for(size_t j = 0; j < ny; j++)
			{
				fNeurons[count] = new Neuron(position, numWeights);
				position[0] = double(i);
				position[1] = double(j);
				fNeurons[count]->SetPosition(position);
				count++;
			}
		}
	}
}

Neuron* SOM::FindBMU(vector<double> argInput)
{
	size_t nNeurons = fNeurons.size();
	
	double lowestDistance = 1e6;
	
	for(size_t i = 0; i < nNeurons; i++)
	{
		double distance = fNeurons[i]->GetWeightDistanceFrom(argInput);

		if(distance < lowestDistance)
		{
			lowestDistance = distance;
			BMNeuron = fNeurons[i];
		}
	}

	return BMNeuron;
}

void SOM::PrintNetwork()
{
	size_t n = fNeurons.size();

	for (size_t i = 0; i < n; i++)
	{
  		vector<double> pos = fNeurons[i]->GetPosition();
  		cout<<"Neuron "<<i<<" position: ";
  		for(size_t j = 0; j < pos.size(); j++)
		{
   			cout<<pos[j]<<" ";
  		}
  		vector<double> w = fNeurons[i]->GetWeight();
  		cout<<"Weights: ";
  		for(size_t j = 0; j < w.size(); j++)
		{
   			cout<<w[j]<<" ";
 		}
		cout<<endl;
	}	
}

void SOM::SetNEpochs(size_t epochs)
{
		fnEpochs = epochs;
}

void SOM::SetInitialLearningRate(double initialLearningRate)
{
		fInitialLearningRate = initialLearningRate;
}

void SOM::TrainNetwork(vector<vector<double> > trainingData)
{
	double Rmap;
	size_t nx, ny;
	size_t ndim = fDimensions.size();

	if(ndim == 1)
	{
		nx = fDimensions[0];
		Rmap = 0.5*double(nx);
	}
	if(ndim == 2)
	{
		nx = fDimensions[0];
		ny = fDimensions[1];
		Rmap = (nx >= ny ? 0.5*double(nx) : 0.5*double(ny));
	}

	double rt = 0;
	double d, factor;
	size_t nTraining = trainingData.size();
	size_t n;
	size_t nNeurons = fNeurons.size();

	TRandom3 rand;

	for(int t = 0; t < fnEpochs; t++)
	{
		double learningRate = fInitialLearningRate*exp((double)-t/(double)fnEpochs);
		rt = Rmap*exp(-1*log(Rmap)/fnEpochs);
		n = rand.Integer(nTraining);
		Neuron *bmu = FindBMU(trainingData[n]);
			//cout<<"trainingData[n] inside of SOM TrainNetwork is: "<<trainingData[n]<<endl;
			//cout<<"Neuron *bmu inside of SOM TrainNetwork is: "<<bmu<<endl;
			//cout<<"Neuron *bmu weight inside of SOM TrainNetwork is: "<<bmu->GetWeight()<<endl;
		vector<double> bmuPosition = bmu->GetPosition();

		for(size_t i = 0; i < nNeurons; i++)
		{
			d = fNeurons[i]->GetDistanceFromNeuron(bmu);
			factor = learningRate*exp(-d/(2.0*rt*rt));
			fNeurons[i]->AdjustWeight(trainingData[n], factor);
		}
	}
}

/*void PredictionPhase(vector<double> realTestData)
{
}*/
