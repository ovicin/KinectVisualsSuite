//
//  NeonTrails.h
//  SDMPVisuals
//
//  Created by Chris Hall on 5/18/14.
//
//

#ifndef __SDMPVisuals__NeonTrails__
#define __SDMPVisuals__NeonTrails__

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS

class NeonTrails : public ofBaseApp {
public:
    NeonTrails();
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void drawPointCloud();
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
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
    
    /******** NEON TRAILS *******/
    void setupTrails();
    void updateDepthImg();
    void updatePrevFrames();
    void updateTrailsImg();
    
    int threshold = 50;
    int motionThresh = 1;
    int updateCounter = 0;
    ofxCvGrayscaleImage depthImg;
    ofxCvColorImage trailsImg;
    ofImage neonImg;
    vector< ofxCvGrayscaleImage > prevFrames;
    int prevFramesCounter = 0;
    int prevFramesMax = 5;
    unsigned char trail = 30;
    int camW = 640;
    int camH = 480;
    
    
    /****************************/
    
    
    
    
	
	// used for viewing the point cloud
	ofEasyCam easyCam;
};

#endif /* defined(__SDMPVisuals__NeonTrails__) */
