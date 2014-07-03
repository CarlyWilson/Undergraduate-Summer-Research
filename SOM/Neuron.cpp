//---------------------------------
//
// Name: Neuron.c
//
// Description: initalizes neuron constructor
//
// Author: Carly Wilson
//
//--------------------------------

#include <math.h>
#include <stdlib.h>

#include "Neuron.h"

//Constructor
Neuron::Neuron(vector<double> argPosition, size_t numWeights)
{
	fPosition = argPosition;

	double random;

	for(size_t i = 0; i < numWeights; i++)
	{
		random = ((double)rand()/(double)RAND_MAX);
		fWeight.push_back(random);
	}
}

void Neuron::SetPosition(vector<double> argPosition)
{
	fPosition = argPosition;
}

void Neuron::SetWeight(vector<double> argWeight)
{
	fWeight = argWeight; 
}

vector<double> Neuron::GetPosition()
{
	return fPosition;
}

vector<double> Neuron::GetWeight()
{
	return fWeight;
}

double Neuron::GetWeightDistanceFrom(vector<double> argInput)
{
        double weightDistance = 0;

        for(size_t i = 0; i < fWeight.size(); i++)
        {
                weightDistance += (argInput[i] - fWeight[i])*(argInput[i] - fWeight[i]);
        }

        return sqrt(weightDistance);
}

double Neuron::GetPositionDistanceFrom(vector<double> argPosition)
{
	double positionDistance = 0;

	for(size_t i = 0; i < fPosition.size(); i++)
	{
		positionDistance += (argPosition[i] - fPosition[i])*(argPosition[i] - fPosition[i]);
	}
	
	return sqrt(positionDistance);
}

double Neuron::GetDistanceFromNeuron(Neuron* argNeuron)
{
	vector<double> pos = argNeuron->GetPosition();
	
	return GetPositionDistanceFrom(pos);
}

void Neuron::AdjustWeight(vector<double> input, double factor)
{
	//cout fweight before and after 
    if(fWeight.size()!=input.size())
    {
    	cout<<"Error! Cannot adjust weights!"<<endl;
        return;
    }
    for(size_t i = 0; i < input.size(); i++)
	{
			//cout<<"fWeight before in Neuron class AdjustWeight is: "<<fWeight[i]<<endl;
		fWeight[i] = fWeight[i] + factor * (input[i] - fWeight[i]);
			//cout<<"fWeight after in Neuron class AdjustWeight is: "<<fWeight[i]<<endl;
	}
}
//output first
//write first for neuron to cout neuron size of weight vector load weights, entries in weight vector, weight vector, position of weight vector
/*ostream& operator<<(ostream &os, const Neuron& obj)
{
	//write obj to stream
    os<<obj<<endl;
    return os;
}*/

