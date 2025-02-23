//
//  FireTrails.h
//  NeonFireCombo
//
//  Created by Chris Hall on 5/18/14.
//
//

#ifndef __NeonFireCombo__FireTrails__
#define __NeonFireCombo__FireTrails__


#endif /* defined(__NeonFireCombo__FireTrails__) */

#pragma once

#include "MSAFluid.h"
//#include "MSATimer.h"
#include "ParticleSystem.h"

#include "ofMain.h"

#include "ofxPostProcessing.h"

// comment this line out if you don't wanna use the GUI
// you will need ofxSimpleGuiToo, ofxMSAInteractiveObject & ofxXmlSettings
// if you don't use the GUI, you won't be able to see the fluid parameters
#define USE_GUI

#ifdef USE_GUI
#include "ofxSimpleGuiToo.h"
#endif

class FireTrails{
public:
    
    enum kColor{
        kColorFire,
        kColorBlue,
        kColorPurple,
        kColorMulti,
        kColorMax
    };
    
    FireTrails();
    
    void init();
	void fireSetup();
	void fireUpdate();
	void fireDraw();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
    
	void fadeToColor(float r, float g, float b, float speed);
	void addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce);
    
    void resetParticleCount(void);
    unsigned long getParticleCount(void);
    
    float                   colorMult;
    float                   velocityMult;
	int                     fluidCellsX;
	bool                    resizeFluid;
	bool                    drawFluid;
	bool                    drawParticles;
    float                   fireForce = 0.005;
    float                   fireForceInc = 0.0005;
    kColor                  colorMode;
	
	msa::fluid::Solver      fluidSolver;
	msa::fluid::DrawerGl	fluidDrawer;
	
	ParticleSystem          particleSystem;
	
    unsigned long           particleCount;
	ofVec2f                 pMouse;
	
#ifdef USE_TUIO
	ofxTuioClient tuioClient;
#endif
    bool bo_0FxaaPass;
    bool bo_1BloomPass;
    bool bo_2ofPass;
    bool bo_3KaleidoscopePass;
    bool bo_4NoiseWarpPass;
    bool bo_5PixelatePass;
    bool bo_6EdgePass;
    bool bo_7VerticalTiltShifPass;
    bool bo_8GodRaysPass;
    /************** SHADERS  ***************/
    ofxPostProcessing post;
    ofLight light;
    
    ofSoundPlayer sndPlayer;
    int curSound;
};
