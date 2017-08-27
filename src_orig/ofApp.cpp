#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(40);

    
    ofBackground(255);
    ofSetColor(0);
    
    
    ///////////////////////////////////////
    // ARTNET
    //at first you must specify the Ip address of this machine
    // artnet.setup("10.0.0.4"); //make sure the firewall is deactivated at this point
    
    ///////////////////////////////////////
    // SVG
    vector<ofPolyline> outlines;
    svg.load("install.svg");
    float h = svg.getHeight();
    //float factor = 490/h;
    float factor = (33*30.48)/ svg.getWidth();
    for (int i = 0; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        for(int j=0;j<(int)lines.size();j++){
            outlines.push_back(lines[j].getResampledBySpacing(1));
        }
    }
    vector<int> pathOrder = { 39,34,35, 42,28, 29, 9,18, 44,26,27,19,21,8, 41,20, 7,36,10,11, 5, 22, 0, 1, 2,13,15,38,37,14, 12, 25, 3, 4, 17, 23,16,24,6,31,30,43,32,33,40};
    
    //svg.getNumPath()
    for( int k = 0; k < pathOrder.size(); k++) {
        ofPath& mypath = svg.getPathAt(pathOrder[k]);
        const vector<ofPolyline>& polylines = mypath.getOutline();
        tributaries.push_back(*new Tributary( k, pathOrder[k], polylines[0], factor));
    }
    
    tributaries[16].addTributaryEnd(tributaries[17]);
    tributaries[30].addTributaryEnd(tributaries[26]);
    tributaries[30].addTributaryEnd(tributaries[28]);
    
//    tributaries.erase(tributaries.begin() + 17);
//    tributaries.erase(tributaries.begin() + 26 - 1);
//    tributaries.erase(tributaries.begin() + 28 - 2);
    
    
    int erase[] = {17, 26, 28}; //, 29, 30, 31, 32, 37, 40};
    
    for (int i = 2; i >= 0; i--) {
         tributaries.erase(tributaries.begin() + erase[i]);
    }
    
    
    pulsing = false;
    //setRandomPulse(40, 2, 30); // millis, packet size, separation
   
    float totalLEDs = 0;
    float totalCentimeters = 0;
    for( int i = 0; i < tributaries.size(); i++) {
        totalLEDs += tributaries[i].pixels.size();
        totalCentimeters += tributaries[i].len;
    }
    float dmxLEDs = totalLEDs * 3;
    float divideByUniverses = dmxLEDs / 8;
    
    cout << "total meters: ";
    cout << totalCentimeters/100 << endl;
    cout << "total LEDs: ";
    cout << totalLEDs << endl;
    cout << "total DMX channels: ";
    cout << dmxLEDs << endl;
    ///////////////////////////////////////
    
    setupKinect();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    
    if (pulsing) {
        if (ofGetElapsedTimeMillis() < lastChecked) lastChecked = 0;
        else if (ofGetElapsedTimeMillis() - lastChecked > pulseTime) {
            for( int i = 0; i < tributaries.size(); i++) {
                tributaries[i].updatePulse();
            }
            lastChecked = ofGetElapsedTimeMillis();
        }
    }
    updateKinect();
}

//--------------------------------------------------------------
void ofApp::draw(){
    //displayTributaries();
    ofSetColor(220);
    ofDrawRectangle(0, 0, 33*30.48, 17*30.48);
    int erase[] = {29, 30, 31, 32, 37, 40};
    for( int i = 0; i < tributaries.size(); i++) {
        if (i == 29 || i == 30 || i == 31 || i == 32 || i == 37 || i ==40) tributaries[i].draw(ofColor(255, 0, 0));
        else tributaries[i].draw(ofColor(0, 0, 255));
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

//--------------------------------------------------------------



void ofApp::setRandomPulse(int ms, int ps, int sep){
    pulsing = true;
    pulseTime = ms;
    for( int i = 0; i < tributaries.size(); i++) {
        tributaries[i].setRandomPulse(ps, sep);
    }
}



//--------------------------------------------------------------
//void ofApp::sendDMX(ofDragInfo dragInfo){
    // list nodes for sending with subnet / universe
    // artnet.sendDmx("10.0.0.149", 0xf, 0xf, testImage.getPixels(), 512);
    // artnet.sendDmx("10.0.0.149", testImage.getPixels().getData(), 512);

//    int numUniverses = 8;
//    for (var i = 1; i <= numUniverses; i++) {
//        const unsigned char *data512 = 0;
//        // artnet.sendDmx(ip, subnet, univeres, data, size)
//        artnet.sendDmx("10.0,0,149", 0xf, i, *data512, 512);
//    }
//}


//--------------------------------------------------------------
void ofApp::setupKinect() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    
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
    
    nearThreshold = 230;
    farThreshold = 70;
    bThreshWithOpenCV = true;
    
    ofSetFrameRate(60);
    
    // zero the tilt on startup
    angle = 0;
    kinect.setCameraTiltAngle(angle);
    
    // start from the front
    bDrawPointCloud = false;
}

//--------------------------------------------------------------
void ofApp::updateKinect() {
    
    ofBackground(100, 100, 100);
    
    kinect.update();
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels());
        
        // or we do it ourselves - show people how they can work with the pixels
        pix = grayImage.getPixels();
        int numPixels = pix.size();
        for(int i = 0; i < numPixels; i++) {
            if(pix[i] < nearThreshold && pix[i] > farThreshold) {
                pix[i] = 255;
            } else {
                pix[i] = 0;
            }
        }
    }
    
#ifdef USE_TWO_KINECTS
    kinect2.update();
#endif
}

//--------------------------------------------------------------
void ofApp::drawKinect() {
    
    ofSetColor(255, 255, 255);
    
    if(bDrawPointCloud) {
        easyCam.begin();
        drawPointCloud();
        easyCam.end();
    } else {
        // draw from the live kinect
        kinect.drawDepth(10, 10, 400, 300);
        kinect.draw(420, 10, 400, 300);
        
        grayImage.draw(10, 320, 400, 300);
        contourFinder.draw(10, 320, 400, 300);
        
#ifdef USE_TWO_KINECTS
        kinect2.draw(420, 320, 400, 300);
#endif
    }
    
    // draw instructions
    ofSetColor(255, 255, 255);
    stringstream reportStream;
    
}



//--------------------------------------------------------------
void ofApp::exit() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
    
#ifdef USE_TWO_KINECTS
    kinect2.close();
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
    switch (key) {
        case ' ':
            bThreshWithOpenCV = !bThreshWithOpenCV;
            break;
            
        case'p':
            bDrawPointCloud = !bDrawPointCloud;
            break;
            
        case '>':
        case '.':
            farThreshold ++;
            if (farThreshold > 255) farThreshold = 255;
            break;
            
        case '<':
        case ',':
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
            angle++;
            if(angle>30) angle=30;
            kinect.setCameraTiltAngle(angle);
            break;
            
        case OF_KEY_DOWN:
            angle--;
            if(angle<-30) angle=-30;
            kinect.setCameraTiltAngle(angle);
            break;
    }
}

