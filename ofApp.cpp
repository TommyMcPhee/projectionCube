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
	rescale = sqrt(8);
	for (int a = 1; a < 9; a++) {
		minimums[a - 1] = 1.0 / pow(2.0, pow(pow((float)a, 0.5) + 1.0, 2.0));
	}
	for (int a = 0; a < 4; a++) {
		for (int b = 0; b < 9; b++) {
			envelopeFractal[a][b] = envelopeData(rand() % 8, ofRandomuf(), minimumIncrement);
		}
	}

	rowPhase = 0.0;
	for (int a = 0; a < 2; a++) {
		parameterChange[a] = 0;
	}
	setupWav();
	audioSetup();
	//WHY IS THIS NEEDED?!? Array subscript out of range if the for loop is deleted
	for (int a = 0; a < 0; a++) {
	}
}

void ofApp::setupWav() {
	wavFile.open("projectionCubeRecording.wav", ios::binary);
	wavFile << "RIFF";
	wavFile << "----";
	wavFile << "WAVE";
	wavFile << "fmt ";
	writeToFile(wavFile, byteDepth * 8, 4);
	writeToFile(wavFile, 1, 2);
	writeToFile(wavFile, channels, 2);
	writeToFile(wavFile, sampleRate, 4);
	writeToFile(wavFile, sampleRate * byteDepth, 4);
	writeToFile(wavFile, byteDepth, 2);
	writeToFile(wavFile, byteDepth * 8, 2);
	wavFile << "data";
	wavFile << "----";
	preAudioP = wavFile.tellp();
	maxSampleInt = pow(2, byteDepth * 8 - 1) - 1;
}

void ofApp::writeToFile(ofstream& file, int value, int size) {
	file.write(reinterpret_cast<const char*> (&value), size);
}

void ofApp::recordSample(int channel) {
	sampleInt = static_cast<int>(sample[channel] * maxSampleInt);
	wavFile.write(reinterpret_cast<char*> (&sampleInt), byteDepth);
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

float ofApp::bipolar(float input) {
	return input * 2.0 - 1.0;
}

void ofApp::audioOut(ofSoundBuffer& buffer) {
	for (int a = 0; a < buffer.getNumFrames(); a++) {
		for (int b = 0; b < 4; b++) {
			int alternate = b % 2;
			for (int c = 0; c < fractalLayers[b]; c++) {
				int negative = fractalLayers[b] - c;
				currentRowIndex = rows[form[b]][(envelopeFractal[b][c].returnRowIndex() + transposition[b]) % 7];
				currentEnvelopeIndex = envelopeFractal[b][c].returnEnvelopeIndex();
				currentValues[b] = envelopeFractal[b][c].lerp(envelopes[currentRowIndex][currentEnvelopeIndex], envelopes[currentRowIndex][currentEnvelopeIndex + 1]);
				if (c > 0) {
					if (c < fractalLayers[b] - 1) {
						lastValues[b] = currentValues[b];
						increment = (1.0 - minimums[c]) * pow(lastValues[b] / (float)(c + 1), pow((float)c, 0.5) + 1.0) + minimums[c];
					}
					else {
						lastValues[b] = minimumIncrement;
						increment = minimumIncrement;
						rowPhase += increment;
						if (rowPhase > 2.0 || change) {
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
											cout << "end" << endl;
											end = true;
											wavFile.close();
											ofSoundStreamClose();
										}
										fractalLayers[b] = incrementIndex(2, b) + 2;
										//rescale = sqrt((float)(10 - fractalLayers[b])) + 1.0;
									}
									rowCounters[alternate][d] = incrementIndex(alternate, d + 2) + 1;
								}
							}
							rowPhase = fmod(rowPhase, 2.0);
							if (change) {
								change = false;
							}
							else {
								change = true;
							}
						}
					}
					envelopeFractal[b][c - 1].setIncrement(increment);
				}
				else {
					if (b == 3) {
						pan[0] = sinh(bipolar(currentValues[3])) * 0.5 + 0.5;
						pan[1] = 1.0 - pan[0];
						frequency = abs(pow(currentValues[1], rescale));
						detune = sinh(bipolar(currentValues[2])) * frequency;
						phaseIncrement[0] = frequency + detune;
						phaseIncrement[1] = frequency - detune;
						for (int d = 0; d < channels; d++) {
							phase[d] += phaseIncrement[d];
							phase[d] = fmod(phase[d], TWO_PI);
							sample[d] = sin(phase[d]) * sqrt(abs(pan[d])) * abs(pow(currentValues[0] * pow(1.0 - phaseIncrement[d], rescale), rescale));
							if (end) {
								sample[d] = 0.0;
							}
							recordSample(d);
							buffer[a * channels + d] = sample[d];
						}
					}
				}
			}
		}
	}
}