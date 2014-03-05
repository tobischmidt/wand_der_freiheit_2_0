#include "ofMain.h"
#include "testApp.h"

using namespace std;

int main() {
	ofSetupOpenGL(1024,768,OF_WINDOW);
	ofRunApp(new testApp());

}
