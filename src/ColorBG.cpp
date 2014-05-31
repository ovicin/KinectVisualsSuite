//
//  ColorBG.cpp
//  SDMPVisuals_master
//
//  Created by Chris Hall on 5/28/14.
//
//

#include "ColorBG.h"

ColorBG::ColorBG()
{
    isChangingColor = true;
    color = ofColor(0, 0, 0);
    darkness = 40;
}

void ColorBG::setup(){
    
}

void ColorBG::update(){
    if(isChangingColor)
        color = getNextColor();
}

ofColor ColorBG::getNextColor()
{
    float theta = (ofGetElapsedTimef()) / (15 * PI);
    float r = abs(sin(theta)*255);
    float g = abs(sin(theta + 2)*255);
    float b = abs(sin(theta + 4)*255);
    
    return ofColor(r,g,b);
}

void ColorBG::draw()
{
    ofFill();
    ofSetColor(color);
        ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        ofSetColor(0, 0, 0, darkness);
            ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    ofNoFill();
    
    //ofRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());


}

void ColorBG::keyPressed(int key)
{
    switch(key)
    {
            case OF_KEY_RIGHT:
                ++darkness;
                break;
            case OF_KEY_LEFT:
                --darkness;
                break;
            
            case 'C':
            case 'c':
                isChangingColor = !isChangingColor;
                break;
            
            default:
            break;
            
    }
}