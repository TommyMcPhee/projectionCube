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
	int rowIndex, envelopeIndex = 0;
private:
	float iterate();
	float position, increment;
};

#endif