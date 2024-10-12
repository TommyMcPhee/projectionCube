#include "ofApp.h"

void ofApp::resetTemporaryIndicies(int rowSize) {
	temporaryIndicies.clear();
	for (int z = 0; z < rowSize; z++) {
		temporaryIndicies.push_back(z);
	}
}

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
		resetTemporaryIndicies(8);
		for (int a = 0; a < 8; a++) {
			int newElement = fillRow();
			rows[0][a] = newElement;
			rows[1][a] = (8 - newElement) % 8;
			rows[2][a] = newElement * 3 % 8;
			rows[3][a] = newElement * 5 % 8;
		}
		for (int a = 0; a < 8; a++) {
			if (rows[0][(8 - a) % 8] != rows[1][a] || rows[2][(8 - a) % 8] != rows[3][a]) {
				break;
			}
			else {
				if (a == 3) {
					printRows();
					symmetry = true;
				}
			}
		}
		for (int a = 0; a < 8; a++) {
			rows[1][a] = rows[0][7 - a];
			rows[2][a] = rows[0][a] * 3 % 8;
			rows[3][a] = rows[0][a] * 5 % 8;
		}
	}
	for (int a = 0; a < 3; a++) {
		resetTemporaryIndicies(4);
		for (int b = 0; b < 4; b++) {
			int newElement = fillRow();
			rowGroups[a].rowForms[b] = newElement;
		}
		for (int b = 0; b < a; b++) {
			int identical = 0;
			for (int c = 0; c < 3; c++) {
				if (rowGroups[a].rowForms[c] == rowGroups[b].rowForms[c]) {
					identical++;
					if (identical > 2) {
						a--;
					}
				}
			}
		}
		rowGroups[a].rowForms[3] = 3;
		int rotation = rand() % 4;
		for (int b = 0; b < 4; b++) {
			//rowGroups[a].rowIndicies[b] = rand() % 8;
			rowGroups[a].rowForms[b] += rotation;
			rowGroups[a].rowForms[b] %= 4;
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
	for (int a = 0; a < 2; a++) {
		totalPhases[a] = 0.0;
		rowPhases[a] = 1.0 + ofRandomuf();
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

int ofApp::incrementIndex(int group, int index) {
	rowGroups[group].rowIndicies[index]++;
	rowGroups[group].rowIndicies[index] %= 8;
	rowGroups[group].rowElements[index] = rows[rowGroups[group].rowForms[index]][rowGroups[group].rowIndicies[index]];
	return rowGroups[group].rowElements[index];
}

void ofApp::audioOut(ofSoundBuffer& buffer) {
	for (int a = 0; a < buffer.getNumFrames(); a++) {
		for (int b = 0; b < 4; b++) {
			for (int c = 0; c < fractalLayers[b]; c++) {
				currentRowIndex = rows[form[b]][(envelopeFractal[b][c].returnRowIndex() + transposition[b]) % 7];
				currentEnvelopeIndex = envelopeFractal[b][c].returnEnvelopeIndex();
				lastValues[b] = currentValues[b];
				currentValues[b] = envelopeFractal[b][c].lerp(envelopes[currentRowIndex][currentEnvelopeIndex], envelopes[currentRowIndex][currentEnvelopeIndex + 1]);
				if (c > 0) {
					increment = pow(1.0 - lastValues[b], 1.0 - lastValues[b]) * frameSample / frameLimits[c];
					if (c == fractalLayers[b] - 1) {
						totalPhases[b % 2] += increment;
						rowPhases[b % 2] += increment;
						if (rowPhases[b % 2] > 1.0) {
							parameterChange[b % 2] = incrementIndex(b % 2, 0);
							if (parameterChange[b % 2] < 4) {
								form[b % 2] = parameterChange[b % 2];
								transposition[b % 2] = incrementIndex(b % 2, 1);
							}
							else {
								form[(b % 2) + 2] = 7 - parameterChange[b % 2];
								transposition[(b % 2) + 2] = incrementIndex(b % 2, 1);
							}
							rowGroups[b % 2].rowCounters[b % 2]--;
							if (rowGroups[b % 2].rowCounters[b % 2] < 1) {
								if (rowGroups[b % 2].rowIndicies[(b % 2) + 2] == 7) {
									if (rowGroups[2].rowIndicies[b] == 7) {
										cout << "end" << b << endl;
										ofSoundStreamClose();
									}
									fractalLayers[b] = incrementIndex(2, b) + 2;
									cout << "new layer" << endl;
								}
								rowGroups[b % 2].rowCounters[b % 2] = incrementIndex(b % 2, (b % 2) + 2) + 1;
							}
							rowPhases[b % 2] = fmod(rowPhases[b % 2], 1.0);
						}
					}
					envelopeFractal[b][c - 1].setIncrement(increment);
				}
				else {
					pan[0] = currentValues[3];
					pan[1] = (1.0 - pan[0]);
					float phaseIncrement = abs(pow(currentValues[1], 5.0));
					float detune = (2.0 * currentValues[2] - 1.0) * phaseIncrement;
					phase[0] += phaseIncrement + detune;
					phase[1] += phaseIncrement - detune;
					for (int d = 0; d < channels; d++) {
						phase[d] = fmod(phase[d], TWO_PI);
						sample[d] = sin(phase[d]) * sqrt(pan[d]) * abs(pow(currentValues[0], 3.0));
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