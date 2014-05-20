#include "NeonTrails.h"


//--------------------------------------------------------------
NeonTrails::NeonTrails()
{
    
}


void NeonTrails::setup() {
    ofEnableAlphaBlending();
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetBackgroundAuto(false);
    ofBackground(123, 0, 200);
	// enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
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
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
    
    neonImg.allocate(kinect.width, kinect.height, OF_IMAGE_COLOR_ALPHA);
	
	nearThreshold = 230;
	farThreshold = 70;
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(30);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// start from the front
	bDrawPointCloud = false;
    
    setupTrails();
    
    //    ofColor c(255);
    //    //ofBackground(c);
    //
    //    ofSetColor(c);
    //    ofRect(10, 20, 400, 300);
}

//--------------------------------------------------------------
void NeonTrails::update() {
	
	//ofBackground(100, 100, 100);
	
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
    updateTrailsImg();
    
#ifdef USE_TWO_KINECTS
	kinect2.update();
#endif
}

/*************************************************************
 *******************    NEON TRAILS STUFF     ****************
 *************************************************************/

void NeonTrails::setupTrails()
{
    
    depthImg.allocate(kinect.width, kinect.height);
	trailsImg.allocate(kinect.width, kinect.height);
    
    //depthImg = *new ofxCvGrayscaleImage();
    
    for (int i = 0; i < prevFramesMax; i++) {
        prevFrames.push_back(ofxCvGrayscaleImage(depthImg));
    }
    //trailsImg = *new ofxCvColorImage();
    
}

void NeonTrails::updateDepthImg()
{
    if(kinect.isFrameNew()) {
		
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

void NeonTrails::updatePrevFrames()
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

void NeonTrails::updateTrailsImg()
{
    if(prevFrames.size() == prevFramesMax);
    {
        stringstream reportStream;
        reportStream << "updateTrailsImg()"<<endl;
        
        unsigned char *pix = depthImg.getPixels();
        //unsigned char *prevPix = prevFrames[(prevFramesCounter+1)%prevFramesMax].getPixels();
        unsigned char *prevPix = prevFrames[prevFramesCounter].getPixels();
        unsigned char *trailsPix = trailsImg.getPixels();
        
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
        //neonImg.setFromPixels(neonPix, neonImg.getWidth(), neonImg.getHeight(), OF_IMAGE_COLOR_ALPHA);
        //ofColor c = ofColor(255);
        //neonImg.setColor(c);
        
        
        
        //neonImg.setFromPixels(neonPix, neonImg.getWidth(), neonImg.getHeight(), OF_IMAGE_COLOR_ALPHA);
        
        
        
        //        for(int i = 0; i < numPixels; i++)
        //        {
        //            unsigned char *rgbPix;
        //            if(abs(prevPix[i] - pix[i]) > motionThresh)
        //            {
        //                trailsPix[3*i] = 255;
        //                trailsPix[3*i + 1] = 30;
        //                trailsPix[3*i + 2] = 128;
        //            }
        //            else
        //            {
        //                trailsPix[3*i] = 0;
        //                trailsPix[3*i + 1] = 0;
        //                trailsPix[3*i + 2] = 0;
        //                //trailsPix[i] = 0;
        //            }
        //        }
        //        trailsImg.flagImageChanged();
    }
}

//--------------------------------------------------------------
void NeonTrails::draw() {
	
	ofSetColor(255, 255, 255);
	
	if(bDrawPointCloud) {
		easyCam.begin();
		drawPointCloud();
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
        //trailsImg.draw(10, 10, 400, 300);
        
        
        
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

void NeonTrails::drawPointCloud() {
	int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect.getDistanceAt(x, y) > 0) {
				mesh.addColor(kinect.getColorAt(x,y));
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
			}
		}
	}
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
	mesh.drawVertices();
	ofDisableDepthTest();
	ofPopMatrix();
}

//--------------------------------------------------------------
void NeonTrails::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}

//--------------------------------------------------------------
void NeonTrails::keyPressed (int key) {
    ofLogNotice() << "Trail: " << (int)trail << endl;
	switch (key) {
            
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;
			
		case '>':
		case '.':
            threshold++;
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
            threshold--;
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;
			
		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;
			
		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;
			
		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;
			
		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
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
            
        case OF_KEY_RIGHT:
            trail--;
            if(trail < 0) trail = 0;
            break;
        case OF_KEY_LEFT:
            trail++;
            if(trail > 254) trail = 254;
            break;
	}
}

//--------------------------------------------------------------
void NeonTrails::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void NeonTrails::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void NeonTrails::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void NeonTrails::windowResized(int w, int h)
{}
