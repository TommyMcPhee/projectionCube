#pragma once

#include "ofMain.h"
#include <cmath>
#include "envelopeData.h"

class ofApp : public ofBaseApp {

public:
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
	/*
	array<int, 4> currentRowIndicies;
	array<int, 4> currentEnvelopeIndicies;
	array<float, 4> lastValues;
	array<float, 4> currentValues;
	*/
	float iterateRow(envelopeData parameter);
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
	envelopeData spreadDelta;
	envelopeData spreadEnvelope;
	envelopeData panDelta;
	envelopeData panEnvelope;
	envelopeData frequencyDelta;
	envelopeData frequencyEnvelope;
	envelopeData amplitudeDelta;
	envelopeData amplitudeEnvelope;
	float panPosition, phaseIncrement, detune;
	//array<array<envelopeData, 3>, 4> envelopeFractal;
	array<int, 3> indicies;
	array<float, 2> phase;
};