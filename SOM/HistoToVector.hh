//--------------------------------------------
//
// Name: HistoToVector.hh
//
// Description: Converts a Waveform into a vector for the SOM
//
// Author: Carly Jo Wilson
//
//--------------------------------------------

#ifndef HISTOTOVECTOR_HH
#define HISTOTOVECTOR_HH

#include "Neuron.hh"
#include "SOM.hh"

class HistoToVector
{
	public:
		HistoToVector();
		vector<double> ConvertToVector(TH1D* h, double argNorm = -1)
		{
			fHist = h;
			fNormalization = argNorm;
			ConvertToVector();
			return fVector;
		};

		void ConvertToVector();
		void FindAlignmentPoint();
		void Normalize();

		void SetfN(size_t numOutputs);
		void SetfAlignmentPoint(size_t fAlign);
		void SetfNormalization(double fNorm);
		size_t GetfN();
		size_t GetfAlignmentPoint();
		double GetfNormalization();

	private:
		size_t fN;
		size_t fAlignmentPoint;
		double fNormalization;
		vector<double> fVector;
		TH1D* fHist;
};

#endif
