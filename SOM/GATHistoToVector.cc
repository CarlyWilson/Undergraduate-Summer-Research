//----------------------------------------
//
// File Name: GATHistoToVector.cc
//
// Description: Initialization for GATHistoToVector header file and converts a histogram to a vector<double>.
//
// Author: Carly Jo Wilson
//
//----------------------------------------

#include "GATHistoToVector.hh"
#include <MGTWaveform.hh>
#include <MGTEvent.hh>
#include <MGWFBaselineRemover.hh>

GATHistoToVector::GATHistoToVector()
{
	fN = 100;
	fNormalization = -1;
}

void GATHistoToVector::ConvertToVector(DistanceCalcType type)
{
	MGWFBaselineRemover *base = new MGWFBaselineRemover();
	double rms = base->GetBaselineRMS();
	sigma = rms; 

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

	if(type == Euclidean)
	{
		fVector.resize(fN);
		for(size_t i = 0; i < fN; i++)
		{	
			fVector[i] = fHist->GetBinContent(fAlignmentPoint - fN/2 + i);
		}
	}
	else if(type == Chisquared) 
	{
		fVector.resize(fN+1);
		fVector[0] = sigma; 
		for(size_t i = 0; i < fN; i++)
		{	
			fVector[i+1] = fHist->GetBinContent(fAlignmentPoint - fN/2 + i);
		}
	}
}

void GATHistoToVector::Normalize()
{
	if(fNormalization < 0)
	{
		fNormalization = fHist->GetMaximum() - fHist->GetMinimum();
	}

	fHist->Scale(1.0/fNormalization);
	sigma = sigma/fNormalization;
}

void GATHistoToVector::FindAlignmentPoint()
{
	//default set fAlignmentPoint to T50
	double max = fHist->GetMaximum();
	double alpoint = 0.5*max; //make 0.5 a data memeber to be set
	size_t maxBin = fHist->GetMaximumBin();
	fAlignmentPoint = maxBin;

	while(fHist->GetBinContent(fAlignmentPoint) > alpoint) fAlignmentPoint--;
}

void GATHistoToVector::SetfN(size_t numOutputs)
{
	fN = numOutputs;
}

void GATHistoToVector::SetfAlignmentPoint(size_t fAlign)
{
	fAlignmentPoint = fAlign;
}

void GATHistoToVector::SetfNormalization(double fNorm)
{
	fNormalization = fNorm;
}

size_t GATHistoToVector::GetfN()
{
	return fN;
}

size_t GATHistoToVector::GetfAlignmentPoint()
{
	return fAlignmentPoint;
}

double GATHistoToVector::GetfNormalization()
{
	return fNormalization;
}
