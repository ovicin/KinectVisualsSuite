//#include "ofMain.h"
//#include "testApp.h"
//
////========================================================================
//int main( ){
//	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context
//
//	// this kicks off the running of my app
//	// can be OF_WINDOW or OF_FULLSCREEN
//	// pass in width and height too:
//	ofRunApp(new testApp());
//
//}



#include "testApp.h"
#include "ofMain.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
    
    ofAppGlutWindow window;
	//ofSetupOpenGL(&window, 640*2,480*2, OF_WINDOW);			// <-------- setup the GL context
    ofSetupOpenGL(&window, 640*2, 480*2, OF_FULLSCREEN);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());
    
}