//
//  FireTrails.cpp
//  NeonFireCombo
//
//  Created by Chris Hall on 5/18/14.
//
//

#include "FireTrails.h"

//char sz[] = "[Rd9?-2XaUP0QY[hO%9QTYQ`-W`QZhcccYQY[`b";


float tuioXScaler = 1;
float tuioYScaler = 1;

//--------------------------------------------------------------
FireTrails::FireTrails()
{
    ofLog(OF_LOG_NOTICE, "FireTrails instance being created ");
    
    colorMode = kColorFire;
    
#ifdef USE_GUI
	gui.addSlider("fluidCellsX", fluidCellsX, 20, 400);
	gui.addButton("resizeFluid", resizeFluid);
    gui.addSlider("colorMult", colorMult, 0, 100);
    gui.addSlider("velocityMult", velocityMult, 0, 100);
	gui.addSlider("fs.viscocity", fluidSolver.viscocity, 0.0, 0.01);
	gui.addSlider("fs.colorDiffusion", fluidSolver.colorDiffusion, 0.0, 0.0003);
	gui.addSlider("fs.fadeSpeed", fluidSolver.fadeSpeed, 0.0, 0.1);
	gui.addSlider("fs.solverIterations", fluidSolver.solverIterations, 1, 50);
	gui.addSlider("fs.deltaT", fluidSolver.deltaT, 0.1, 5);
	gui.addComboBox("fd.drawMode", (int&)fluidDrawer.drawMode, msa::fluid::getDrawModeTitles());
	gui.addToggle("fs.doRGB", fluidSolver.doRGB);
	gui.addToggle("fs.doVorticityConfinement", fluidSolver.doVorticityConfinement);
	gui.addToggle("drawFluid", drawFluid);
	gui.addToggle("drawParticles", drawParticles);
	gui.addToggle("fs.wrapX", fluidSolver.wrap_x);
	gui.addToggle("fs.wrapY", fluidSolver.wrap_y);
    gui.addSlider("tuioXScaler", tuioXScaler, 0, 2);
    gui.addSlider("tuioYScaler", tuioYScaler, 0, 2);
    
	gui.currentPage().setXMLName("ofxMSAFluidSettings.xml");
    gui.loadFromXML();
	gui.setDefaultKeys(true);
	gui.setAutoSave(true);
    //gui.show();
#endif

    
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<FxaaPass>()->setEnabled(true);
    post.createPass<BloomPass>()->setEnabled(true);
    post.createPass<DofPass>()->setEnabled(false);
    post.createPass<KaleidoscopePass>()->setEnabled(false);
    post.createPass<NoiseWarpPass>()->setEnabled(false);
    post.createPass<PixelatePass>()->setEnabled(false);
    post.createPass<EdgePass>()->setEnabled(false);
    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(false);

    light.setPosition(1000, 1000, 1000);
    
    //init();
   // setup();
}

void FireTrails::init(){
    char sz[] = "[Rd9?-2XaUP0QY[hO%9QTYQ`-W`QZhcccYQY[`b";
    
	for(int i=0; i<strlen(sz); i++) sz[i] += 20;
	
	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.1).setDeltaT(0.5).setVisc(0.000005);
	fluidDrawer.setup(&fluidSolver);
	
	fluidCellsX			= 150;
	
	drawFluid			= true;
	drawParticles		= true;
	
#ifdef USE_TUIO
	tuioClient.start(3333);
#endif
    
	
#ifdef USE_GUI
	gui.addSlider("fluidCellsasdjfk;asdfj laskdjfX", fluidCellsX, 20, 400);
	gui.addButton("resizeFluid", resizeFluid);
    gui.addSlider("colorMult", colorMult, 0, 100);
    gui.addSlider("velocityMult", velocityMult, 0, 100);
	gui.addSlider("fs.viscocity", fluidSolver.viscocity, 0.0, 0.01);
	gui.addSlider("fs.colorDiffusion", fluidSolver.colorDiffusion, 0.0, 0.0003);
	gui.addSlider("fs.fadeSpeed", fluidSolver.fadeSpeed, 0.0, 0.1);
	gui.addSlider("fs.solverIterations", fluidSolver.solverIterations, 1, 50);
	gui.addSlider("fs.deltaT", fluidSolver.deltaT, 0.1, 5);
	gui.addComboBox("fd.drawMode", (int&)fluidDrawer.drawMode, msa::fluid::getDrawModeTitles());
	gui.addToggle("fs.doRGB", fluidSolver.doRGB);
	gui.addToggle("fs.doVorticityConfinement", fluidSolver.doVorticityConfinement);
	gui.addToggle("drawFluid", drawFluid);
	gui.addToggle("drawParticles", drawParticles);
	gui.addToggle("fs.wrapX", fluidSolver.wrap_x);
	gui.addToggle("fs.wrapY", fluidSolver.wrap_y);
    gui.addSlider("tuioXScaler", tuioXScaler, 0, 2);
    gui.addSlider("tuioYScaler", tuioYScaler, 0, 2);
    
	gui.currentPage().setXMLName("ofxMSAFluidSettings.xml");
    gui.loadFromXML();
	gui.setDefaultKeys(true);
	gui.setAutoSave(true);
    //gui.show();
