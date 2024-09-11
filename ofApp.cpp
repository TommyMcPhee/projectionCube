#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	vector<int> indicies;
	for (int a = 0; a < 8; a++) {
		indicies.push_back(a);
	}
	for (int a = 0; a < 8; a++) {
		int randomIndex = rand() % indicies.size();
		rows[0][a] = indicies[randomIndex];
		auto erasePosition = indicies.begin() + randomIndex;
		indicies.erase(erasePosition);
	}
	//check for undesirable symmetries before proceeding
	for (int a = 0; a < 8; a++) {
		rows[2][a] = 7 - rows[0][a];
		rows[4][a] = rows[0][a] * 3 % 8;
		rows[6][a] = rows[0][a] * 5 % 8;
	}
	for (int a = 0; a < 8; a++) {
		for (int b = 1; b < 8; b++) {
			rows[b][a] = rows[b - 1][7 - a];
		}
	}
	static const float oneThird = 1.0 / 3.0;
	static const float twoThirds = 2.0 / 3.0;
	envelopes[0] = { 0.0, 0.25, 0.5, 0.75, 1.0 };
	envelopes[1] = { 0.0, oneThird, twoThirds, 1.0, 0.0 };
	envelopes[2] = { 0.0, 0.5, 1.0, 0.5, 0.0 };
	envelopes[3] = { 0.0, 1.0, twoThirds, oneThird, 0.0 };
	envelopes[4] = { 1.0, 0.75, 0.5, 0.25, 0.0 };
	envelopes[5] = { 1.0, twoThirds, oneThird, 0.0, 1.0 };
	envelopes[6] = { 1.0, 0.5, 0.0, 0.5, 1.0 };
	envelopes[7] = { 1.0, 0.0, oneThird, twoThirds, 1.0 };
	for (int a = 0; a < layers; a++) {
		envelopeFractal[a] = envelopeData(rand() % 8, 0.0, pow(abs(ofRandomf()), 1 + a));
	}
	audioSetup();
	//videoSetup();
}

void ofApp::ofSoundStreamSetup(ofSoundStreamSettings& settings) {

}

void ofApp::audioSetup() {
	streamSettings.setOutListener(this);
	streamSettings.setApi(ofSoundDevice::Api::MS_WASAPI);
	streamSettings.sampleRate = sampleRate;
	streamSettings.bufferSize = bufferSize;
	streamSettings.numOutputChannels = channels;
	stream.setup(streamSettings);
}

void ofApp::videoSetup() {
	shader.load("serialShader");
	frameBuffer.allocate(ofGetScreenWidth(), ofGetScreenHeight());
	frameBuffer.clear();
}
//temporary
/*
float ofApp::lerp(float inputA, float inputB, float mix) {
	return inputB * (1.0 - mix) + (inputA * mix);
}
*/
//
void ofApp::audioOut(ofSoundBuffer& buffer) {
	for (int a = 0; a < buffer.getNumFrames(); a++) {
		pan[0] = 0.5;
		pan[1] = (1.0 - pan[0]);
		for (int b = 0; b < layers; b++) {
			int currentRowIndex = envelopeFractal[b].returnRowIndex();
			int currentEnvelopeIndex = envelopeFractal[b].returnEnvelopeIndex();
			float envelopeValue = envelopeFractal[b].lerp(envelopes[currentRowIndex][currentEnvelopeIndex], envelopes[currentRowIndex][currentEnvelopeIndex + 1]);
			if (b > 0) {
				envelopeFractal[b - 1].setIncrement(envelopeValue / (float)(pow(b + layers, b)));
			}
			else {
				for (int c = 0; c < channels; c++) {
					sample[c] = envelopeValue;
					buffer[a * channels + c] = sample[c];
				}
			}
		}
		/*
		for (int b = 0; b < channels; b++) {
			envelope[0] += envelope[1];
			if (envelope[0] >= 4.0) {
				envelope[0] -= 4.0;
				rowIndex++;
				rowIndex %= 7;
			}
			int rowValue = rows[0][rowIndex];
			int envelopeIndex = trunc(envelope[0]);
			//sample[b] = sqrt(pan[b]) * lerp(envelopes[rowValue][envelopeIndex], envelopes[rowIndex][envelopeIndex + 1], fmod(envelope[0], 1.0)) * 2.0 - 1.0;
			buffer[a * channels + b] = sample[b];
		}
		*/
	}
}


void ofApp::setUniforms() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	refresh();
	frameBuffer.begin();
	shader.begin();
	setUniforms();
	frameBuffer.draw(0, 0);
	shader.end();
	frameBuffer.end();
	frameBuffer.draw(0.0, 0.0);
}

void ofApp::refresh() {
	width = (float)ofGetWidth();
	height = (float)ofGetHeight();
	frameBuffer.allocate(width, height);
	ofClear(0, 0, 0, 255);
	window.set(width, height);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}