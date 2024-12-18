#pragma once

#ifndef envelopeData_h
#define envelopeData_h

#include <stdio.h>
#include <cmath>

class envelopeData {
public:
	envelopeData();
	envelopeData(int _rowIndex, float _position, float _increment);
	void setIncrement(float _increment);
	int returnRowIndex();
	int returnEnvelopeIndex();
	float lerp(float inputA, float inputB);
	int rowIndex, envelopeIndex;
private:
	float iterate();
	float position = 0.0, increment;
};

#endif