#endif
	
    //windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)
	pMouse = msa::getWindowCenter();
	resizeFluid			= true;
}

void FireTrails::fireSetup() {
        char sz[] = "[Rd9?-2XaUP0QY[hO%9QTYQ`-W`QZhcccYQY[`b";
    for(int i=0; i<strlen(sz); i++) sz[i] += 20;

	// setup fluid stuff
	fluidSolver.setup(100, 100);
    //fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
    fluidSolver.enableRGB(true).setFadeSpeed(0.1).setDeltaT(0.5).setVisc(0.000005);
	fluidDrawer.setup(&fluidSolver);

	fluidCellsX			= 150;

	drawFluid			= true;
	drawParticles		= false;

	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
    ofSetVerticalSync(true);
//	ofSetVerticalSync(false);
    ofSetCoordHandedness(OF_RIGHT_HANDED);

#ifdef USE_TUIO
	tuioClient.start(3333);
#endif


//#ifdef USE_GUI
//	gui.addSlider("fluidCellsX", fluidCellsX, 20, 400);
//	gui.addButton("resizeFluid", resizeFluid);
//    gui.addSlider("colorMult", colorMult, 0, 100);
//    gui.addSlider("velocityMult", velocityMult, 0, 100);
//	gui.addSlider("fs.viscocity", fluidSolver.viscocity, 0.0, 0.01);
//	gui.addSlider("fs.colorDiffusion", fluidSolver.colorDiffusion, 0.0, 0.0003);
//	gui.addSlider("fs.fadeSpeed", fluidSolver.fadeSpeed, 0.0, 0.1);
//	gui.addSlider("fs.solverIterations", fluidSolver.solverIterations, 1, 50);
//	gui.addSlider("fs.deltaT", fluidSolver.deltaT, 0.1, 5);
//	gui.addComboBox("fd.drawMode", (int&)fluidDrawer.drawMode, msa::fluid::getDrawModeTitles());
//	gui.addToggle("fs.doRGB", fluidSolver.doRGB);
//	gui.addToggle("fs.doVorticityConfinement", fluidSolver.doVorticityConfinement);
//	gui.addToggle("drawFluid", drawFluid);
//	gui.addToggle("drawParticles", drawParticles);
//	gui.addToggle("fs.wrapX", fluidSolver.wrap_x);
//	gui.addToggle("fs.wrapY", fluidSolver.wrap_y);
//    gui.addSlider("tuioXScaler", tuioXScaler, 0, 2);
//    gui.addSlider("tuioYScaler", tuioYScaler, 0, 2);
//
//	gui.currentPage().setXMLName("ofxMSAFluidSettings.xml");
//    gui.loadFromXML();
//	gui.setDefaultKeys(true);
//	gui.setAutoSave(true);
//    gui.show();
//#endif

    //windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)
	pMouse = msa::getWindowCenter();
	resizeFluid			= true;

	ofEnableAlphaBlending();
	ofSetBackgroundAuto(false);
}



//void FireTrails::setup() {
//    ofSetFrameRate(60);
//	ofBackground(0, 0, 0);
//	ofSetVerticalSync(false);
//    
//    ofEnableAlphaBlending();
//	ofSetBackgroundAuto(false);
//}


void FireTrails::fadeToColor(float r, float g, float b, float speed) {
    glColor4f(r, g, b, speed);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
}


// add force and dye to fluid, and create particles
void FireTrails::addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce) {
    float speed = vel.x * vel.x  + vel.y * vel.y * msa::getWindowAspectRatio() * msa::getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
    if(speed > 0) {
		pos.x = ofClamp(pos.x, 0.0f, 1.0f);
		pos.y = ofClamp(pos.y, 0.0f, 1.0f);
		
        int index = fluidSolver.getIndexForPos(pos);
		
		if(addColor) {
            //			Color drawColor(CM_HSV, (getElapsedFrames() % 360) / 360.0f, 1, 1);
			ofColor drawColor;
            float hsbNum;
            switch (colorMode) {
                case kColorFire:
                    hsbNum = ofRandom(0.3);
                    drawColor.setHsb(hsbNum, 255, 255);
                    break;
                case kColorBlue:
                    hsbNum = ofRandom(0.5) + 169.6;
                    drawColor.setHsb( hsbNum, 255, 255);
                    break;
                case kColorPurple:
                    hsbNum = ofRandom(0.4) + 170;
                    drawColor.setHsb( hsbNum, 255, 255);
                    break;
                    
//                case kColorMulti:
//                {
//                    drawColor.setHsb((ofGetFrameNum()/300) % 360 , 255, 255);
//                    break;
//                }
            }

            //drawColor.setHsb(ofGetFrameNum() % 2 , 255, 255);
			//drawColor.setHsb((ofGetFrameNum() % 255), 255, 255);
			
			fluidSolver.addColorAtIndex(index, drawColor * colorMult);
			
			if(drawParticles)
				particleSystem.addParticles(pos * ofVec2f(ofGetWindowSize()), 3);
		}
		
		if(addForce)
			fluidSolver.addForceAtIndex(index, vel * velocityMult);
        
		
    }
}


