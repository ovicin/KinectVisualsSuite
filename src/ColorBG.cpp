//
//  ColorBG.cpp
//  SDMPVisuals_master
//
//  Created by Chris Hall on 5/28/14.
//
//

#include "ColorBG.h"

#define NUM_OF_IMGS 7

ColorBG::ColorBG()
{
    isChangingColor = true;
    color = ofColor(0, 0, 0);
    darkness = 40;
    
    isImageMode = true;
    imgIndex = 0;
    
// Manually populate imgVec
    ofImage img;
    
    
    img.loadImage("images/glowgirl.jpg");
    float imgRatio = img.getHeight()/img.getWidth();
    img.resize(ofGetScreenWidth() * 1/imgRatio, ofGetHeight());
    imgVec.push_back(img);
    
    
    img.loadImage("images/hammock_file-small.jpg");
    imgRatio = img.getHeight()/img.getWidth();
    img.resize(ofGetScreenWidth() * 1/imgRatio, ofGetHeight());
    imgVec.push_back(img);

    img.loadImage("images/moon-art.jpg");
    imgRatio = img.getHeight()/img.getWidth();
    img.resize(ofGetScreenWidth(), ofGetHeight() * imgRatio);
    imgVec.push_back(img);

    img.loadImage("images/platonic-solids.jpg");
    imgRatio = img.getHeight()/img.getWidth();
    img.resize(ofGetScreenWidth(), ofGetHeight());
    imgVec.push_back(img);
    
    
    img.loadImage("images/tetrahedron3.jpg");
    imgRatio = img.getHeight()/img.getWidth();
    img.resize(ofGetScreenWidth(), ofGetHeight() * imgRatio);
    imgVec.push_back(img);
    
    img.loadImage("images/tetrahedron4.jpg");
    imgRatio = img.getHeight()/img.getWidth();
    img.resize(ofGetScreenWidth(), ofGetHeight() * imgRatio);
    imgVec.push_back(img);
    
    img.loadImage("images/tribal-pattern-sky.jpg");
    imgRatio = img.getHeight()/img.getWidth();
    img.resize(ofGetScreenWidth(), ofGetHeight() * imgRatio);
    imgVec.push_back(img);
}

void ColorBG::setup(){
    
}

void ColorBG::update(){
    if(isChangingColor && !isImageMode)
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
    if(isImageMode)
    {
        //image.draw(0,0);
        imgVec[imgIndex].draw(0, 0);
    }
    else
    {
        ofFill();
        ofSetColor(color);
            ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
            ofSetColor(0, 0, 0, darkness);
                ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        
        ofNoFill();
    }
    
    //ofRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());


}

void ColorBG::keyPressed(int key)
{
    switch(key)
    {
            case OF_KEY_RIGHT:
                if(isImageMode)
                    (++imgIndex) % NUM_OF_IMGS;
                else
                    ++darkness;
                break;
            case OF_KEY_LEFT:
                if(isImageMode)
                    (--imgIndex) % NUM_OF_IMGS;
                else
                    --darkness;
                break;
            
            case 'I':
            case 'i':
                isImageMode = !isImageMode;
                break;
            case 'C':
            case 'c':
                isChangingColor = !isChangingColor;
                break;
            
            default:
            break;
            
    }
}