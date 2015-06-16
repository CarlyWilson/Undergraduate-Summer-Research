//----------------------------
//
// Name: GATSOM.cc
//
// Description: contains implementations of functions listed in SOM.h
//
// Author: Carly Wilson
//
//----------------------------

#include <sstream> 
#include <math.h>

#include "GATSOM.hh"
#include <TH1F.h>
#include <TRandom3.h>

GATSOM::GATSOM(vector<size_t> dimensions, size_t numWeights){
	size_t numDimensions = dimensions.size();

	this->dimensions = dimensions;
	initialLearningRate = 0.9;
	numEpochs = 100000;
	version = 1;

	distanceCalc = Euclidean;
	trainingType = Classic;
	
	if(numDimensions > 2){
		cout<<"Error: Dimensions must be either 1 or 2"<<endl;
		return;
	}

	if(numDimensions == 1){
		size_t numDimensionsX = dimensions[0];
		neurons.resize(numDimensionsX);
		vector<double> position(numDimensions);

		for(size_t i = 0; i < numDimensionsX; i++){
			neurons[i] = new GATNeuron(position, numWeights);
			position[0] = double(i);
			neurons[i]->SetPosition(position);
		}
	}
	else if(numDimensions == 2){
		size_t numDimensionsX = dimensions[0];
		size_t numDimensionsY = dimensions[1];
		size_t index = 0;
		neurons.resize(numDimensionsX*numDimensionsY);
		vector<double> position(numDimensions);
		
		for(size_t row = 0; row < numDimensionsX; row++){
			for(size_t column = 0; column < numDimensionsY; column++){
				neurons[index] = new GATNeuron(position, numWeights);
				position[0] = double(row);
				position[1] = double(column);
				neurons[index]->SetPosition(position);
				index++;
			}
		}
	}
}

GATSOM::GATSOM(){
	initialLearningRate = 0.9;
	numEpochs = 100000;
	distanceCalc = Euclidean;
	trainingType = Classic;
}

GATNeuron* GATSOM::FindBMU(vector<double> weights){
	size_t numNeurons = neurons.size();
	double lowestDistance = 1e6;

	GATNeuron* BMNeuron;
	
	for(size_t i = 0; i < numNeurons; i++){
		double distance = neurons[i]->GetWeightDistanceFrom(weights, distanceCalc);

		if(distance < lowestDistance){
			lowestDistance = distance;
			BMNeuron = neurons[i];
		}
	}
	return BMNeuron;
}

TH1D GATSOM::PlotNeuron(size_t index){
	vector<double> weight = neurons[index]->GetWeight();
	vector<double> position = neurons[index]->GetPosition();
	size_t numWeights = weight.size();

	string title;
	stringstream titlestream;
	titlestream<<"Neuron";


	for(size_t i = 0; i < position.size(); i++){
		titlestream<<"_"<<position[i];
	}

	title = titlestream.str();
	TH1D histogram(title.c_str(), "", numWeights, 0, numWeights);

	for(size_t i = 0; i < numWeights; i++){
		histogram.SetBinContent(i + 1, weight[i]);
	}
	return histogram;
}

void GATSOM::ClassicTrainingAlgorithm(vector<vector<double> > trainingData){
	size_t numDimensionsX, numDimensionsY;
	size_t numDimensions = dimensions.size();
	double mapRadius;

	if(numDimensions == 1){
		numDimensionsX = dimensions[0];
		mapRadius = 0.5*double(numDimensionsX);
	}
	else if(numDimensions == 2){
		numDimensionsX = dimensions[0];
		numDimensionsY = dimensions[1];
		mapRadius = (numDimensionsX >= numDimensionsY ? 0.5*double(numDimensionsX) : 0.5*double(numDimensionsY));
	}

	size_t numTraining = trainingData.size();
	size_t numNeurons = neurons.size();

	TRandom3 rand;

	for(size_t time = 0; time < numEpochs; time++){
		double learningRate = initialLearningRate*exp(-(double)time/(double)numEpochs);
		double radiusOverTime = mapRadius*exp(-(double)time*log(mapRadius)/(double)numEpochs);
		size_t randomNumber = rand.Integer(numTraining);
		GATNeuron *bmu = FindBMU(trainingData[randomNumber]);
		vector<double> bmuPosition = bmu->GetPosition();
		vector<double> weights = bmu->GetWeight();

		for(size_t i = 0; i < numNeurons; i++){
			double distance = neurons[i]->GetDistanceFromNeuron(bmu);
			double factor = learningRate*exp(-distance/(2.0*radiusOverTime*radiusOverTime));
			neurons[i]->AdjustWeightClassic(trainingData[randomNumber], factor);
		}
	}
}