void FireTrails::fireUpdate(){
	if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX / msa::getWindowAspectRatio());
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}
	
#ifdef USE_TUIO
	tuioClient.getMessage();
	
	// do finger stuff
	list<ofxTuioCursor*>cursorList = tuioClient.getTuioCursors();
	for(list<ofxTuioCursor*>::iterator it=cursorList.begin(); it != cursorList.end(); it++) {
		ofxTuioCursor *tcur = (*it);
        float vx = tcur->getXSpeed() * tuioCursorSpeedMult;
        float vy = tcur->getYSpeed() * tuioCursorSpeedMult;
        if(vx == 0 && vy == 0) {
            vx = ofRandom(-tuioStationaryForce, tuioStationaryForce);
            vy = ofRandom(-tuioStationaryForce, tuioStationaryForce);
        }
        addToFluid(ofVec2f(tcur->getX() * tuioXScaler, tcur->getY() * tuioYScaler), ofVec2f(vx, vy), true, true);
    }
#endif
    
    //ofVec2f pos = ofVec2f(mouseX, mouseY)/ ofGetWindowSize();
    ofVec2f pos = ofVec2f(150, 150)/ ofGetWindowSize();
    ofVec2f vel = ofVec2f(0, -2) / ofGetWindowSize();
	addToFluid(pos, vel, true, true);
	
	fluidSolver.update();
}

void FireTrails::fireDraw(){
//    // copy enable part of gl state
//    glPushAttrib(GL_ENABLE_BIT);
//    
//    // setup gl state
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    light.enable();
    
    // begin scene to post process
    post.begin();
    
        if(drawFluid) {
            ofClear(0);
            glColor3f(1, 1, 1);
            fluidDrawer.draw(0, 0, ofGetWidth(), ofGetHeight());
        } else {
            //		if(ofGetFrameNum()%5==0)
            fadeToColor(0, 0, 0, 0.01);
        }
        if(drawParticles)
            particleSystem.updateAndDraw(fluidSolver, ofGetWindowSize(), drawFluid);
    post.end();
//    glPopAttrib();
    //	ofDrawBitmapString(sz, 50, 50);
    
#ifdef USE_GUI
	gui.draw();
#endif
}


void FireTrails::keyPressed  (int key){
    switch(key) {
//		case '1':
//			fluidDrawer.setDrawMode(msa::fluid::kDrawColor);
//			break;
//            
//		case '2':
//			fluidDrawer.setDrawMode(msa::fluid::kDrawMotion);
//			break;
//            
//		case '3':
//			fluidDrawer.setDrawMode(msa::fluid::kDrawSpeed);
//			break;
//			
//		case '4':
//			fluidDrawer.setDrawMode(msa::fluid::kDrawVectors);
//			break;
        case OF_KEY_RIGHT:
            fireForce += fireForceInc;
            break;
        
        case OF_KEY_LEFT:
            fireForce -= fireForceInc;
            break;
        case 'w':
            fireForce = 0.005;
            break;
        case 's':
            fireForce = -0.005;
        
        case 'c':
            switch (colorMode) {
                case kColorFire:
                    colorMode = kColorBlue;
                    break;
                case kColorBlue:
                    colorMode = kColorPurple;
                    break;
                case kColorPurple:
                    colorMode = kColorFire;
                    break;
                case kColorMulti:
                    colorMode = kColorFire;
                    break;
                default:
                    colorMode = kColorFire;
                    break;
            }
            break;
            
		case 'd':
			drawFluid ^= true;
			break;
			
		case 'p':
			drawParticles ^= true;
			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
			
		case 'r':
			fluidSolver.reset();
			break;
			
		case 'b': {
            //			Timer timer;
            //			const int ITERS = 3000;
            //			timer.start();
            //			for(int i = 0; i < ITERS; ++i) fluidSolver.update();
            //			timer.stop();
            //			cout << ITERS << " iterations took " << timer.getSeconds() << " seconds." << std::endl;
		}
			break;
			
    }
}


//--------------------------------------------------------------
void FireTrails::mouseMoved(int x, int y){
	ofVec2f eventPos = ofVec2f(x, ofGetWindowHeight() - y);
	ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
	ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
	addToFluid(mouseNorm, mouseVel, true, true);
	pMouse = eventPos;
}

void FireTrails::mouseDragged(int x, int y, int button) {
	ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
	ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
	addToFluid(mouseNorm, mouseVel, false, true);
	pMouse = eventPos;
}
