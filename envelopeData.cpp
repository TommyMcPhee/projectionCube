#include "envelopeData.h"

envelopeData::envelopeData() {

}

envelopeData::envelopeData(int _index, float _position, float _increment) {
	index = _index;
	position = _position;
	increment = _increment;
}

void envelopeData::setIncrement(float _increment) {
	increment = _increment;
}

float envelopeData::read() {
	position += increment;
	if (position >= 4.0) {
		index++;
		index %= 7;
		position -= 4.0;
	}
	envelopeIndex = trunc(position);
	return std::lerp(envelopes[rowIndex][envelopeIndex], envelopes[rowIndex][envelopeIndex + 1], fmod(position, 1.0));
}