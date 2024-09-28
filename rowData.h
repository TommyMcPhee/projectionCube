#pragma once

#ifndef rowData_h
#define rowData_h

#include <stdio.h>
#include <cmath>

class rowData {
public:
	rowData();
	rowData(float _increment);
	void setIncrement(float _increment);
	int iterateA();
	int iterateB();
	int iterateC();
	int iterateD();
private:
	float phaseA, phaseB, phaseC, phaseD, increment;
};

#endif
