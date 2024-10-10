#include "envelopeData.h"

envelopeData::envelopeData() {

}

envelopeData::envelopeData(int _index, float _position, float _increment) {
	rowIndex = _index;
	position = _position;
	increment = _increment;
}

void envelopeData::setIncrement(float _increment) {
	increment = fmod(_increment, 4.0);
}

float envelopeData::iterate() {
	position += increment;
	envelopeIndex = trunc(position);
	if (position >= 4.0) {
		rowIndex++;
		rowIndex %= 7;
		position = fmod(position, 4.0);
	}
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