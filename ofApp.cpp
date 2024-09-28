#include "ofApp.h"

int ofApp::fillRow() {
	int randomIndex = rand() % temporaryIndicies.size();
	int element = temporaryIndicies[randomIndex];
	auto erasePosition = temporaryIndicies.begin() + randomIndex;
	temporaryIndicies.erase(erasePosition);
	return element;
}

void ofApp::setup() {
	bool symmetryA = false;
	bool symmetryB = false;
	while (!symmetryA && !symmetryB) {
		for (int a = 0; a < 8; a++) {
			temporaryIndicies.push_back(a);
		}
		for (int a = 0; a < 8; a++) {
			rows[0][a] = fillRow();
		}
		for (int a = 0; a < 8; a++) {
			if (rows[0][7 - a] != 7 - rows[0][a]) {
				temporaryIndicies.clear();
				break;
			}
			else {
				if (a == 3) {
					symmetryA = true;
				}
			}
		}
		temporaryIndicies.clear();
		for (int a = 0; a < 8; a++) {
			rows[1][a] = rows[0][7 - a];
			rows[2][a] = rows[0][a] * 3 % 8;
			rows[3][a] = rows[0][a] * 5 % 8;
			if (rows[2][7 - a] != rows[3][a]){
				break;
				cout << "broke" << endl;
				}
			else {
				if (a == 7) {

					cout << rows[0][a];
					cout << rows[1][a];
					cout << rows[2][a];
					cout << rows[3][a] << endl;

					symmetryB = true;
				}
			}
		}
	}
	for (int a = 0; a < 6; a++) {
		for (int b = 0; b < 4; b++) {
			temporaryIndicies.push_back(b);
		}
		for (int b = 0; b < 4; b++) {
			rowGroups[a][b] = fillRow();
		}
		for (int b = 0; b < a; b++) {
			for (int c = 0; c < 4; c++) {
				int equivelence = 0;
				if (rowGroups[b][c] == rowGroups[a][c]) {
					equivelence++;
				}
				if (equivelence > 2) {
					temporaryIndicies.clear();
					equivelence = 0;
					a--;
					b = a;
				}
			}
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
	for (int a = 0; a < 4; a++) {
		for (int b = 0; b < 10; b++) {
			envelopeFractal[a][b] = envelopeData(rand() % 8, abs(ofRandomf()), frequencyLimit);
		}
	}
	audioSetup();
	videoSetup();
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

void ofApp::audioOut(ofSoundBuffer& buffer) {
	for (int a = 0; a < buffer.getNumFrames(); a++) {
		for (int b = 0; b < 4; b++) {
			//set c equal to array[b]
			for (int c = 0; c < 10; c++) {
				currentRowIndicies[b] = envelopeFractal[b][c].returnRowIndex();
				currentEnvelopeIndicies[b] = envelopeFractal[b][c].returnEnvelopeIndex();
				lastValues[b] = currentValues[b];
				currentValues[b] = envelopeFractal[b][c].lerp(envelopes[currentRowIndicies[b]][currentEnvelopeIndicies[b]], envelopes[currentRowIndicies[b]][currentEnvelopeIndicies[b] + 1]);
				if (c > 0) {
					//if(b = ){}
					envelopeFractal[b][c - 1].setIncrement(currentValues[b] * lastValues[b] * frequencyLimit);
				}
				else {
					pan[0] = currentValues[3];
					pan[1] = (1.0 - pan[0]);
					float phaseIncrement = currentValues[0] * frequencyLimit;
					float detune = (2.0 * currentValues[1] - 1.0) * currentValues[0];
					phase[0] += currentValues[0] + detune;
					phase[1] += currentValues[0] - detune;
					for (int d = 0; d < channels; d++) {
						phase[d] = fmod(phase[d], TWO_PI);
						sample[d] = sin(phase[d]) * sqrt(pan[d]) * currentValues[2];
						buffer[a * channels + d] = sample[d];
					}
				}
			}
		}
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
	frequencyLimit = 6.0 * (float)(ofGetFrameRate() / sampleRate);
	width = (float)ofGetWidth();
	height = (float)ofGetHeight();
	frameBuffer.allocate(width, height);
	ofClear(0, 0, 0, 255);
	window.set(width, height);
}