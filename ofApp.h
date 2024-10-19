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
	void videoSetup();
	float frameSample, lastIncrement, minimumIncrement, increment;
	int currentRowIndex, currentEnvelopeIndex;
	bool end = false;
	array<int, 4> parameterChange;
	array<int, 4> form;
	array<int, 4> transposition;
	array<float, 4> lastValues;
	array<float, 4> currentValues;
	array<float, 2> pan;
	array<float, 2> sample;
	void audioOut(ofSoundBuffer& buffer);
	float width;
	float height;
	ofVec2f window;
	ofFbo frameBuffer;
	ofShader shader;
	void refresh();
	void setUniforms();
	void draw();
	array<array<int, 8>, 4> rows;
	array<array<float, 5>, 8> envelopes;
	array<float, 2> totalPhases, rowPhases;
	struct rowData {
		array<int, 4> rowForms;
		array<int, 4> rowElements;
		array<int, 4> rowIndicies = { 0, 0, 0, 0 };
	};
	array<array<int, 2>, 2> rowCounters;
	int incrementIndex(int group, int index);
	array<rowData, 3> rowGroups;
	array<int, 4> fractalLayers = { 2, 2, 2, 2 };
	array<array<envelopeData, 10>, 4> envelopeFractal;
	array<int, 3> indicies;
	array<float, 2> phaseIncrement;
	array<float, 2> phase;
};