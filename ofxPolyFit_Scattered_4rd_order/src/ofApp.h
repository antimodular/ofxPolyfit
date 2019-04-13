#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxPolyFit.h"

#include <sstream>

#include <stdio.h>

typedef double DataType;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
    
      void    initialiseData();

    void makeLookUpTables();
    void openLookUpTable();
    
    pfitDataSet <DataType> training_dataSet;
    
    ofxPolyFit_<DataType>	polynomial_fit_bicubic;
    int camWidth, camHeight;
    int referencePointSize;
    float gridSize;
    bool switchDistribution;
    
    vector<float> panArray;
    vector<float> tiltArray;
 

    float opened_panArray[1280*720];
     float opened_tiltArray[1280*720];
};

#endif
