#pragma once

#include "ofMain.h"
#include <cmath>
#include "envelopeData.h"

class ofApp : public ofBaseApp {

public:
	vector<int> temporaryIndicies;
	int fillRow();
	void resetTemporaryIndicies(int rowSize);
	void printRows();
	void setup();
	ofSoundStreamSettings streamSettings;
	static const int sampleRate = 48000;
	static const int bufferSize = 256;
	static const int channels = 2;
	ofSoundStream stream;
	void ofSoundStreamSetup(ofSoundStreamSettings& settings);
	void audioSetup();
	array<array<int, 8>, 4> rows;
	array<array<float, 5>, 8> envelopes;
	array<float, 2> totalPhases, rowPhases;
	struct rowData {
		array<int, 4> rowForms;
		array<int, 4> rowElements;
		array<int, 4> rowIndicies = { 0, 0, 0, 0 };
	};
	array<array<int, 2>, 2> rowCounters;
	array<rowData, 3> rowGroups;
	int incrementIndex(int group, int index);
	array<float, 2> adjustment = { 1.0, 1.0 };
	array<int, 4> fractalLayers = { 2, 2, 2, 2 };
	array<array<envelopeData, 10>, 4> envelopeFractal;
	array<float, 9> minimumIncrements;
	float minimumIncrement, lastIncrement, increment, frequency, detune;
	int currentRowIndex, currentEnvelopeIndex;
	bool end = false;
	array<int, 4> parameterChange;
	array<int, 4> form;
	array<int, 4> transposition;
	array<float, 4> lastValues;
	array<float, 4> currentValues;
	array<float, 2> pan;
	array<float, 2> phaseIncrement;
	array<float, 2> phase;
	array<float, 2> sample;
	void audioOut(ofSoundBuffer& buffer);
};