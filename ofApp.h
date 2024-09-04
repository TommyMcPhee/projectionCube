#pragma once

#include "ofMain.h"
#include <cmath>

class ofApp : public ofBaseApp {

public:
	void setup();
	ofSoundStreamSettings streamSettings;
	static const int sampleRate = 48000;
	static const int bufferSize = 256;
	static const int channels = 2;
	ofSoundStream stream;
	void ofSoundStreamSetup(ofSoundStreamSettings& settings);
	void audioSetup();
	void videoSetup();
	array<float, 2> pan;
	array<float, 2> sample;
	float lerp(float inputA, float inputB, float mix);
	float lookup(int index, float envelopeData);
	void audioOut(ofSoundBuffer& buffer);
	float width;
	float height;
	ofVec2f window;
	ofFbo frameBuffer;
	ofShader shader;
	void refresh();
	void setUniforms();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	array<int, 8> row;
	array<array<float, 5>, 8> envelopes;
	static const int layers = 4;
	array<array<float, 2>, layers> envelopeFractal;
	array<float, 2> envelope = { 0.0, 0.01 };
	int rowIndex = 0;
};