void GATSOM::BatchTrainingAlgorithm(vector<vector<double> > trainingData){
	size_t numDimensionsX, numDimensionsY;
	size_t numDimensions = dimensions.size();
	double mapRadius;

	if(numDimensions == 1){
		numDimensionsX = dimensions[0];
		mapRadius = 0.5*double(numDimensionsX);
	}
	else if(numDimensions == 2){
		numDimensionsX = dimensions[0];
		numDimensionsY = dimensions[1];
		mapRadius = (numDimensionsX >= numDimensionsY ? 0.5*double(numDimensionsX) : 0.5*double(numDimensionsY));
	}

	size_t numTraining = trainingData.size();
	size_t numNeurons = neurons.size();

	TRandom3 rand;

	for(size_t time = 0; time < numEpochs; time++){
		vector<vector<double> > numerator;
		vector<double> denominator;
		double learningRate = initialLearningRate*exp(-(double)time/(double)numEpochs);
		double radiusOverTime = mapRadius*exp(-(double)time*log(mapRadius)/(double)numEpochs);
		size_t randomNumber = rand.Integer(numTraining);

		for(size_t input = 1; input <= trainingData.size(); input++){
			GATNeuron *bmu = FindBMU(trainingData[randomNumber]);
			vector<double> bmuPosition = bmu->GetPosition();
			vector<double> weights = bmu->GetWeight();

			for(size_t i = 1; i <= numNeurons; i++){
				double distance = neurons[i]->GetDistanceFromNeuron(bmu);
				double factor = exp(-distance/(radiusOverTime*radiusOverTime));

				for(int j = 0; j < trainingData[input].size(); j++){
					numerator[i][j] += factor*trainingData[input][j];
					denominator[i] += factor;
				}
			}
		}
		for(size_t i = 0; i < numNeurons; i++){
			neurons[i]->AdjustWeightBatch(numerator[i], denominator[i]);
		}
	}
}

void GATSOM::TrainNetwork(vector<vector<double> > trainingData, TrainingAlgorithmType type){
	switch (type){
		case Classic:
			ClassicTrainingAlgorithm(trainingData);
			break;
		case Batch:
			BatchTrainingAlgorithm(trainingData);
			break;
		default:
			cout<<"Error: Fxn TrainNetwork cannot accept a TrainingAlgorithmType of "<<type<<endl;
			break;
	}
}

void GATSOM::PrintNetwork(){
	for (size_t i = 0; i < neurons.size(); i++){
		cout<<neurons[i];
	}
	cout<<endl;
}

void GATSOM::SetNumEpochs(size_t epochs){
		numEpochs = epochs;
}

void GATSOM::SetInitialLearningRate(double initialLearningRate){
		this->initialLearningRate = initialLearningRate;
}

void GATSOM::SetDistCalcType(DistanceCalcType type){
	distanceCalc = type;
}

void GATSOM::SetTrainingAlgorithmType(TrainingAlgorithmType type){
	trainingType = type;
}

GATNeuron* GATSOM::GetNeuron(size_t index){
	return neurons[index];
}

vector<GATNeuron*> GATSOM::GetNeurons(){
	return neurons;
}

DistanceCalcType GATSOM::GetDistCalcType(){
	return distanceCalc;
}

TrainingAlgorithmType GATSOM::GetAlgorithmType(){
	return trainingType;
}

double GATSOM::GetPopularityOfBMU(vector<double> weights){
	return FindBMU(weights)->GetPopularity();
}

ostream &operator<<(ostream & stream, GATSOM *som){
	stream<<som->version<<" ";
	stream<<som->neurons.size()<<" ";

	for(size_t i = 0; i < som->neurons.size(); i++){
		stream<<som->neurons[i]<<" ";
	}

	stream<<som->dimensions.size()<<" ";
	
	for(size_t i = 0; i < som->dimensions.size(); i++){
			stream<<som->dimensions[i]<<" ";
	}

	stream<<som->numEpochs<<" ";
	stream<<som->initialLearningRate<<" ";
	stream<<endl;
	return stream;
}

istream &operator>>(istream & stream, GATSOM *som){
	stream>>som->version;

	if(som->version == 1){
		size_t numNeurons;
		stream>>numNeurons;
		cout<<"numNeurons: "<<numNeurons<<endl;
		som->neurons.resize(numNeurons);

		vector<double> trash;
		size_t junk = 0;

		for(size_t i = 0; i < numNeurons; i++){
			som->neurons[i] = new GATNeuron(trash, junk);
			stream>>som->neurons[i];
		}

		size_t numDimensions;
		stream>>numDimensions;
		som->dimensions.resize(numDimensions);

		for(size_t i = 0; i < numDimensions; i++){
			stream>>som->dimensions[i];
		}

		stream>>som->numEpochs;
		stream>>som->initialLearningRate;
	}
	else{
		cerr<<"Unknown version of SOM"<<endl;
	}
}
