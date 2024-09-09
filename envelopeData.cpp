#include "../envelopeData.h"

envelopeData::envelopeData() {

}

envelopeData::envelopeData(int _index, float _position, float _increment) {
	rowIndex = _index;
	position = _position;
	increment = _increment;
}

void envelopeData::setIncrement(float _increment) {
	increment = _increment;
}

float envelopeData::iterate() {
	position += increment;
	if (position >= 4.0) {
		rowIndex++;
		rowIndex %= 7;
		position -= 4.0;
	}
	envelopeIndex = trunc(position);
	return fmod(position, 1.0);
}

int envelopeData::returnRowIndex() {
	return rowIndex;
}

int envelopeData::returnEnvelopeIndex() {
	return envelopeIndex;
}

float envelopeData::lerp(float inputA, float inputB) {
	float mix = iterate();
	return inputB * (1.0 - mix) + (inputA * mix);
}