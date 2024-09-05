#pragma once
#include <stdio.h>

class envelopeData {
public:
	envelopeData();
	envelopeData(int _index, float _position, float _increment);
	void setIncrement(float _increment);
	float read();
private:
	int index;
	float position, increment;
};