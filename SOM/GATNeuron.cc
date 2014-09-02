//---------------------------------
//
// Name: GATNeuron.cc
//
// Description: initalizes neuron constructor
//
// Author: Carly Wilson
//
//--------------------------------

#include <math.h>
#include <stdlib.h>
#include <TRandom3.h>

#include "GATNeuron.hh"

GATNeuron::GATNeuron(vector<double> argPosition, size_t numWeights)
{
	fPosition = argPosition;
	fVersion = 1;
	fPopularity = 0; 
	TRandom3 rand;

	double random;

	for(size_t i = 0; i < numWeights; i++)
	{
		random = rand.Rndm();
		fWeight.push_back(random);
	}
}

void GATNeuron::SetPosition(vector<double> argPosition)
{
	fPosition = argPosition;
}

void GATNeuron::SetWeight(vector<double> argWeight)
{
	fWeight = argWeight; 
}

void GATNeuron::IncreasePopularity(double numOfWaveforms)
{
	fPopularity += 1.0/numOfWaveforms; 
}

vector<double> GATNeuron::GetPosition()
{
	return fPosition;
}

vector<double> GATNeuron::GetWeight()
{
	return fWeight;
}

double GATNeuron::GetPopularity()
{
	return fPopularity;
}

double GATNeuron::GetWeightDistanceFrom(vector<double> argInput)
{
	double weightDistance = 0;

	for(size_t i = 0; i < fWeight.size(); i++)
    {
        weightDistance += (argInput[i] - fWeight[i])*(argInput[i] - fWeight[i]);
    }

    return sqrt(weightDistance);
}

double GATNeuron::GetPositionDistanceFrom(vector<double> argPosition)
{
	double positionDistance = 0;

	for(size_t i = 0; i < fPosition.size(); i++)
	{
		positionDistance += (argPosition[i] - fPosition[i])*(argPosition[i] - fPosition[i]);
	}
	
	return sqrt(positionDistance);
}

double GATNeuron::GetDistanceFromNeuron(GATNeuron* argNeuron)
{
	vector<double> pos = argNeuron->GetPosition();
	
	return GetPositionDistanceFrom(pos);
}

void GATNeuron::AdjustWeight(vector<double> input, double factor)
{
    if(fWeight.size()!= input.size())
    {
    	cout<<"Error! Cannot adjust weights!"<<endl;
        return;
    }
    for(size_t i = 0; i < input.size(); i++)
	{
		fWeight[i] = fWeight[i] + factor * (input[i] - fWeight[i]);
	}
}

ostream& operator<<(ostream & stream, GATNeuron *arg)
{
	stream<<arg->fVersion<<" ";
	stream<<arg->fPopularity<<" ";
	stream<<arg->fPosition.size()<<" ";

	for(size_t i = 0; i < arg->fPosition.size(); i++)
	{
		stream<<arg->fPosition[i]<<" ";
	}

	stream<<arg->fWeight.size()<<" ";

	for(size_t k = 0; k < arg->fWeight.size(); k++)
	{
		stream<<arg->fWeight[k]<<" ";
	}
	stream<<endl;
	return stream;
}

istream& operator>>(istream & stream, GATNeuron *arg)
{
	stream>>arg->fVersion;
	stream>>arg->fPopularity;
	if(arg->fVersion == 1)
	{
		size_t npos;
		stream>>npos;
		arg->fPosition.resize(npos);

		for(size_t i = 0; i < npos; i++)
		{
			stream>>arg->fPosition[i];
		}

		size_t nw;
		stream>>nw;
		arg->fWeight.resize(nw);

		for(size_t k = 0; k < nw; k++)
		{
			stream>>arg->fWeight[k];
		}
	}
	else
	{
		cerr<<"Unknown version of Neuron"<<endl;
	}
	return stream;
}
