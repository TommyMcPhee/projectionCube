#include "rowData.h"

rowData::rowData() {

}

int rowData::iterateA(){
	phaseA += increment;
	phaseA = fmod(increment, 8.0);
	return trunc(phaseA);
}

int rowData::iterateB() {
	phaseB += increment;
	phaseB = fmod(increment, 8.0);
	return trunc(phaseB);
}

int rowData::iterateC() {
	phaseC += increment;
	phaseC = fmod(increment, 8.0);
	return trunc(phaseC);
}

int rowData::iterateD(){
	phaseD += increment;
	phaseD = fmod(increment, 8.0);
	return trunc(phaseD);
}