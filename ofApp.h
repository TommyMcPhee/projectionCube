#pragma once

#include "ofMain.h"
#include <cmath>
#include "envelopeData.h"

class ofApp : public ofBaseApp {

public:
	vector<int> temporaryIndicies;
	int fillRow();
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
	float frequencyLimit;
	array<float, 9> frequencyLimits;
	float frameSamples;
	array<int, 4> currentRowIndicies;
	array<int, 4> currentEnvelopeIndicies;
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
	array<array<int, 2>, 4> rowLengths;
	struct rowData {
		int rowForm;
		int rowElement;
		float rowPhase = 1.1;
		float totalPhase;
	};
	array<array<rowData, 4>, 2> rowGroups;
	array<int, 4> fractalLayers;
	array<array<envelopeData, 10>, 4> envelopeFractal;
	array<int, 3> indicies;
	array<float, 2> phase;
};