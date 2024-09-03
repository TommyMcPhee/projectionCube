#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	ofSoundStreamSettings streamSettings;
	static const int sampleRate = 48000;
	static const int bufferSize = 256;
	static const int channels = 2;
	void audioSetup();
	void videoSetup();
	void ofSoundStreamSetup(ofSoundStreamSettings &settings);
	array<float, 2> sample;
	void audioOut(ofSoundBuffer &buffer);
	ofFbo frameBuffer;
	ofShader shader;
	void refresh();
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
};