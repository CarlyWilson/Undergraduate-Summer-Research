//----------------------------
//
// Name: GATSOM.cc
//
// Description: contains implementations of functions listed in SOM.h
//
// Author: Carly Wilson
//
//----------------------------

#include <math.h>
#include <sstream> 
#include <TH1F.h>
#include <TRandom3.h>
#include "GATSOM.hh"

GATSOM::GATSOM(vector<size_t> argDimensions, size_t numWeights)
{
	size_t ndim = argDimensions.size();

	fDimensions = argDimensions;
	fInitialLearningRate = 0.9;
	fnEpochs = 100000;
	fDistanceCalc = 0;

	fVersion = 1;
	
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
			fNeurons[i] = new GATNeuron(position, numWeights);
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
				fNeurons[count] = new GATNeuron(position, numWeights);
				position[0] = double(i);
				position[1] = double(j);
				fNeurons[count]->SetPosition(position);
				count++;
			}
		}
	}
}

GATNeuron* GATSOM::FindBMU(vector<double> argInput)
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

GATNeuron* GATSOM::GetNeuron(size_t arg)
{
	return fNeurons[arg];
}

GATNeuron* GATSOM::SetDistCalType(size_t arg)
{
	fDistanceCalc = arg;
}

vector<GATNeuron*> GATSOM::GetNeurons()
{
	return fNeurons;
}

TH1D GATSOM::PlotNeuron(size_t arg)
{
	vector<double> weight = fNeurons[arg]->GetWeight();
	size_t n = weight.size();
	string title;
	stringstream titlestream;
	titlestream<<"Neuron";
	vector<double> pos = fNeurons[arg]->GetPosition();

	for(size_t i = 0; i < pos.size(); i++)
	{
		titlestream<<"_"<<pos[i];
	}

	title = titlestream.str();
	TH1D h(title.c_str(), "", n, 0, n);

	for(size_t j = 0; j < n; j++)
	{
		h.SetBinContent(j + 1, weight[j]);
	}
	return h;
}

void GATSOM::PrintNetwork()
{
	for (size_t i = 0; i < fNeurons.size(); i++)
	{
		cout<<fNeurons[i];
	}
	cout<<endl;
}

void GATSOM::SetNEpochs(size_t epochs)
{
		fnEpochs = epochs;
}

void GATSOM::SetInitialLearningRate(double initialLearningRate)
{
		fInitialLearningRate = initialLearningRate;
}

ostream &operator<<(ostream & stream, GATSOM *arg)
{
	stream<<arg->fVersion<<" ";
	stream<<arg->fNeurons.size()<<" ";

	for(size_t i = 0; i < arg->fNeurons.size(); i++)
	{
		stream<<arg->fNeurons[i]<<" ";
	}

	stream<<arg->fDimensions.size()<<" ";
	
	for(size_t k = 0; k < arg->fDimensions.size(); k++)
	{
			stream<<arg->fDimensions[k]<<" ";
	}

	stream<<arg->fnEpochs<<" ";
	stream<<arg->fInitialLearningRate<<" ";
	stream<<endl;
	return stream;
}

istream &operator>>(istream & stream, GATSOM *arg)
{
	stream>>arg->fVersion;

	if(arg->fVersion == 1)
	{
		size_t nNeurons;
		stream>>nNeurons;
		cout<<"nNeurons: "<<nNeurons<<endl;
		arg->fNeurons.resize(nNeurons);

		vector<double> trash;
		size_t junk = 0;

		for(size_t k = 0; k < nNeurons; k++)
		{
			arg->fNeurons[k] = new GATNeuron(trash, junk);
			stream>>arg->fNeurons[k];
		}
		size_t ndim;
		stream>>ndim;
		arg->fDimensions.resize(ndim);
		for(size_t i = 0; i < ndim; i++)
		{
			stream>>arg->fDimensions[i];
		}

		stream>>arg->fnEpochs;
		stream>>arg->fInitialLearningRate;
	}
	else
	{
		cerr<<"Unknown version of SOM"<<endl;
	}
}

void GATSOM::TrainNetwork(vector<vector<double> > trainingData)
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

	for(size_t t = 0; t < fnEpochs; t++)
	{
		double learningRate = fInitialLearningRate*exp(-(double)t/(double)fnEpochs);
		rt = Rmap*exp(-(double)t*log(Rmap)/(double)fnEpochs);
		n = rand.Integer(nTraining);
		GATNeuron *bmu = FindBMU(trainingData[n]);
		vector<double> bmuPosition = bmu->GetPosition();
		vector<double> weights = bmu->GetWeight();

		for(size_t i = 0; i < nNeurons; i++)
		{
			d = fNeurons[i]->GetDistanceFromNeuron(bmu);
			factor = learningRate*exp(-d/(2.0*rt*rt));
			fNeurons[i]->AdjustWeight(trainingData[n], factor);
		}
	}
}
