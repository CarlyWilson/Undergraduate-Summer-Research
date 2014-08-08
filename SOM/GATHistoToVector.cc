//----------------------------------------
//
// File Name: GATHistoToVector.cc
//
// Description: Initialization for HistoToVector header file and converts a histogram to a vector<double>.
//
// Author: Carly Jo Wilson
//
//----------------------------------------

#include "GATHistoToVector.hh"

HistoToVector::HistoToVector()
{
	fN = 100;
	fNormalization = -1;
}

void HistoToVector::ConvertToVector()
{
	Normalize();
	FindAlignmentPoint();

	if(fAlignmentPoint < fN/2)
	{
		cout<<"Warning: need to artificially re-align waveform"<<endl;
		fAlignmentPoint += (fN/2 - fAlignmentPoint + 1);//check math
	}

	size_t maxBin = fHist->GetMaximumBin();

	if(maxBin + fN/2 > fHist->GetNbinsX())
	{
		cout<<"Warning: need to artificially re-align waveform"<<endl;
		fAlignmentPoint -= (fHist->GetNbinsX() - fAlignmentPoint - 1); //check math
	}

	fVector.resize(fN);
	for(size_t i = 0; i < fN; i++)
	{
		fVector[i] = fHist->GetBinContent(fAlignmentPoint - fN/2 + i);
	}
}

void HistoToVector::Normalize()
{
	if(fNormalization < 0)
	{
		fNormalization = fHist->GetMaximum() - fHist->GetMinimum();
	}

	fHist->Scale(1.0/fNormalization);
}

void HistoToVector::FindAlignmentPoint()
{
	//default set fAlignmentPoint to T50
	double max = fHist->GetMaximum();
	double alpoint = 0.5*max; //make 0.5 a data memeber to be set
	size_t maxBin = fHist->GetMaximumBin();
	fAlignmentPoint = maxBin;

	while(fHist->GetBinContent(fAlignmentPoint) > alpoint) fAlignmentPoint--;
}

void HistoToVector::SetfN(size_t numOutputs)
{
	fN = numOutputs;
}

void HistoToVector::SetfAlignmentPoint(size_t fAlign)
{
	fAlignmentPoint = fAlign;
}

void HistoToVector::SetfNormalization(double fNorm)
{
	fNormalization = fNorm;
}

size_t HistoToVector::GetfN()
{
	return fN;
}

size_t HistoToVector::GetfAlignmentPoint()
{
	return fAlignmentPoint;
}

double HistoToVector::GetfNormalization()
{
	return fNormalization;
}
