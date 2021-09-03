#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	ofGLFWWindowSettings settings;
	settings.setSize(1024, 768);
	settings.resizable = false;
	ofCreateWindow(settings);

	ofSetVerticalSync(true);

	ofRunApp(new ofApp());
}
