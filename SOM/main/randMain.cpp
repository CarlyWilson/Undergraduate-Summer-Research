//--------------------------------
//
// Name: randMain.cpp
//
// Description: Initializes a network with random weight vectors, prints the network, 
//	then finds the BMU to some input vector, and displays the position vector of that BMU
//
// Author: Carly Wilson
//
//-------------------------------

#include "../Neuron.h"
#include "../SOM.h"

//make it so when it prints out it is readable 

int main(int argc, char* argv[])
{
	srandom((unsigned)time(NULL));
	
	vector<size_t> dimensions(2);

	dimensions[0] = 5;// nx
	dimensions[1] = 5;// ny

	size_t numWeights = 4;

	SOM* som = new SOM(dimensions, numWeights);
	som->SetNEpochs(10000);
	som->SetInitialLearningRate(0.9);

	size_t nTraining = 10;

	vector<vector<double> > trainingData(nTraining);
	double random;

	for(int i = 0; i < nTraining; i++)
	{
		trainingData[i].resize(numWeights);

		for(int j = 0; j < numWeights; j++)
		{
			random = ((double)rand()/(double)RAND_MAX);
			trainingData[i][j] = random;
		}
	}

	som->PrintNetwork();
	som->TrainNetwork(trainingData);
	som->PrintNetwork();

	Neuron* bmu = som->FindBMU(trainingData[0]);
	vector<double> pos = bmu->GetPosition();

	cout<<"The bmu is at position: ";

	for(int i = 0; i <dimensions.size(); i++)
	{
		cout<<pos[i]<<" ";
	}
	
	cout<<endl;
}
