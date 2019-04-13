#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxPolyFit.h"

#include <sstream>


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
    
      void    initialiseData(bool initMethode);

    pfitDataSet <DataType> training_dataSet;
    
    ofxPolyFit_<DataType>	polynomial_fit_bicubic;
    int camWidth, camHeight;
    int referencePointSize;
    float gridSize;
    bool switchDistribution;
    
};

#endif
