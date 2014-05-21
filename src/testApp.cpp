#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    kinectSetup();
    
    programType = kProgramTypeNeonTrails;
    fireTrails = FireTrails();      // this calls its init method
    neonInit();
    
    switch (programType) {
        case kProgramTypeFireTrails:
            fireTrails.setup();
            break;
        case kProgramTypeNeonTrails:
            neonSetup();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::update(){
    
    switch (programType) {
        case kProgramTypeFireTrails:
            kinectUpdate();
            fireTrails.update();
            break;
        case kProgramTypeNeonTrails:
            kinectUpdate();
//            neonTrails.update();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    switch (programType) {
        case kProgramTypeFireTrails:
            fireTrails.draw();
            break;
        case kProgramTypeNeonTrails:
            drawNeonTrails();
//            neonTrails.draw();
            break;
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
//    switch (programType) {
//        case kProgramTypeFireTrails:
//            programType = kProgramTypeNeonTrails;
//            //fireTrails.keyPressed(key);
//            break;
//        case kProgramTypeNeonTrails:
//            programType = kProgramTypeFireTrails;
////            neonTrails.keyPressed(key);
//            break;
//        default:
//            break;
//    }
    switch (key) {
        case '1':
            programType = kProgramTypeNeonTrails;
            break;
        case '2':
            programType = kProgramTypeFireTrails;
            fireTrails.setup();
            break;
            
        case OF_KEY_UP:
            //			angle++;
            //			if(angle>30) angle=30;
            //			kinect.setCameraTiltAngle(angle);
            threshold--;
            cout << threshold;
            break;
            
        case OF_KEY_DOWN:
            //			angle--;
            //			if(angle<-30) angle=-30;
            //			kinect.setCameraTiltAngle(angle);
            threshold++;
            cout << threshold;
            break;
        default:
            break;

    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    switch (programType) {
        case kProgramTypeFireTrails:
            fireTrails.mouseMoved(x, y);
            break;
            
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}

/****************     KINECT FUNCTIONS    ****************/

void testApp::kinectImgInit()
{
    depthImg.allocate(kinect.width, kinect.height);
    
    for(int i = 0; i<prevFramesMax; i++){
        prevFrames.push_back(ofxCvGrayscaleImage(depthImg));
    }
    
    // May not need these imgs
    colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
}

void testApp::kinectSetup(){
    
    // enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	
#ifdef USE_TWO_KINECTS
	kinect2.init();
	kinect2.open();
#endif
    
    nearThreshold = 230;
	farThreshold = 70;
	bThreshWithOpenCV = true;
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// start from the front
	bDrawPointCloud = false;
    
    kinectImgInit();
}

void testApp::kinectUpdate(){
    kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			grayThreshNear = grayImage;
			grayThreshFar = grayImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		} else {
			
			// or we do it ourselves - show people how they can work with the pixels
			unsigned char * pix = grayImage.getPixels();
			
			int numPixels = grayImage.getWidth() * grayImage.getHeight();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					//pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
		
		// update the cv images
		grayImage.flagImageChanged();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, true);
	}
	
    updateDepthImg();
    switch (programType) {
        case kProgramTypeFireTrails:
            addFireForces();
            break;
        case kProgramTypeNeonTrails:
            updateNeonImg();
            break;
        default:
            break;
    }
    
#ifdef USE_TWO_KINECTS
	kinect2.update();
#endif

}


/***************     NEON TRAILS FUNCTIONS     ***********/

void testApp::neonInit()
{
    neonImg.allocate(kinect.width, kinect.height, OF_IMAGE_COLOR_ALPHA);
}

void testApp::neonSetup()
{
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(false);
    ofBackground(125, 25, 190);
    ofSetFrameRate(30);
    
    nearThreshold = 230;
	farThreshold = 70;
	bThreshWithOpenCV = true;
}

void testApp::updateDepthImg()      // this is more of a gen kinect func
{
    if(kinect.isFrameNew())
    {
        updatePrevFrames();         // be sure to update prev frames first
		
        // load grayscale depth image from the kinect source
		depthImg.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
        
        // or we do it ourselves - show people how they can work with the pixels
        unsigned char * pix = depthImg.getPixels();
        
        int numPixels = depthImg.getWidth() * depthImg.getHeight();
        for(int i = 0; i < numPixels; i++) {
            if(pix[i] > threshold) {
                //pix[i] = 255;
            } else {
                pix[i] = 0;
            }
        }
		
		// update the cv images
		depthImg.flagImageChanged();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		//contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, true);
	}

}

void testApp::updatePrevFrames()
{
    if(prevFrames.size() <= prevFramesCounter)
    {
        prevFrames.push_back(depthImg);
    }
    else
    {
        prevFrames[prevFramesCounter] = depthImg;
    }
    prevFramesCounter = (++prevFramesCounter) % prevFramesMax;
}

void testApp::addFireForces()
{
    if(prevFrames.size() == prevFramesMax)
    {
        unsigned char *pix = depthImg.getPixels();
        unsigned char *prevPix = prevFrames[prevFramesCounter].getPixels();
        
        
        //int numPixels = depthImg.getWidth() * depthImg.getHeight();
        int counter = 0;
        int spacing = 25;
        int up = 1;
        for(int y = 0; y < depthImg.getHeight(); y++)
        {
            for(int x = 0; x < depthImg.getWidth(); x++)
            {
                int i = y*depthImg.getWidth()+x;
                if(abs(prevPix[i] - pix[i]) > motionThresh && pix[i] != 0)
                {
                    if(counter++ % spacing == 0)
                    {
                       int posX = x*(ofGetWindowWidth()/depthImg.getWidth());
                       int posY = y*(ofGetWindowHeight()/depthImg.getHeight());
                        
                       // int posX = x*(depthImg.getWidth()/ofGetWindowWidth());
                       // int posY = y*(depthImg.getHeight()/ofGetWindowHeight());
                       // int posX = x;
                       // int posY = y;
                        
                       // ofVec2f pos = ofVec2f(posX,posY);
                        ofVec2f pos = ofVec2f(posX,posY) / ofGetWindowSize();
                        
                        //up *= -1;
                        ofVec2f vel = ofVec2f(0, -0.005) / ofGetWindowSize();
                        
                        fireTrails.addToFluid(pos, vel, true, true);
                        //void addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce);
                    }
                }
            }
            
        }
    }
}


void testApp::updateNeonImg()
{
    if(prevFrames.size() == prevFramesMax);
    {
        stringstream reportStream;
        reportStream << "updateTrailsImg()"<<endl;
        
        unsigned char *pix = depthImg.getPixels();
        //unsigned char *prevPix = prevFrames[(prevFramesCounter+1)%prevFramesMax].getPixels();
        unsigned char *prevPix = prevFrames[prevFramesCounter].getPixels();
        //unsigned char *trailsPix = trailsImg.getPixels();
        
        unsigned char *neonPix = neonImg.getPixels();
        
        int numPixels = depthImg.getWidth() * depthImg.getHeight();
        
        for(int i = 0; i < numPixels; i++)
        {
            if(abs(prevPix[i] - pix[i]) > motionThresh && pix[i] != 0)
            {
                //ofColor c(254, 30, 128, CHAR_MAX);
                //neonImg.setColor(i, c);
                neonPix[4*i] = 255;
                neonPix[4*i + 1] = 30;
                neonPix[4*i + 2] = 128;
                neonPix[4*i + 3] = CHAR_MAX;
            }
            else
            {
                //ofColor c(0, 0, 0, trail);
                //neonImg.setColor(i, c);
                neonPix[4*i] = 0;
                neonPix[4*i + 1] = 0;
                neonPix[4*i + 2] = 0;
                neonPix[4*i + 3] = trail;
                //trailsPix[i] = 0;
            }
        }
        neonImg.update();
    }
}

void testApp::drawNeonTrails(){
    
	ofSetColor(255, 255, 255);
	
	if(bDrawPointCloud) {
		easyCam.begin();
		//drawPointCloud();
		easyCam.end();
	} else {
		// draw from the live kinect
		//kinect.drawDepth(10, 10, 400, 300);
		kinect.draw(420, 20, 400, 300);
		
		grayImage.draw(10, 320, 400, 300);
		contourFinder.draw(10, 320, 400, 300);
        
        
        
        //  ofColor c(0);
        //ofBackground(c);
        
        //ofSetColor(c);
        //ofRect(10, 20, 400, 300);
        
        neonImg.draw(10, 20, 400, 300);
        
        //neonImg.draw(10, 20, 400, 300);
        
        depthImg.draw(420, 320, 400, 300);
		
#ifdef USE_TWO_KINECTS
		kinect2.draw(420, 320, 400, 300);
#endif
	}
	
	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
    
    if(kinect.hasAccelControl()) {
        reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
        << ofToString(kinect.getMksAccel().y, 2) << " / "
        << ofToString(kinect.getMksAccel().z, 2) << endl;
    } else {
        reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
		<< "motor / led / accel controls are not currently supported" << endl << endl;
    }
    
	reportStream << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
	<< "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
	<< "set near threshold " << nearThreshold << " (press: + -)" << endl
	<< "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
	<< ", fps: " << ofGetFrameRate() << endl
	<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
    
    if(kinect.hasCamTiltControl()) {
    	reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
        << "press 1-5 & 0 to change the led mode" << endl;
    }
    
	ofDrawBitmapString(reportStream.str(), 20, 652);
    
}

void testApp::exit(){
    kinect.setCameraTiltAngle(0);   // zero tilt on exit
    kinect.close();
    
#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}






