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
			rowGroups[a].rowForms[b] += rotation;
			rowGroups[a].rowForms[b] %= 4;
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
	minimumIncrement = 0.0000152587890625;
	for (int a = 1; a < 10; a++) {
		minimumIncrements[a - 1] = pow(minimumIncrement, 1.0 / (float)a) / (float)a;
	}
	for (int a = 0; a < 4; a++) {
		for (int b = 0; b < 10; b++) {
			envelopeFractal[a][b] = envelopeData(rand() % 8, ofRandomuf(), minimumIncrement);
		}
	}
	for (int a = 0; a < 2; a++) {
		totalPhases[a] = 0.0;
		rowPhases[a] = 1.0 + ofRandomuf();
		parameterChange[a] = 0;
	}
	audioSetup();
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

int ofApp::incrementIndex(int group, int index) {
	rowGroups[group].rowIndicies[index]++;
	rowGroups[group].rowIndicies[index] %= 8;
	rowGroups[group].rowElements[index] = rows[rowGroups[group].rowForms[index]][rowGroups[group].rowIndicies[index]];
	return rowGroups[group].rowElements[index];
}

void ofApp::audioOut(ofSoundBuffer& buffer) {
	for (int a = 0; a < buffer.getNumFrames(); a++) {
		for (int b = 0; b < 4; b++) {
			int alternate = b % 2;
			adjustment[alternate] = (totalPhases[alternate] + minimumIncrement) / (totalPhases[(alternate + 1) % 2] + minimumIncrement);
			for (int c = 0; c < fractalLayers[b]; c++) {
				int negative = fractalLayers[b] - c - 1;
				currentRowIndex = rows[form[b]][(envelopeFractal[b][c].returnRowIndex() + transposition[b]) % 7];
				currentEnvelopeIndex = envelopeFractal[b][c].returnEnvelopeIndex();
				if (c < fractalLayers[b] - 1) {
					lastValues[b] = currentValues[b];
				}
				else {
					lastValues[b] = minimumIncrement;
				}
				currentValues[b] = envelopeFractal[b][c].lerp(envelopes[currentRowIndex][currentEnvelopeIndex], envelopes[currentRowIndex][currentEnvelopeIndex + 1]);
				if (c > 0) {
					if (c < fractalLayers[b] - 1) {
						increment = (1.0 - minimumIncrements[negative]) * pow(lastValues[b] / (float)(c + 1), pow((float)c, 0.5) + 1.0) + minimumIncrements[negative];
						totalPhases[alternate] += increment;
						rowPhases[alternate] += increment;
						if (rowPhases[alternate] > 1.0) {
							for (int d = 0; d < 2; d++) {
								rowCounters[alternate][d]--;
								if (rowCounters[alternate][d] < 1) {
									if (parameterChange[alternate] < 4) {
										if (d == 0) {
											form[alternate] = parameterChange[alternate];
											parameterChange[alternate] = incrementIndex(alternate, 0);
										}
										else {
											transposition[alternate] = incrementIndex(alternate, 1);
										}
									}
									else {
										if (d == 0) {
											form[alternate + 2] = 7 - parameterChange[alternate];
											parameterChange[alternate] = incrementIndex(alternate, 0);
										}
										else {
											transposition[alternate + 2] = incrementIndex(alternate, 1);
										}
									}
									if (rowGroups[alternate].rowIndicies[alternate + 2] == 7) {
										if (rowGroups[2].rowIndicies[b] == 7) {
											cout << "end" << b << endl;
											end = true;
											ofSoundStreamClose();
										}
										fractalLayers[b] = incrementIndex(2, b) + 2;
										cout << "new layer" << endl;
									}
									rowCounters[alternate][d] = incrementIndex(alternate, d + 2) + 1;
								}
							}
							rowPhases[alternate] = fmod(rowPhases[alternate], 1.0);
						}
					}
					else {
						increment = (1.0 - minimumIncrements[negative]) * pow(lastValues[b] / (float)(c + 1), pow((float)c, 0.5) + 1.0) + minimumIncrements[negative];
					}
					envelopeFractal[b][c - 1].setIncrement(increment);
				}
				else {
					//modify to bias extreme panning and detune?
					pan[0] = currentValues[3];
					pan[1] = 1.0 - pan[0];
					frequency = abs(pow(currentValues[1], 8.0));
					detune = (2.0 * currentValues[2] - 1.0) * frequency;
					phaseIncrement[0] = frequency + detune;
					phaseIncrement[1] = frequency - detune;
					for (int d = 0; d < channels; d++) {
						phase[d] += phaseIncrement[d];
						phase[d] = fmod(phase[d], TWO_PI);
						sample[d] = sin(phase[d]) * sqrt(pan[d]) * abs(pow(currentValues[0] * pow(1.0 - phaseIncrement[d], 2.0), 4.0));
						if (end) {
							sample[d] = 0.0;
						}
						buffer[a * channels + d] = sample[d];
					}
				}
			}
		}
	}
}