#pragma once

#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxKinect.h"
#include "Tributary.h"
#include "ofxArtnet.h"
 

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void setRandomPulse(int ms, int ps, int sep);
        void updatePulse();
        bool pulsing;
        uint64_t lastChecked;
        int pulseTime;
    
        // ARTNET
        ofxArtnet artnet;

        // SVG
        ofxSVG svg;
        float step;
        vector<Tributary> tributaries;
        vector<Tributary> universes;
        vector<ofPoint> points;
    
    
        // Kinect
        void setupKinect();
        void updateKinect();
        void drawKinect();
        ofxKinect kinect;
        #ifdef USE_TWO_KINECTS
            ofxKinect kinect2;
        #endif
        
        bool bDrawPointCloud;
        int nearThreshold;
        int farThreshold;
        int angle;
        // used for viewing the point cloud
        ofEasyCam easyCam;
        // mine
        ofPixels & pix;
};
