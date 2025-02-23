#pragma once

#include "ofMain.h"
#include "FireTrails.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ColorBG.h"
//#include "NeonTrails.h"

// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS

class testApp : public ofBaseApp{
    
public:
    
    enum kProgramType{
        kProgramTypeNeonTrails,
        kProgramTypeFireTrails,
        kProgramTypeColorBG,
        kProgramTypeImgBG
    };
    
/**************** CORE FUNCTIONS ***************/
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    kProgramType programType;
    
    FireTrails fireTrails;
    void addFireForces();
   // NeonTrails neonTrails;
    
/************    KINECT  STUFF  ******************/
    void kinectSetup();
    void kinectImgInit();
    void kinectUpdate();
    void updateDepthImg();
    void updatePrevFrames();

    ofxKinect kinect;

#ifdef USE_TWO_KINECTS
	ofxKinect kinect2;
#endif
	
	ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
	ofxCvContourFinder contourFinder;
    
    bool bThreshWithOpenCV;
	bool bDrawPointCloud;
	
    int nearThreshold;
	int farThreshold;
	int angle;
    bool isMirror;
    
    ofEasyCam easyCam;
    
    
/************ Colors ***********/
    ofColor getNeonColor();
    
/************   NEON TRAILS   ******************/
    
    void neonInit();
    void neonSetup();
    void updateNeonImg();
    void neonKeyPressed(int key);
    
    void drawNeonTrails();
    
    int threshold = 50;
    int motionThresh = 5;
    //int motionThresh = 10;
    int updateCounter = 0;
    ofxCvGrayscaleImage depthImg;
    ofImage neonImg;
    vector< ofxCvGrayscaleImage > prevFrames;
    int prevFramesCounter = 0;
    int prevFramesMax = 5;
    unsigned char trail = 30;
    int camW = 640;
    int camH = 480;
    
    bool hidden = false;
    bool overlayHidden = true;

    
    
/**********   IMG  *********/
    
    vector< ofImage > imgVec;
    int imgIndex = 0;
    
    void drawImgTrails();
    void imgBGKeyPressed(int key);
    

/*******  ColorBG  ******/
    
    ColorBG colorBG;
    
    
    
};
