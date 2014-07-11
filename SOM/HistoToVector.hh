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
		vector<double> ConvertToVector(TH1F* h, double argNorm = -1)
		{
			fHist = h;
			fNormalization = argNorm;
			ConvertToVector;
			return fVector;
		};

		void ConvertToVector();
		void FindAlignmentPoint();
		void Normalize();

};

#endif
