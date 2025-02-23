//
//  ColorBG.h
//  SDMPVisuals_master
//
//  Created by Chris Hall on 5/28/14.
//
//

#ifndef __SDMPVisuals_master__ColorBG__
#define __SDMPVisuals_master__ColorBG__

#include <iostream>
#include "ofMain.h"

class ColorBG{

public:
    ColorBG();
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    bool isImageMode;
    
    
    /**** Image Stuff ****/
    vector<ofImage> imgVec;
    ofImage image;
    int imgIndex;
    
    
    /**** Color Stuff *****/
    
    ofColor getNextColor();
    
    int darkness;
    bool isChangingColor;
    ofColor color;
    
};



#endif /* defined(__SDMPVisuals_master__ColorBG__) */
