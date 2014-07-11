//--------------------------------
//
// Name: randMain.cc
//
// Description: Initializes a network with random weight vectors, prints the network, 
//	then finds the BMU to some input vector, and displays the position vector of that BMU
//
// Author: Carly Wilson
//
//-------------------------------

#include "../Neuron.hh"
#include "../SOM.hh"

#include <fstream>

int main(int argc, char* argv[])
{
	srandom((unsigned)time(NULL));
	
	vector<size_t> dimensions(2);

	dimensions[0] = 25;// nx
	dimensions[1] = 25;// ny

	size_t numWeights = 4;

	SOM* som = new SOM(dimensions, numWeights);
	som->SetNEpochs(100000);
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
	cout<<bmu;

	ofstream outfile;
	outfile.open("testNeuron.dat");
	outfile<<bmu;
	outfile.close();

	outfile.open("testSOM.dat");
	outfile<<som;
	outfile.close();

	ifstream infile("testNeuron.dat");

	vector<double> junk;
	vector<size_t> junky;
	size_t numJunk = 0;

	Neuron *n = new Neuron(junk, numJunk);// with junk inputs since they will be over written
	infile>>n;
	cout<<n;
	infile.close();

	infile.open("testSOM.dat");
	SOM *s = new SOM(junky, numJunk);
	infile>>s;
	cout<<s;
	infile.close();

	vector<double> pos = bmu->GetPosition();

	cout<<"The bmu is at position: ";

	for(int i = 0; i <dimensions.size(); i++)
	{
		cout<<pos[i]<<" ";
	}

	cout<<endl;
}
