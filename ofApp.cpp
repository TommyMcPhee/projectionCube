#include "ofApp.h"

int ofApp::fillRow() {
	int randomIndex = rand() % temporaryIndicies.size();
	int element = temporaryIndicies[randomIndex];
	auto erasePosition = temporaryIndicies.begin() + randomIndex;
	temporaryIndicies.erase(erasePosition);
	return element;
}

void ofApp::printRows() {
	for (int b = 0; b < 4; b++) {
		for (int c = 0; c < 8; c++) {
			cout << rows[b][c];
		}
		cout << endl;
	}
}

void ofApp::setup() {
	bool symmetry = false;
	while (!symmetry) {
		for (int a = 0; a < 8; a++) {
			temporaryIndicies.push_back(a);
		}
		for (int a = 0; a < 8; a++) {
			int newElement = fillRow();
			rows[0][a] = newElement;
			rows[1][a] = (8 - newElement) % 8;
			rows[2][a] = newElement * 3 % 8;
			rows[3][a] = newElement * 5 % 8;
		}
		for (int a = 0; a < 8; a++) {
			if (rows[0][(8 - a) % 8] != rows[1][a] || rows[2][(8 - a) % 8] != rows[3][a]) {
				temporaryIndicies.clear();
				break;
			}
			else {
				if (a == 3) {
					printRows();
					symmetry = true;
				}
			}
		}
		temporaryIndicies.clear();
		for (int a = 0; a < 8; a++) {
			rows[1][a] = rows[0][7 - a];
			rows[2][a] = rows[0][a] * 3 % 8;
			rows[3][a] = rows[0][a] * 5 % 8;
		}
	}
	for (int a = 0; a < 4; a++) {
		temporaryIndicies.push_back(a);
	}
	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++) {
			int newElement = fillRow();
			rowGroups[a][b].rowForm = newElement;
		}
		for(int b = 0; b < a; b++){
			int identical = 0;
			for (int c = 0; c < 3; c++) {
				if (rowGroups[a][c].rowForm == rowGroups[b][c].rowForm) {
					identical++;
					if (identical > 3) {
						a--;
					}
				}
			}
		}
		rowGroups[a][3].rowForm = 3;
		int rotation = rand() % 4;
		for (int b = 0; b < 4; b++) {
			rowGroups[a][b].rowForm += rotation;
			rowGroups[a][b].rowForm %= 4;
			rowGroups[a][b].rowPhase = fmod(ofRandomuf(), 8.0);
		}
	}
	for (int a = 1; a < 10; a++) {
		frameLimit += 6.0 * (float)a;
		frameLimits[a - 1] = frameLimit;
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
			envelopeFractal[a][b] = envelopeData(rand() % 8, ofRandomuf(), frameSample * frameLimits[a]);
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
			for (int c = 0; c < fractalLayers[b]; c++) {
				currentRowIndicies[b] = envelopeFractal[b][c].returnRowIndex();
				currentEnvelopeIndicies[b] = envelopeFractal[b][c].returnEnvelopeIndex();
				lastValues[b] = currentValues[b];
				currentValues[b] = envelopeFractal[b][c].lerp(envelopes[currentRowIndicies[b]][currentEnvelopeIndicies[b]], envelopes[currentRowIndicies[b]][currentEnvelopeIndicies[b] + 1]);
				if (c > 0) {
					increment = frameSample / frameLimits[c];
					if (c == fractalLayers[b]) {
						for (int d = 0; d < 2; d++) {
							rowGroups[b % 2].rowPhases[d] += increment;
							if (rowGroups[b % 2].rowPhases[d] > 1.0) {
								rowGroups[b % 2].rowCounters[d]--;
								if (rowGroups[b % 2].rowCounters[d] < 1) {
									rowGroups[b % 2].rowIndicies[d]++;
									rowGroups[b % 2].rowCounters[d] = rowGroups[b % 2].rowElements[d + 2];
									if (rowGroups[b % 2].rowIndicies[d] > 7) {
										fractalLayers[b] = rowGroups[3].rowElements[b];
									}
								}
								rowGroups[b % 2].rowPhases[d] = fmod(rowGroups[b % 2].rowPhases[d], 1.0);
							}
						}
					}
					envelopeFractal[b][c - 1].setIncrement(currentValues[b] * lastValues[b] * increment);
				}
				else {
					pan[0] = currentValues[1];
					pan[1] = (1.0 - pan[0]);
					float phaseIncrement = currentValues[3] * frameSample / 6.0;
					float detune = (2.0 * currentValues[2] - 1.0) * currentValues[0];
					phase[0] += currentValues[0] + detune;
					phase[1] += currentValues[0] - detune;
					for (int d = 0; d < channels; d++) {
						phase[d] = fmod(phase[d], TWO_PI);
						sample[d] = sin(phase[d]) * sqrt(pan[d]) * currentValues[0];
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
	frameSample = (float)(ofGetFrameRate() / sampleRate);
	width = (float)ofGetWidth();
	height = (float)ofGetHeight();
	frameBuffer.allocate(width, height);
	ofClear(0, 0, 0, 255);
	window.set(width, height);
}